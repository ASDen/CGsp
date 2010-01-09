// CGsp.cpp : Defines the entry point for the console application.

#include "stdafx.h"

typedef CGAL::Simple_cartesian<double>                 Kernel;
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
	//
	Outline oA(15,1.0);
	FrameCreater::FillFrames(0,4,0.1,0.1,&Outline::OutlineAmount,oA);
	FrameCreater::FillFrames(5,7,0.4,0.4,&Outline::OutlineAmount,oA);
	FrameCreater::FillFrames(8,12,0.7,0.7,&Outline::OutlineAmount,oA);

	//FrameCreater::FillFrames(0,5,2,2,&Outline::Fnum,oA);
	//FrameCreater::FillFrames(6,7,5,5,&Outline::Fnum,oA);
	//
	Box_3 ms(2,4,8,4,4,4);
	Polyhedron mP= ms.Draw();


	oA.DoAtFrame(mP,8);

	std::cout<<oA.OutlineAmount.val<<" "<<oA.Fnum.val<<std::endl;
	std::ostream_iterator< double > output( std::cout, " " );
	std::copy( oA.OutlineAmount.FrameValues.begin(), oA.OutlineAmount.FrameValues.end(), output );
	/*
	char bufferF[3];
	itoa (i,bufferF,10);
	std::string strF(bufferF);
	strF = std::string("D:\\") + strF + std::string(".off");
	std::ofstream of(strF.c_str());

	char bufferJ[3];
	itoa (i,bufferJ,10);
	std::string strJ(bufferJ);
	strJ = std::string("D:\\") + strJ + std::string(".obj");
	std::ofstream oof(strJ.c_str());
	*/

	/*
	std::vector <Point_3> arr;
	arr.push_back(Point_3 (0,2,3));
	arr.push_back(Point_3 (0,2,4));
	arr.push_back(Point_3 (0,2,5));
	arr.push_back(Point_3 (0,2,6));
	arr.push_back(Point_3 (0,2,7));
	arr.push_back(Point_3 (0,2,8));

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
	*/

	Point_3 Center = Point_3(50,50,100);

	//Circle_2 s(4,20);
	//Arc_2 s(6,270,true);
	//Ellipse_2 s(6,4);
	//Plane_3 s(2,3,1,3);
	//Rectangle_2 s(20,10);

	//Box_3 s(2,4,8,5,20,5);
	Box_3 s(2,4,8,4,4,4);
	//Capsule_3 s(30,200,10,10);
	//ChamferCyl_3 s(30,60,10,10,5,5,15);
	//Cone_3 s(2,5,10,11,2,3);
	//Cylinder_3 s(3,20,20,9,30);
	//Lathe_3 s(arr,Center,Z_ax,20,360);
	//Pyramid_3 s(100,200,200,5,50,5);
	//Sphere_3 s(20,50);
	//Spindle_3 s(10,30,20,10,5,15);
	//Spring_3 s(20,2.5,200,10,10,40);
	//Torus_3 s(20,5,0,0,10,20);
	//Tube_3 s(4,3,5,4,5,20);

	
	Polyhedron P;
	P = s.Draw();

	std::transform(P.facets_begin(), P.facets_end(), P.planes_begin(), Normal_vector());

	Bevel B(18,1.25,1.25,Polygon);
	//B.Do(P);

	Extrude E(18,1.25,Polygon);
	//E.Do(P);
	
	Outline O(5,2.25);
	O.Do(P);

	Bridge Br(18,20);
	//Br.Do(P);

	Twist Tw(230,Center,Z_ax,40,-40);
	//Tw.Do(P);

	Taper Ta(1,Center,X_ax,20,-20);
	//Ta.Do(P);

	Bend Be(90,Center,Z_ax,30,-30);
	//Be.Do(P);

	Skew S(30,Center,Y_ax,20,-20);
	//S.Do(P);

	Stretch St(-1,Center,Z_ax);
	//St.Do(P);
	
	////
	s.ApplyModifier(&Br);
	s.ApplyModifier(&Ta);

	////

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