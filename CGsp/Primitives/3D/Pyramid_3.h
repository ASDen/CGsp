//this could have also been implemented by delegation
class Pyramid_3 : Primitives 
{
public:
	//The basic parameters in the Pyramid
	double width;
	double depth;
	double height;
	int width_Seg;
	int depth_Seg;
	int height_Seg;

	//Set the default parameters in the Pyramid
	Pyramid_3():width(25.0),depth(25.0),height(25.0),width_Seg(1),depth_Seg(1),height_Seg(1)
	{}

	//Set the parameters with user defined values
	Pyramid_3(double w,double l,double h,int x,int y,int z):width(w),depth(l),height(h),width_Seg(x),depth_Seg(y),height_Seg(z)
	{}

	Polyhedron Draw()
	{
		Polyhedron P;
		typedef Polyhedron::Point_3         Point;
		typedef Polyhedron::Halfedge_handle Halfedge_handle;
		//Starting the Pyramid with tetrahedron, with width, length and height
		Halfedge_handle h1 = P.make_tetrahedron( Point( width, 0, 0),
												Point( 0, 0, height),
												Point( 0, 0, 0),
												Point( 0, depth, 0));
		//editing on the tetrahedron to convert it to 4-facet Pyramid
		Halfedge_handle g1 = h1->next()->opposite()->next();             
		P.split_edge( g1 );                                              
		g1->opposite()->vertex()->point() = Point( width, depth, 0);            
		Halfedge_handle f = P.split_facet( g1->next(), g1->next()->next()->next());
		h1->next()->vertex()->point() = Point ( width/2, depth/2, height ); 
		f = f->opposite();
		Halfedge_handle s = h1->next();
		Halfedge_handle g2 = h1->opposite()->next();
		Halfedge_handle h2 = g1->opposite()->next();

		Halfedge_handle a = P.split_edge( h1 );
		a->vertex()->point() = Point ( width/2, depth/2, 0 );
		Halfedge_handle b = h1;
		h1 = P.split_facet( s->next(), s->next()->next()->next() );
		Halfedge_handle c = P.split_facet( b->opposite(), a->opposite()->next() );
		P.split_facet( b->opposite(), c->next() );
		Halfedge_handle h = g2->opposite()->next();
		Halfedge_handle g = g1->next();

		//arrays will be used later to store the width and depth halfedges
		Halfedge_handle* arr_wF = new Halfedge_handle[width_Seg];
		Halfedge_handle* arr_wB = new Halfedge_handle[width_Seg];
		Halfedge_handle* arr_dL = new Halfedge_handle[depth_Seg];
		Halfedge_handle* arr_dR = new Halfedge_handle[depth_Seg];

		//draw the width segments
		if ( width_Seg >= 2)
		{
			g1 = g1->opposite();
			double n = width / width_Seg;
			//loop on width segments
			for ( int i = 1 ; i < width_Seg ; i++ )
			{
				//determine the two points, in front and back of the width segments and put them in the right position
				Halfedge_handle hh = P.split_edge( h1 );
				hh->vertex()->point() = Point ( n*i, 0, 0 );
				Halfedge_handle gg = P.split_edge( g1 );
				gg->vertex()->point() = Point ( n*i, depth, 0 );
				//connect the points to draw width segments, and store half edges in the array
				arr_wF[i-1] = P.split_facet( hh, h1->next());
				P.split_facet( h1->opposite(), hh->opposite()->next());
				arr_wB[i-1] = P.split_facet( g1->opposite(), gg->opposite()->next());
				P.split_facet( gg, g1->next());
			}
		}

		//draw the depth segments
		if ( depth_Seg >= 2)
		{
			h2 = h2->opposite();
			double n = depth / depth_Seg;
			//loop on depth segments
			for ( int i = 1 ; i < depth_Seg ; i++ )
			{
				//determine the two points, in left and right of the depth segments and put them in the right position
				Halfedge_handle hh = P.split_edge( h2 );
				hh->vertex()->point() = Point ( width, n*i, 0 );
				Halfedge_handle gg = P.split_edge( g2 );
				gg->vertex()->point() = Point ( 0, n*i, 0 );
				//connect the points to draw depth segments, and store half edges in the array
				arr_dR[i-1] = P.split_facet( hh, h2->next());
				P.split_facet( h2->opposite(), hh->opposite()->next());
				arr_dL[i-1] = P.split_facet( g2->opposite(), gg->opposite()->next());
				P.split_facet( gg, g2->next());
			}
		}

		//draw the height segments
		if ( height_Seg >= 2 )
		{
			//loop on height segments
			for ( int i = 1 ; i < height_Seg ; i++ )
			{
				double n = ( height / height_Seg ) * i;
				double w = width / width_Seg;
				//split the basic two half edges and put the points in the right position
				P.split_edge ( h );
				P.split_edge ( g );
				double wid = ( w * ( width_Seg / 2.0 ) / height_Seg ) * i;
				double dep = ( ( depth / 2 ) / height_Seg ) * i;
				h->opposite()->vertex()->point() = Point ( wid, dep, n );
				dep = depth - ( ( depth / 2 ) / height_Seg ) * i ;
				g->opposite()->vertex()->point() = Point ( wid, dep, n );
				//loop on width segments and draw the height segments
				for ( int j = 1 ; j < width_Seg ; j++ )
				{
					//split the half edges in the array, move the points and connect between them in the front and back faces
					Halfedge_handle a = P.split_edge ( arr_wF[j-1] );
					Halfedge_handle b = P.split_edge ( arr_wB[j-1] );
					wid = ( j * w ) + ( w * ( ( width_Seg / 2.0 ) - j ) / height_Seg ) *i;
					dep = ( ( depth / 2 ) / height_Seg ) * ( i );
					a->vertex()->point() = Point ( wid, dep, n );
					P.split_facet ( a, a->next()->next());
					dep = depth - ( ( depth / 2 ) / height_Seg ) * ( i );
					b->vertex()->point() = Point ( wid, dep, n );
					P.split_facet ( arr_wB[j-1]->opposite(), b->opposite()->next()->next());
				}
				//To attach the last two points in the two faces
				Halfedge_handle a = P.split_edge ( s );
				Halfedge_handle b = P.split_edge ( f );
				wid = ( width_Seg * w ) + ( w * ( ( width_Seg / 2.0 ) - width_Seg ) / height_Seg ) *i;
				dep = ( ( depth / 2 ) / height_Seg ) * ( i );
				a->vertex()->point() = Point ( wid, dep, n );
				P.split_facet ( a, a->next()->next());
				dep = depth - ( ( depth / 2 ) / height_Seg ) * ( i );
				b->vertex()->point() = Point ( wid, dep, n );
				P.split_facet ( f->opposite(), b->opposite()->next()->next());
				
				double d = depth / depth_Seg;
				//loop on depth segments and draw the height segments
				for ( int j = 1 ; j < depth_Seg ; j++ )
				{
					
					//split the half edges in the array, move the points and connect between them in the left and right faces
					Halfedge_handle a = P.split_edge ( arr_dL[j-1] );
					Halfedge_handle b = P.split_edge ( arr_dR[j-1] );
					wid = ( ( width / 2 ) / height_Seg ) * ( i );
					dep = ( j * d ) + ( d * ( ( depth_Seg / 2.0 ) - j ) / height_Seg ) *i;
					a->vertex()->point() = Point ( wid, dep, n );
					P.split_facet ( arr_dL[j-1]->opposite(), a->opposite()->next()->next());
					wid = width - ( ( width / 2 ) / height_Seg ) * ( i );
					b->vertex()->point() = Point ( wid, dep, n );
					P.split_facet ( b, b->next()->next() );
				}
				//To attach the last two points in the two faces
				P.split_facet ( b, b->next()->next());
				P.split_facet ( g->opposite(), g->opposite()->next()->next()->next());
			}
		}
		return P;
	}
};