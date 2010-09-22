//#include "Polyhedron_ex.h"
#include "Mesh_cutter.h"
#include "Parameterization_polyhedron_adaptor_ex.h"

enum TexType
{
	Tex_CGAL_General,
	Tex_Sphere,
	Tex_Cylinder,
	Tex_Torus,
	Tex_Tube,
	Tex_Box
};


// Mesh adaptors
typedef Parameterization_polyhedron_adaptor_ex              Parameterization_polyhedron_adaptor;
typedef Primitives* pPrimitive;
typedef CGAL::Parameterization_mesh_patch_3<Parameterization_polyhedron_adaptor>
Mesh_patch_polyhedron;

// Type describing a border or seam as a vertex list
typedef std::list<Parameterization_polyhedron_adaptor::Vertex_handle>
Seam;
class Texture {
public:
	static Seam cut_mesh(Parameterization_polyhedron_adaptor& mesh_adaptor)
	{

		// Helper class to compute genus or extract borders
		typedef CGAL::Parameterization_mesh_feature_extractor<Parameterization_polyhedron_adaptor_ex>
			Mesh_feature_extractor;
		typedef Mesh_feature_extractor::Border  Border;
		typedef Mesh_cutter::Backbone           Backbone;

		Seam seam;              // returned list

		// Get refererence to Polyhedron_3 mesh
		Polyhedron& mesh = mesh_adaptor.get_adapted_mesh();

		// Extract mesh borders and compute genus
		Mesh_feature_extractor feature_extractor(mesh_adaptor);
		int nb_borders = feature_extractor.get_nb_borders();
		int genus = feature_extractor.get_genus();

		// If mesh is a topological disk
		if (genus == 0 && nb_borders > 0)
		{
			// Pick the longest border
			seam = feature_extractor.get_longest_border();
		}
		else // if mesh is *not* a topological disk, create a virtual cut
		{
			Backbone seamingBackbone;           // result of cutting
			Backbone::iterator he;

			// Compute a cutting path that makes the mesh a "virtual" topological disk
			mesh.compute_facet_centers();
			Mesh_cutter cutter(mesh);
			if (genus == 0)
			{
				// no border, we need to cut the mesh
				assert (nb_borders == 0);
				cutter.cut(seamingBackbone);    // simple cut
			}
			else // genus > 0 -> cut the mesh
			{
				cutter.cut_genus(seamingBackbone);
			}

			// The Mesh_cutter class is quite buggy
			// => we check that seamingBackbone is valid
			//
			// 1) Check that seamingBackbone is not empty
			if (seamingBackbone.begin() == seamingBackbone.end())
				return seam;                    // return empty list
			//
			// 2) Check that seamingBackbone is a loop and
			//    count occurences of seam halfedges
			mesh.tag_halfedges(0);              // Reset counters
			for (he = seamingBackbone.begin(); he != seamingBackbone.end(); he++)
			{
				// Get next halfedge iterator (looping)
				Backbone::iterator next_he = he;
				next_he++;
				if (next_he == seamingBackbone.end())
					next_he = seamingBackbone.begin();

				// Check that seamingBackbone is a loop: check that
				// end of current HE == start of next one
				if ((*he)->vertex() != (*next_he)->opposite()->vertex())
					return seam;                // return empty list

				// Increment counter (in "tag" field) of seam halfedges
				(*he)->tag( (*he)->tag()+1 );
			}
			//
			// 3) check that the seamingBackbone is a two-way list
			for (he = seamingBackbone.begin(); he != seamingBackbone.end(); he++)
			{
				// Counter of halfedge and opposite halfedge must be 1
				if ((*he)->tag() != 1 || (*he)->opposite()->tag() != 1)
					return seam;                // return empty list
			}

			// Convert list of halfedges to a list of vertices
			for (he = seamingBackbone.begin(); he != seamingBackbone.end(); he++)
				seam.push_back((*he)->vertex());
		}

		return seam;
	}

	// Call appropriate parameterization method based on command line parameters
	template<
		class ParameterizationMesh_3,   // 3D surface
		class GeneralSparseLinearAlgebraTraits_d,
			// Traits class to solve a general sparse linear system
		class SymmetricSparseLinearAlgebraTraits_d
			// Traits class to solve a symmetric sparse linear system
	>
	typename CGAL::Parameterizer_traits_3<ParameterizationMesh_3>::Error_code
	static parameterize(ParameterizationMesh_3& mesh,  // Mesh parameterization adaptor
	const std::string& type,              // type of parameterization (see usage)
	const std::string& border)            // type of border parameterization (see usage)
		{
			typename CGAL::Parameterizer_traits_3<ParameterizationMesh_3>::Error_code err;

			if ( (type == std::string("floater"))  && (border == std::string("circle")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Mean_value_coordinates_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Circular_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("floater")) && (border == std::string("square")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Mean_value_coordinates_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Square_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("barycentric")) && (border == std::string("circle")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Barycentric_mapping_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Circular_border_uniform_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("barycentric")) && (border == std::string("square")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Barycentric_mapping_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Square_border_uniform_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("conformal")) && (border == std::string("circle")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Discrete_conformal_map_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Circular_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("conformal")) && (border == std::string("square")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Discrete_conformal_map_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Square_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("authalic")) && (border == std::string("circle")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Discrete_authalic_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Circular_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("authalic")) && (border == std::string("square")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::Discrete_authalic_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Square_border_arc_length_parameterizer_3<ParameterizationMesh_3>,
					GeneralSparseLinearAlgebraTraits_d
					>());
			}
			else if ( (type == std::string("lscm")) && (border == std::string("2pts")) )
			{
				err = CGAL::parameterize(
					mesh,
					CGAL::LSCM_parameterizer_3<
					ParameterizationMesh_3,
					CGAL::Two_vertices_parameterizer_3<ParameterizationMesh_3>,
					SymmetricSparseLinearAlgebraTraits_d
					>());
			}
			else
			{
				std::cerr << "Error: invalid parameters combination " << type << " + " << border << std::endl;
				err = CGAL::Parameterizer_traits_3<ParameterizationMesh_3>::ERROR_WRONG_PARAMETER;
			}

			return err;
		}



		template<TexType T>
		static int CalcUV(Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale);

		template<>
		static int CalcUV<Tex_Sphere> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			Facet_iterator f;
			for(f = mesh.facets_begin(); f != mesh.facets_end(); f++)
			{
				Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
				Halfedge_facet_circulator he = f->facet_begin();
				Halfedge_facet_circulator end = he;
				CGAL_For_all(he,end)
				{
					const Point_3& p = he->vertex()->point();
					Sphere_3* Obj = dynamic_cast<Sphere_3*>(P);

					double v = acos(p.z()/Obj->radius)/M_PI;
					double u = acos(p.y()/(Obj->radius*sin(v*M_PI))) / (2 * M_PI);

					he->uv(u, v);
				}
			}

			return 1;
		}

		template<>
		static int CalcUV<Tex_Box> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			Box_3* Obj0  = dynamic_cast<Box_3*>(P);
			Pyramid_3* Obj1	  = dynamic_cast<Pyramid_3*>(P);

			double width, length, height;

			if (val == 1)
			{
				width = Obj1->width;
				length = Obj1->depth;
				height = Obj1->height;
			}

			else
			{
				width = Obj0->width;
				length = Obj0->length;
				height = Obj0->height;
			}

			Traingulate trg;
			Eigen::Transform3d T;
			Eigen::Vector3d Original(width/2,length/2,height/2);
			T.setIdentity();
			T.pretranslate (Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			Facet_iterator f;
			for(f = mesh.facets_begin(); f != mesh.facets_end(); f++)
			{
				Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
				Halfedge_facet_circulator he = f->facet_begin();
				Halfedge_facet_circulator end = he;
				CGAL_For_all(he,end)
				{
					const Point_3& p = he->vertex()->point();

					int x[3] = {1,0,0};
					int y[3] = {0,1,0};
					int z[3] = {0,0,1};


					if((n.x() * x[0] + n.y() * x[1] + n.z() * x[2]) != 0)
						he->uv(p.y() * HorScale, p.z() * HorScale);
                           
					else if((n.x() * y[0] + n.y() * y[1] + n.z() * y[2]) != 0)
						he->uv(p.x() * HorScale, p.z() * HorScale);

					else if((n.x() * z[0] + n.y() * z[1] + n.z() * z[2]) != 0)
						he->uv(p.x() * HorScale, p.y() * HorScale);
					
				}
			}

			T.setIdentity();
			T.pretranslate (-Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			return 1;
		}

		template<>
		static int CalcUV<Tex_Torus> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			Facet_iterator f;
			for(f = mesh.facets_begin(); f != mesh.facets_end(); f++)
			{
				Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
				Halfedge_facet_circulator he = f->facet_begin();
				Halfedge_facet_circulator end = he;
				CGAL_For_all(he,end)
				{
					const Point_3& p = he->vertex()->point();
					Torus_3* Obj0 = dynamic_cast<Torus_3*>(P);
					Spring_3* Obj1 = dynamic_cast<Spring_3*>(P);

					// Determine its angle from the y-axis.

					double u = (1.0 - (atan2(p.z(), p.x()) + M_PI) / (2 * M_PI));

					double len = sqrt(p.x() * p.x() + p.z() * p.z());

					// Now rotate about the y-axis to get the point P into the x-z plane.
					double x = len - Obj0->radius1;
					double v = (atan2(p.y(), p.x()) + M_PI) / (2 * M_PI);

					he->uv(u, v);
				}
			}

			return 1;
		}

		template<>
		static int CalcUV<Tex_Cylinder> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			Cylinder_3* Obj0  = dynamic_cast<Cylinder_3*>(P);
			Capsule_3* Obj1	  = dynamic_cast<Capsule_3*>(P);
			ChamferCyl_3* Obj2 = dynamic_cast<ChamferCyl_3*>(P);
			Cone_3* Obj3	  = dynamic_cast<Cone_3*>(P);
			Spindle_3* Obj4	  = dynamic_cast<Spindle_3*>(P);
			
			double height;

			if (val == 1)
			{
				height = Obj1->height;
			}

			else if (val == 2)
			{
				height = Obj2->height;
			}

			else if (val == 3)
			{
				height = Obj3->height;
			}
			
			else if (val == 4)
			{
				height = Obj4->height;
			}

			else
			{
				height = Obj0->height;
			}

			Traingulate trg;
			Eigen::Transform3d T;
			Eigen::Vector3d Original(0,0,height/2);
			T.setIdentity();
			T.pretranslate (Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			Facet_iterator f;
			for(f = mesh.facets_begin(); f != mesh.facets_end(); f++)
			{
				Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
				Halfedge_facet_circulator he = f->facet_begin();
				Halfedge_facet_circulator end = he;
				CGAL_For_all(he,end)
				{
					const Point_3& p = he->vertex()->point();


					//v = p.z() / side_Seg * scale + 0.08
					//decreasing scale streaching the image "vertical" --number of checkboard squares decrease
					double v;

					//u = asin(p.x() / R) / (2*M_PI) + scale * 0.85
					//the most sutable value is 0.88
					double u;

					if (val == 1)
					{
						v = p.z() / Obj1->Segs * VerScale + .08;
						u = asin(p.x() / Obj1->radius) / (2 * M_PI) + HorScale * 0.85;
					}

					else if (val == 2)
					{
						v = p.z() / Obj2->side_Seg * VerScale + .08;
						u = asin(p.x() / Obj2->radius) / (2 * M_PI) + HorScale * 0.85;
					}

					else if (val == 3)
					{
						v = p.z() / Obj3->side_Seg * VerScale + .08;
						u = asin(p.x() / max(Obj3->radius1, Obj3->radius2)) / (2 * M_PI) + HorScale * 0.85;
					}
					
					else if (val == 4)
					{
						v = p.z() / Obj4->side_Seg * VerScale + .08;
						u = asin(p.x() / Obj4->radius) / (2 * M_PI) + HorScale * 0.85;
					}

					else
					{
						v = p.z() / Obj0->side_Seg * VerScale + .08;
						u = asin(p.x() / Obj0->radius) / (2 * M_PI) + HorScale * 0.85;
					}

					he->uv(u, v);
				}
			}
			
			T.setIdentity();
			T.pretranslate (-Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			return 1;
		}

		template<>
		static int CalcUV<Tex_Tube> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			Traingulate trg;
			Eigen::Transform3d T;
			Eigen::Vector3d Original(0,0,25);
			T.setIdentity();
			T.pretranslate (Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			Facet_iterator f;
			for(f = mesh.facets_begin(); f != mesh.facets_end(); f++)
			{
				Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
				Halfedge_facet_circulator he = f->facet_begin();
				Halfedge_facet_circulator end = he;
				CGAL_For_all(he,end)
				{
					const Point_3& p = he->vertex()->point();
					Tube_3* Obj = dynamic_cast<Tube_3*>(P);

					//v = p.z() / side_Seg * scale + 0.08
					//decreasing scale streaching the image "vertical" --number of checkboard squares decrease
					double v = p.z() / Obj->side_Seg * VerScale + 0.08;
					
					//u =  asin(p.x() / max(r1, r2)) / (2 * M_PI) + scale * 0.85
					//the most sutable value is 0.88
					double u = asin(p.x() / max(Obj->radius1, Obj->radius2)) / (2 * M_PI) + HorScale * 0.85;

					he->uv(u, v);
				}
			}

			T.setIdentity();
			T.pretranslate (-Original);
			trg.ApplyTransformToPolyhedron(mesh,T);

			return 1;
		}

		template<>
		static int CalcUV<Tex_CGAL_General> (Polyhedron& mesh, pPrimitive P, int val, float HorScale, float VerScale)
		{
			CGAL::Timer total_timer;
			total_timer.start();

			//std::cerr << "PARAMETERIZATION" << std::endl;

			//***************************************
			// Read options on the command line
			//***************************************

			std::string type;               // default: Floater param
			std::string border;             // default: circular border param.
			std::string solver;             // default: OpenNL solver
			std::string input;              // required
			std::string output;             // default: out.eps

			border = "square";
			type = "conformal";
			solver = "opennl";

			//***************************************
			// Read the mesh
			//***************************************

			CGAL::Timer task_timer;
			task_timer.start();

			task_timer.reset();

			//***************************************
			// Create mesh adaptor
			//***************************************

			// The Surface_mesh_parameterization package needs an adaptor to handle Polyhedron_ex meshes
			Parameterization_polyhedron_adaptor mesh_adaptor(mesh);

			// The parameterization methods support only meshes that
			// are topological disks => we need to compute a cutting path
			// that makes the mesh a "virtual" topological disk
			//
			// 1) Cut the mesh
			Seam seam = cut_mesh(mesh_adaptor);
			if (seam.empty())
			{
				std::cerr << "Input mesh not supported: the example cutting algorithm is too simple to cut this shape" << std::endl;
				return EXIT_FAILURE;
			}
			//
			// 2) Create adaptor that virtually "cuts" a patch in a Polyhedron_ex mesh
			Mesh_patch_polyhedron   mesh_patch(mesh_adaptor, seam.begin(), seam.end());
			if (!mesh_patch.is_valid())
			{
				std::cerr << "Input mesh not supported: non manifold shape or invalid cutting" << std::endl;
				return EXIT_FAILURE;
			}

			//std::cerr << "Mesh cutting: " << task_timer.time() << " seconds." << std::endl;
			task_timer.reset();

			//***************************************
			// switch parameterization
			//***************************************

			//std::cerr << "Parameterization..." << std::endl;

			// Defines the error codes
			typedef CGAL::Parameterizer_traits_3<Mesh_patch_polyhedron> Parameterizer;
			Parameterizer::Error_code err;

			err = parameterize<Mesh_patch_polyhedron,
				OpenNL::DefaultLinearSolverTraits<double>,
				OpenNL::SymmetricLinearSolverTraits<double>
			>(mesh_patch, type, border);


			// Report errors
			switch(err) {

			case Parameterizer::OK: // Success
				break;
			case Parameterizer::ERROR_EMPTY_MESH: // Input mesh not supported
			case Parameterizer::ERROR_NON_TRIANGULAR_MESH:
			case Parameterizer::ERROR_NO_TOPOLOGICAL_DISC:
			case Parameterizer::ERROR_BORDER_TOO_SHORT:
				std::cerr << "Input mesh not supported: " << Parameterizer::get_error_message(err) << std::endl;
				return EXIT_FAILURE;
				break;
			default: // Error
				std::cerr << "Error: " << Parameterizer::get_error_message(err) << std::endl;
				return EXIT_FAILURE;
				break;
			};

			//std::cerr << "Parameterization: " << task_timer.time() << " seconds." << std::endl;
			task_timer.reset();

			return EXIT_SUCCESS;
		}
};