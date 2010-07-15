// CGsp.cpp : Defines the entry point for the console application.

#include "stdafx.h"

//typedef CGAL::Simple_cartesian<float>                 Kernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                                Point_3;
typedef Kernel::Vector_3                               Vector_3;
typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
typedef CGAL::Polyhedron_3<Traits>                     Polyhedron;
typedef Polyhedron::HalfedgeDS						   HalfedgeDS;
typedef Polyhedron::Facet_iterator                     Facet_iterator;
typedef Polyhedron::Vertex_iterator                    Vertex_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator   Halfedge_facet_circulator;
typedef Polyhedron::Halfedge_handle                    Halfedge_handle;
typedef Polyhedron::Plane_const_iterator			   Plane_const_iterator;
typedef Kernel::Aff_transformation_3                   Aff3;

#include "Zre.h"

struct Normal_vector {
    template <class Facet>
    typename Facet::Plane_3 operator()( Facet& f) 
	{
		typename Facet::Halfedge_handle h = f.halfedge();
        // Facet::Plane_3 is the normal vector type. We assume the
        // CGAL Kernel here and use its global functions.
        return CGAL::cross_product( h->next()->vertex()->point() - h->vertex()->point(),
									h->next()->next()->vertex()->point() - h->next()->vertex()->point() );
    }
};

int main() {
	std::ofstream of("C:\\123.off");

	std::vector <Point_3> arr;
	arr.push_back(Point_3 (2,0,2));
	arr.push_back(Point_3 (6,0,3));
	arr.push_back(Point_3 (8,0,4));
	arr.push_back(Point_3 (3,0,5));
	arr.push_back(Point_3 (5,0,6));
	arr.push_back(Point_3 (8,0,7));

	arr.push_back(Point_3 (0,1.75,8));
	arr.push_back(Point_3 (0,1.50,8));
	arr.push_back(Point_3 (0,1.25,8));

	arr.push_back(Point_3 (0,1,8));
	arr.push_back(Point_3 (0,1,7));
	arr.push_back(Point_3 (0,1,6));
	arr.push_back(Point_3 (0,1,5));
	arr.push_back(Point_3 (0,1,4));
	arr.push_back(Point_3 (0,1,3));

	arr.push_back(Point_3 (0,1.25,3));
	arr.push_back(Point_3 (0,1.50,3));
	arr.push_back(Point_3 (0,1.75,3));

	
	Point_3* Center = new Point_3(0,0,0);

	//Arc_2 s(30,270,30,true);
	//Circle_2 s(4,20);
	//Ellipse_2 s(6,4,30);
	//Plane_3 s(30,30,10,10);
	//Rectangle_2 s(20,10);

	Box_3 s(20,20,20,15,15,15);
	//Capsule_3 s(30,200,10,10);
	//ChamferCyl_3 s(60,100,30,10,5,5,15);
	//Cone_3 s(2,5,10,3,3,3);
	//Cylinder_3 s(3,20,20,9,30);
	//Lathe_3 s(arr,Center,Z_ax,20,360);
	//Pyramid_3 s(100,200,200,4,4,4);
	//Sphere_3 s(20,50);
	//Spindle_3 s(10,30,20,10,5,15);
	//Spring_3 s(20,2.5,200,10,10,40);
	//Torus_3 s(20,5,0,0,30,40);
	//Tube_3 s(14,13,15,20,20,10);

	Polyhedron P;
	P = s.Draw();
	Traingulate trg;
	//P=s.Draw();
	//std::transform(P.facets_begin(), P.facets_end(), P.planes_begin(), Normal_vector());
	/*Eigen::Transform3d T;
	Eigen::Vector3d Original(0,0,10);
	T.setIdentity();
	T.pretranslate (-Original);
	trg.ApplyTransformToPolyhedron(P,T);*/


	Traingulate tr;
	//tr.Do(P);

	Bevel Be(400,-4,1.25);
	//Be.Do(P);

	Bridge Br(18,20);
	//Br.Do(P);

	Extrude Ex(45,15);
	Ex.Do(P);
	
	Outline Ou(45,1.5);
	//Ou.Do(P);


	Bend Ben(90,s.Center,Y_ax,false,20,-20);
	//Ben.Do(P);

	Bulge Bu(40,s.Center,Z_ax,BRadial,false,45,-45);
	//Bu.Do(P);

	Cylindrical_Wave CylWa(2,8,12,s.Center,Z_ax);
	//CylWa.Do(P);

	Linear_Wave LiWaX(1,10,0,s.Center,Z_ax,X_ax);
	//LiWaX.Do(P);
	Linear_Wave LiWaY(2,10,0,s.Center,Z_ax,Y_ax);
	//LiWaY.Do(P);

	/*ChamferCyl_3 n(1,50,5,10,10,10,40);
	Spindle_3 p(5,30,5,10,20,40);
	Polyhedron P;
	Polyhedron E;
	E = p.Draw();
	P = n.Draw();
	Morph Mor(E,50);
	Mor.Do(P);*/

	//Noise No(5,0.3,0,s.Center,Z_ax);
	//No.Do(P);

	Skew Sk(30,s.Center,Z_ax,false,20,-20);
	//Sk.Do(P);

	Smooth Sm(1);
	//Sm.Do(P);

	Spherify Sph(50);
	//Sph.Do(P);

	Squeeze Sq(-30,s.Center,Z_ax,false,10,0);
	//Sq.Do(P);

	Stretch St(-20,s.Center,Z_ax,true,50,-50);
	//St.Do(P);
	
	Taper Ta(3,s.Center,X_ax,false,20,-20);
	//Ta.Do(P);

	Twist Tw(270,s.Center,Z_ax,true,-5,15);
	//Tw.Do(P);

	// Write polyhedron in Object File Format (OFF).
	CGAL::set_ascii_mode( of );
	of << "OFF" << std::endl << P.size_of_vertices() << ' ' << P.size_of_facets() << " 0" << std::endl;
	std::copy( P.points_begin(), P.points_end(), std::ostream_iterator<Point_3>( of, "\n"));

	for (  Facet_iterator i = P.facets_begin(); i != P.facets_end(); ++i) 
	{
		Halfedge_facet_circulator j = i->facet_begin();
		// Facets in polyhedral surfaces are at least triangles.
		CGAL_assertion( CGAL::circulator_size(j) >= 3);
		of << CGAL::circulator_size(j) << ' ';
		
		do 
		{
			of << ' ' << std::distance(P.vertices_begin(), j->vertex());
		} while ( ++j != i->facet_begin());
		
		of << std::endl;
	}

	/* Write polyhedron in (OBJ).
	CGAL::set_ascii_mode( oof );
	oof << "# " << P.size_of_vertices() << ' ' << std::endl <<"# "<< P.size_of_facets() << std::endl;
	oof<<"v ";
	std::copy( P.points_begin(), P.points_end(), std::ostream_iterator<Point_3>( oof, "\nv "));
	oof<<"_ _ _\n";
	for (  Facet_iterator i = P.facets_begin(); i != P.facets_end(); ++i)
	{
		Halfedge_facet_circulator j = i->facet_begin();
		// Facets in polyhedral surfaces are at least triangles.
		//CGAL_assertion( CGAL::circulator_size(j) >= 3);
       
		oof << 'f' << ' ';
       
		do
		{
			oof << ' ' << std::distance(P.vertices_begin(), j->vertex())+1;
		} while ( ++j != i->facet_begin());
       
		oof << std::endl;
	}
	*/

	return 0;
}