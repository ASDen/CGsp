//taken from the Polyhedron CGAL demo

typedef Polyhedron::Halfedge_handle Halfedge_handle;

typedef CGAL::Triangulation_2_filtered_projection_traits_3<Kernel>     TTraits;

typedef CGAL::Triangulation_vertex_base_with_info_2<Halfedge_handle,
                                                    TTraits>            Vb;

struct Face_info {
  Halfedge_handle e[3];
  bool is_external;
};

typedef CGAL::Triangulation_face_base_with_info_2<Face_info,
                                                  TTraits>              Fb1;

typedef CGAL::Constrained_triangulation_face_base_2<TTraits, Fb1>       Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                    TDS;
typedef CGAL::No_intersection_tag                                      Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<TTraits, TDS,Itag>   CDTbase;
typedef CGAL::Constrained_triangulation_plus_2<CDTbase>                CDT;

typedef Polyhedron::HalfedgeDS HDS;

class CGSP_CC Traingulate : public Modifier
{
	class Triangulate_modifier : public CGAL::Modifier_base<HDS> {
		CDT* cdt;
		Polyhedron::Facet_handle fh;

	public:
		Triangulate_modifier(CDT* cdt,
			Polyhedron::Facet_handle fh) 
			: cdt(cdt), fh(fh)
		{
		}

		bool is_external(CDT::Face_handle fh) const {
			return fh->info().is_external;
		}

		void operator()(HDS& hds) {
			CGAL::HalfedgeDS_decorator<HDS> decorator(hds);
			typedef Polyhedron::Halfedge Halfedge;

			decorator.make_hole(fh->halfedge());
			for(CDT::Finite_edges_iterator
				eit = cdt->finite_edges_begin(),
				end = cdt->finite_edges_end();
			eit != end; ++eit)
			{
				CDT::Face_handle fh = eit->first;
				const int index = eit->second;
				CDT::Face_handle opposite_fh = fh->neighbor(eit->second);
				const int opposite_index = opposite_fh->index(fh);
				const CDT::Vertex_handle va = fh->vertex(cdt-> cw(index));
				const CDT::Vertex_handle vb = fh->vertex(cdt->ccw(index));

				if( ! (is_external(fh) && is_external(opposite_fh)) && 
					! cdt->is_constrained(*eit) ) 
				{
					// strictly internal edge
					Halfedge_handle h = hds.edges_push_back(Halfedge(),
						Halfedge());
					fh->info().e[index] = h;
					opposite_fh->info().e[opposite_index] = h->opposite();

					decorator.set_vertex(h, va->info()->vertex());
					decorator.set_vertex(h->opposite(), vb->info()->vertex());
				}
				if( cdt->is_constrained(*eit) )
				{
					if(!is_external(fh)) {
						fh->info().e[index] = va->info();
					}
					if(!is_external(opposite_fh)) {
						opposite_fh->info().e[opposite_index] = vb->info();
					}
				}
			}
			for(CDT::Finite_faces_iterator
				fit = cdt->finite_faces_begin(),
				end = cdt->finite_faces_end();
			fit != end; ++fit)
			{
				if(!is_external(fit)) 
				{
					Halfedge_handle h0 = fit->info().e[0];
					Halfedge_handle h1 = fit->info().e[1];
					Halfedge_handle h2 = fit->info().e[2];
					CGAL_assertion( h0 != Halfedge_handle() );
					CGAL_assertion( h1 != Halfedge_handle() );
					CGAL_assertion( h2 != Halfedge_handle() );

					typedef Halfedge::Base HBase;
					h0->HBase::set_next(h1);
					decorator.set_prev(h1, h0);
					h1->HBase::set_next(h2);
					decorator.set_prev(h2, h1);
					h2->HBase::set_next(h0);
					decorator.set_prev(h0, h2);

					decorator.fill_hole(h0);        
				}
			}
		}
	};

public:

	void Do(Polyhedron &P)
	{
		Polyhedron* pMesh = &P;
		if(!pMesh) return;
		if(pMesh->is_pure_triangle()) {
			//already trinagulated
			return;
		}

		typedef Polyhedron::Facet Facet;
		typedef Polyhedron::Facet_iterator Facet_iterator;
		typedef Polyhedron::Facet_handle Facet_handle;

		// One need to store facet handles into a vector, because the list of
		// facets of the polyhedron will be modified during the loop, and
		// that invalidates the range [facets_begin(), facets_end()[.
		std::vector<Facet_handle> facets;
		facets.reserve(pMesh->size_of_facets());
		for(Facet_iterator 
			fit = pMesh->facets_begin(),
			end = pMesh->facets_end();
		fit != end; ++fit) {
			facets.push_back(fit);
		}

		// Iterates on the vector of facet handles
		for(std::vector<Facet_handle>::iterator 
			fit_it = facets.begin(),
			end = facets.end();
		fit_it != end; ++fit_it)
		{
			Facet_handle fit = *fit_it;
			Kernel::Vector_3 normal = compute_facet_normal<Facet,Kernel>(*fit);

			TTraits cdt_traits(normal);
			CDT cdt(cdt_traits);

			Facet::Halfedge_around_facet_circulator 
				he_circ = fit->facet_begin(),
				he_circ_end(he_circ);
			CDT::Vertex_handle previous, first;
			do {
				CDT::Vertex_handle vh = cdt.insert(he_circ->vertex()->point());
				if(first == 0) {
					first = vh;
				}
				vh->info() = he_circ;
				if(previous != 0 && previous != vh) {
					cdt.insert_constraint(previous, vh);
				}
				previous = vh;
			} while( ++he_circ != he_circ_end );
			cdt.insert_constraint(previous, first);

			// sets mark is_external
			for(CDT::Finite_faces_iterator
				fit = cdt.finite_faces_begin(),
				end = cdt.finite_faces_end();
			fit != end; ++fit)
			{
				fit->info().is_external = false;
			}
			std::queue<CDT::Face_handle> face_queue;
			face_queue.push(cdt.infinite_vertex()->face());
			while(! face_queue.empty() ) {
				CDT::Face_handle fh = face_queue.front();
				face_queue.pop();
				CGAL_assertion(cdt.is_infinite(fh));
				if(fh->info().is_external) continue;
				//std::cerr << (void*)(&*fh) << std::endl;
				fh->info().is_external = true;
				for(int i = 0; i <3; ++i) {
					if(!cdt.is_constrained(std::make_pair(fh, i)))
					{
						face_queue.push(fh->neighbor(i));
					}
				}
			}
			// then modify the polyhedron
			Triangulate_modifier modifier(&cdt, fit);
			pMesh->delegate(modifier);
		}
		CGAL_assertion_code(pMesh->normalize_border());
		CGAL_assertion(pMesh->is_valid(false, 3));

	}
};