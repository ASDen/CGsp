//this could have also been implemented by delegation
class Arc_2 : Primitives 
{
public:
	//The basic parameters in the Arc
	double radius;
	double to;
	bool pie_slice;

	//Set the default parameters in the Arc
	Arc_2():radius(25.0),to(90.0),pie_slice(false)
	{}

	//Set the parameters with user defined values
	Arc_2(double r,double t,bool p):radius(r),to(t),pie_slice(p)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;
		//Starting the Arc with triangle, with width and length
		Halfedge_handle h = P.make_triangle ( Point( radius, 0, 0),
											  Point( 0, radius, 0),
											  Point( 0, 0, 0) );
		//editing to point
		to = to / 180.0;
		//editing on the triangle to convert it to Arc
		//Put the basic three points in the right position
		//First Point
		double x = radius * cos ( to * ( 28 ) * ( CGAL_PI/28 ) );
		double y = radius * sin ( to * ( 28 ) * ( CGAL_PI/28 ) );
		h->vertex()->point()= Point ( x, y, 0 );
		//Second Point
		x = radius * cos ( to * ( 28-1 ) * ( CGAL_PI/28 ) );
		y = radius * sin ( to * ( 28-1 ) * ( CGAL_PI/28 ) );
		h->opposite()->vertex()->point()		= Point ( x, y, 0 );
		//Third Point
		x = radius * cos ( to * ( 28-2 ) * ( CGAL_PI/28 ) );
		y = radius * sin ( to * ( 28-2 ) * ( CGAL_PI/28 ) );
		h->opposite()->next()->vertex()->point()= Point ( x, y, 0 );

		//draw the side segments
		for ( int i=3 ; i<28 ; i++ )
		{
			//split some half edge and put it in the right position
			x = radius * cos ( to * ( 28-i ) * ( CGAL_PI/28 ) );
			y = radius * sin ( to * ( 28-i ) * ( CGAL_PI/28 ) );
			P.split_edge( h->next()->opposite() );

			h->next()->vertex()->point()= Point ( x, y, 0 );
		}
		if (pie_slice)
			h->next()->vertex()->point()= Point ( 0, 0, 0 );
		return P;
	}
};