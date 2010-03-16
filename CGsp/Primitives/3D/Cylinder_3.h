//this could have also been implemented by delegation
class Cylinder_3 : public Primitives 
{
public:
	//Basic parametes of the cylinder
	double radius;
	double height;
	int height_Seg;
	int cap_Seg;
	int side_Seg;
	Point_3* Center;

	//Set the default parameters in the Cylinder
	Cylinder_3():radius(15.0),height(25.0),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Cylinder_3(double x):radius(x),height(x),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Cylinder_3(double r,double h):radius(r),height(h),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Cylinder_3(double r,int x):radius(r),height(r),height_Seg(x),cap_Seg(x),side_Seg(x)
	{}

	//Set the parameters with user defined values
	Cylinder_3(double r,double h,int x):radius(r),height(h),height_Seg(x),cap_Seg(x),side_Seg(x)
	{}

	//Set the parameters with user defined values
	Cylinder_3(double r,double h,int x,int y,int z):radius(r),height(h),height_Seg(x),cap_Seg(y),side_Seg(z)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;

		min(radius,0);
		min(height,0);
		maxmin(height_Seg,0,200);
		maxmin(cap_Seg,0,200);
		maxmin(side_Seg,0,200);

		//Starting the cylinder with tetrahedron, with radius1, radius2 and height
		Halfedge_handle h = P.make_tetrahedron( Point( radius, 0, 0 ),
												Point( 0, 0, height ),
												Point( 0, 0, 0 ),
												Point( 0, radius, 0 ) );
		//editing on the tetrahedron to convert it to 3_facet cylinder
		Halfedge_handle g = h->next()->opposite()->next();
		P.split_edge( h->next());
		P.split_edge( g->next());                    
		h->next()->vertex()->point()     = Point( radius, 0, height );
		g->next()->vertex()->point()     = Point( 0, radius, height );
		Halfedge_handle f = P.split_facet( g->next(), g->next()->next()->next() );

		//Arrays will be used later in storing some halfedges
		Halfedge_handle* arr_sideS = new Halfedge_handle[side_Seg];
		Halfedge_handle* arr_capSU  = new Halfedge_handle[side_Seg];
		Halfedge_handle* arr_capSD  = new Halfedge_handle[side_Seg];
		
		//Put the basic three points in the right position
		//First Point
		double x = radius * cos ( 0.0 );
		double y = radius * sin ( 0.0 );
		h->vertex()->point()= Point ( x, y, 0 );
		f->vertex()->point()= Point ( x, y, height );
		arr_sideS[0] = h->next();
		//Second Point
		x = radius * cos ( 2 * ( side_Seg-1 ) * ( CGAL_PI/side_Seg ) );
		y = radius * sin ( 2 * ( side_Seg-1 ) * ( CGAL_PI/side_Seg ) );
		h->opposite()->vertex()->point()		= Point ( x, y, 0 );
		f->opposite()->next()->vertex()->point()= Point ( x, y, height );
		arr_sideS[1] = h->next()->next()->next()->opposite();
		//Third Point
		x = radius * cos ( 2 * ( side_Seg-2 ) * ( CGAL_PI/side_Seg ) );
		y = radius * sin ( 2 * ( side_Seg-2 ) * ( CGAL_PI/side_Seg ) );
		h->opposite()->next()->vertex()->point()= Point ( x, y, 0 );
		f->opposite()->vertex()->point()		= Point ( x, y, height );
		arr_sideS[2] = g->next();

		//If there are cap segments, they will be added
		//in the basic three points
		if ( cap_Seg >= 2 )
		{
			g = g->opposite();
			//put point in the center of the three facetcylinder 
			Halfedge_handle gg = P.split_edge( g );
			Halfedge_handle ff = P.split_edge( f );
			gg->vertex()->point() = Point ( 0, 0, 0 );
			ff->vertex()->point() = Point ( 0, 0, height );
			P.split_facet( gg->opposite()	, gg->opposite()->next()->next()->next()->next() );
			P.split_facet( f				, f->next()->next()->next() );
			arr_capSU[1] = P.split_facet( ff->next()->opposite()	, ff->opposite()->next() );
			arr_capSD[1] = P.split_facet( gg						, g->next() );
			//draw the cap segments
			arr_capSU[0] = ff->next();
			arr_capSD[0] = gg->next()->opposite()->next();
			arr_capSU[2] = ff->opposite();
			arr_capSD[2] = gg->opposite();
			f = ff->next()->next()->opposite();
			g = gg->opposite()->next()->opposite();
		}

		//draw the side segments if they are more than 3
		if ( side_Seg >= 4 )
		{
			g = g->opposite();
			for ( int i=4 ; i<=side_Seg ; i++ )
			{
				//split some half edge and put it in the right position
				x = radius * cos ( 2 * ( side_Seg-i+1 ) * ( CGAL_PI/side_Seg ) );
				y = radius * sin ( 2 * ( side_Seg-i+1 ) * ( CGAL_PI/side_Seg ) );
				Halfedge_handle gg = P.split_edge( g );

				Halfedge_handle e = P.split_edge( f );
				g->opposite()->vertex()->point()= Point ( x, y, 0 );
				e->vertex()->point()			= Point ( x, y, height );
				
				P.split_facet( e, f->next()->next() );
				arr_sideS[i-1] = f->next()->next()->next();
				//If there are cap segments, they will be added
				if ( cap_Seg >= 2 )
				{
					Halfedge_handle x = arr_sideS[i-1];
					Halfedge_handle y1 = P.split_facet( f->opposite(), f->opposite()->next()->next() );
					Halfedge_handle y2 = P.split_facet( gg, g->next() );
					arr_capSU[i-1] = y1->opposite();
					arr_capSD[i-1] = y2->opposite();
				}
			}
		}
		//draw the side segments if they are more than 3
		if ( height_Seg >= 2 )
		{
			//looping on the height segments
			for ( int i = 2 ; i <= height_Seg ; i++ )
			{
				P.split_edge( arr_sideS[0] );
				double n = ( i - 1 )*( height / height_Seg );
				arr_sideS[0]->opposite()->vertex()->point() = Point ( radius, 0, n);
				//loop on side segments and draw the height segment between all side segments
				for ( int j = 1 ; j < side_Seg ; j++ )
				{
					x = radius * cos ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					y = radius * sin ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					P.split_edge( arr_sideS[j] );
					arr_sideS[j]->opposite()->vertex()->point() = Point ( x, y, n);
					P.split_facet( arr_sideS[j]->opposite(), arr_sideS[j]->opposite()->next()->next()->next() );
				}
				//draw the height segment between the last two side segments
				P.split_facet( arr_sideS[0]->opposite(), arr_sideS[0]->opposite()->next()->next()->next() );
			}
		}

		//draw the cap segments
		if ( cap_Seg >= 2 )
		{
			//looping on the height segments
			for ( int i = 2 ; i <= cap_Seg ; i++)
			{
				//draw the down cap segments 
				double n = ( i - 1 )*( radius / cap_Seg );
				Halfedge_handle b = P.split_edge( arr_capSU[0] );
				//draw the first point
				x = n * cos ( 0.0 );
				y = n * sin ( 0.0 );
				arr_capSU[0]->opposite()->vertex()->point() = Point ( x, y, height);
				//loop on the points
				for ( int j = 1 ; j < side_Seg ; j++)
				{
					Halfedge_handle a = P.split_edge( arr_capSU[j] );
					x = n * cos ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					y = n * sin ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					arr_capSU[j]->opposite()->vertex()->point() = Point ( x, y, height);
					P.split_facet( a, arr_capSU[j]->next()->next() );
				}
				//draw the cap segment between the last two side segments
				P.split_facet( b, arr_capSU[0]->next()->next() );

				//draw the down cap segments 
				P.split_edge( arr_capSD[0] );
				//draw the first point
				x = n * cos ( 0.0 );
				y = n * sin ( 0.0 );
				arr_capSD[0]->opposite()->vertex()->point() = Point ( x, y, 0);
				//loop on the points
				for ( int j = 1 ; j < side_Seg ; j++)
				{
					P.split_edge( arr_capSD[j] );
					x = n * cos ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					y = n * sin ( 2 * ( side_Seg - j ) * ( CGAL_PI/side_Seg ) );
					arr_capSD[j]->opposite()->vertex()->point() = Point ( x, y, 0);
		
					if( i == 2 )//if it is in the second one it will be in a triangle, so need just two next()
						P.split_facet( arr_capSD[j]->opposite(), arr_capSD[j]->opposite()->next()->next() );
					else//else it wwill need 3 next()
						P.split_facet( arr_capSD[j]->opposite(), arr_capSD[j]->opposite()->next()->next()->next() );
				}
				//draw the cap segment between the last two side segments
				if( i == 2 )
					P.split_facet( arr_capSD[0]->opposite(), arr_capSD[0]->opposite()->next()->next() );
				else
					P.split_facet( arr_capSD[0]->opposite(), arr_capSD[0]->opposite()->next()->next()->next() );
			}
		}
		Center = new Point_3(0, 0, height/2);
		
		setMesh(P);
		return P;
	}
};