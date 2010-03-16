//this could have also been implemented by delegation
class Ellipse_2: Primitives 
{
public:
	//The basic parameters in the Ellipse
	double width;
	double length;
	int seg;
	Point_3* Center;

	//Set the default parameters in the Ellipse
	Ellipse_2():width(35.0),length(25.0),seg(28)
	{}

	//Set the parameters with user defined values
	Ellipse_2(double w):width(w),length(w),seg(28)
	{}

	//Set the parameters with user defined values
	Ellipse_2(double w, int s):width(w),length(w),seg(s)
	{}

	//Set the parameters with user defined values
	Ellipse_2(double w, double l, int s):width(w),length(l),seg(s)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;

		min(width, 0);
		min(length, 0);
		maxmin(seg,4,200);

		//Starting the Ellipse with triangle, with width and length
		Halfedge_handle h = P.make_triangle ( Point( width, 0, 0),
											  Point( 0, length, 0),
											  Point( 0, 0, 0) );
		//editing on the triangle to convert it to Ellipse
		//Put the basic three points in the right position
		//First Point
		double x = width * cos ( 0.0 );
		double y = length * sin ( 0.0 );
		h->vertex()->point()= Point ( x, y, 0 );
		//Second Point
		x = width * cos ( 2 * ( 28-1 ) * ( CGAL_PI/28 ) );
		y = length * sin ( 2 * ( 28-1 ) * ( CGAL_PI/28 ) );
		h->opposite()->vertex()->point()		= Point ( x, y, 0 );
		//Third Point
		x = width * cos ( 2 * ( 28-2 ) * ( CGAL_PI/28 ) );
		y = length * sin ( 2 * ( 28-2 ) * ( CGAL_PI/28 ) );
		h->opposite()->next()->vertex()->point()= Point ( x, y, 0 );

		//draw the side segments
		for ( int i = 0 ; i < 4 ; i++ )
		{
			int j = 0;
			double w = width, l = length;
			if ( i == 0 )
			{
				j = 3; 
			}
			if ( i == 1 || i == 3 )
			{
				w = length;
				l = width;
			}
			for ( j ; j < 7 ; j++ )
			{
				//split some half edge and put it in the right position
				x = width * cos ( 2 * ( 28 -((i * 7) + j) ) * ( CGAL_PI/28 ) );
				y = length * sin ( 2 * ( 28 -((i * 7) + j) ) * ( CGAL_PI/28 ) );
				P.split_edge( h->next()->opposite() );

				h->next()->vertex()->point()= Point ( x, y, 0 );
			}
		}
		Center = new Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};