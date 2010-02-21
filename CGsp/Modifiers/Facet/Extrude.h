class Extrude : public Modifier
{
public:
	AnimatablePropery<int,Interpolator> Fnum;
	AnimatablePropery<double,Interpolator> ExAmount;
	
	Extrude(int FaceNum, double Amount) : Fnum(FaceNum),ExAmount(Amount)
	{
		props.push_back(&Fnum);
		props.push_back(&ExAmount);
	}

	void Do(Polyhedron &P)
	{
		if(Fnum.val < 0 || Fnum.val > P.size_of_facets ())
			return;

		Plane_const_iterator px = P.planes_begin();
		Facet_iterator iter = P.facets_begin();
		Eigen::Transform3d t;

		std::advance (iter,Fnum.val);
		std::advance (px  ,Fnum.val);
		Halfedge_facet_circulator h = iter->facet_begin();
		Point_3 o = h->vertex()->point(); //arbitary face vertex
		
		t.setIdentity();
		t.translate(Eigen::Vector3d(0,0,ExAmount.val));
		Aff3 aff = CalcExecInFaceCoordSys(t,iter,px,o);

		int size = CGAL::circulator_size (h);
		Halfedge_handle g = P.create_center_vertex (h);

		Point_3 ux;
		
		ux = h->vertex()->point();
		P.split_edge (h->next());
		ux = ux.transform(aff);
		h->next()->vertex ()->point() = ux;

		Facet_iterator f = P.facets_end();
		std::advance (f, 1 - size);

		Halfedge_facet_circulator s;

		for(int i = 1; i < size; ++i)
		{   
			s = f->facet_begin();
			ux = s->vertex()->point();
			P.split_edge (s->next ());
			ux = ux.transform(aff);
			s->next ()->vertex ()->point ()=ux;
			std::advance (f,1);	
		}

		f = P.facets_end();
		std::advance (f,1-size);
		P.split_facet (h->next (),h->next ()->next ()->next());

		for(int i = 1; i < size; ++i)
		{
			s = f->facet_begin();
			P.split_facet (s->next (),s->next ()->next ()->next());
			std::advance (f,1);
		}
		P.erase_center_vertex (g);  
	}
};