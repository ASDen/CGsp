//this could have also been implemented by delegation
class Arc_2 : Primitives 
{
public:
	//The basic parameters in the Arc
	double radius;
	double to;
	int seg;
	bool pie_slice;
	Point_3 Center;

	//Set the default parameters in the Arc
	Arc_2():radius(25.0),to(90.0),seg(28),pie_slice(false)
	{}

	//Set the parameters with user defined values
	Arc_2(double r):radius(r),to(90.0),seg(28),pie_slice(false)
	{}

	//Set the parameters with user defined values
	Arc_2(double r,double t,int s):radius(r),to(t),seg(s),pie_slice(false)
	{}

	//Set the parameters with user defined values
	Arc_2(double r,double t,int s,bool p):radius(r),to(t),seg(s),pie_slice(p)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;

		min(radius, 0);
		min(to, 0);
		maxmin(seg, 4, 200);

		//Starting the Arc with triangle, with width and length
		Halfedge_handle h = P.make_triangle ( Point( radius, 0, 0),
											  Point( 0, radius, 0),
											  Point( 0, 0, 0) );
		//editing to point
		to = to / 180.0;

		//editing on the triangle to convert it to Arc
		//Put the basic three points in the right position
		//First Point
		double x = radius * cos ( to * ( seg ) * ( CGAL_PI/seg ) );
		double y = radius * sin ( to * ( seg ) * ( CGAL_PI/seg ) );
		h->vertex()->point()= Point ( x, y, 0 );
		//Second Point
		x = radius * cos ( to * ( seg-1 ) * ( CGAL_PI/seg ) );
		y = radius * sin ( to * ( seg-1 ) * ( CGAL_PI/seg ) );
		h->opposite()->vertex()->point()		= Point ( x, y, 0 );
		//Third Point
		x = radius * cos ( to * ( seg-2 ) * ( CGAL_PI/seg ) );
		y = radius * sin ( to * ( seg-2 ) * ( CGAL_PI/seg ) );
		h->opposite()->next()->vertex()->point()= Point ( x, y, 0 );

		//draw the side segments
		for ( int i = 3 ; i < seg ; i++ )
		{
			//split some half edge and put it in the right position
			x = radius * cos ( to * ( seg-i ) * ( CGAL_PI/seg ) );
			y = radius * sin ( to * ( seg-i ) * ( CGAL_PI/seg ) );
			P.split_edge( h->next()->opposite() );

			h->next()->vertex()->point()= Point ( x, y, 0 );
		}
		if (pie_slice)
			h->next()->vertex()->point()= Point ( 0, 0, 0 );

		Center = Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};