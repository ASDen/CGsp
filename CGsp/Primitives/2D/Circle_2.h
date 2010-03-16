//this could have also been implemented by delegation
class Circle_2 : Primitives 
{
public:
	//The basic parameters in the Circle
	double radius;
	int num;
	Point_3* Center;

	//Set the default parameters in the Circle
	Circle_2():radius(25.0),num(6)
	{}

	//Set the parameters with user defined values
	Circle_2(double r,int n):radius(r),num(n)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;

		min(radius, 0);
		maxmin(num, 4 ,200);

		//Starting the Circle with triangle, with width and length
		Halfedge_handle h = P.make_triangle ( Point( radius, 0, 0),
											  Point( 0, radius, 0),
											  Point( 0, 0, 0) );

		//editing on the triangle to convert it to Circle
		//Put the basic three points in the right position
		//First Point
		double x = radius * cos ( 0.0 );
		double y = radius * sin ( 0.0 );
		h->vertex()->point()= Point ( x, y, 0 );
		//Second Point
		x = radius * cos ( 2 * ( num-1 ) * ( CGAL_PI/num ) );
		y = radius * sin ( 2 * ( num-1 ) * ( CGAL_PI/num ) );
		h->opposite()->vertex()->point()		= Point ( x, y, 0 );
		//Third Point
		x = radius * cos ( 2 * ( num-2 ) * ( CGAL_PI/num ) );
		y = radius * sin ( 2 * ( num-2 ) * ( CGAL_PI/num ) );
		h->opposite()->next()->vertex()->point()= Point ( x, y, 0 );

		//draw the side segments
		for ( int i=3 ; i<num ; i++ )
		{
			//split some half edge and put it in the right position
			x = radius * cos ( 2 * ( num-i ) * ( CGAL_PI/num ) );
			y = radius * sin ( 2 * ( num-i ) * ( CGAL_PI/num ) );
			P.split_edge( h->next()->opposite() );

			h->next()->vertex()->point()= Point ( x, y, 0 );
		}
		Center = new Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};