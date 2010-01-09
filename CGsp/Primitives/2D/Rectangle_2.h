//this could have also been implemented by delegation
class Rectangle_2 : Primitives 
{
public:
	//The basic parameters in the Rectangle
	double width;
	double length;

	//Set the default parameters in the Rectangle
	Rectangle_2():width(25.0),length(10.0)
	{}

	//Set the parameters with user defined values
	Rectangle_2(double w,double l):width(w),length(l)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;
		//Starting the Rectangle with triangle, with width and length
		Halfedge_handle h1 = P.make_triangle ( Point( width, 0, 0),
											  Point( 0, length, 0),
											  Point( 0, 0, 0) );
		//editing on the triangle to convert it to Rectangle
		Halfedge_handle h2 = h1->next();
		Halfedge_handle h3 = P.split_edge ( h2 );
		h3->vertex()->point() = Point ( width, length, 0 );
		return P;
	}
};