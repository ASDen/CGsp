//this could have also been implemented by delegation
class Box_3 : public Primitives
{
public:
    //The basic parameters in the BOX
    double length;
    double width;
    double height;
    int length_Seg;
    int width_Seg;
    int height_Seg;

    //Set the default parameters in the BOX
    Box_3():length(25.0),width(25.0),height(25.0),length_Seg(1),width_Seg(1),height_Seg(1)
    {}

	//Set the parameters with user defined values
    Box_3(double l):length(l),width(l),height(l),length_Seg(1),width_Seg(1),height_Seg(1)
    {}

	//Set the parameters with user defined values
    Box_3(double l,int x):length(l),width(l),height(l),length_Seg(x),width_Seg(x),height_Seg(x)
    {}

	//Set the parameters with user defined values
    Box_3(double l,double w,double h):length(l),width(w),height(h),length_Seg(1),width_Seg(1),height_Seg(1)
    {}

    //Set the parameters with user defined values
    Box_3(double l,double w,double h,int x):length(l),width(w),height(h),length_Seg(x),width_Seg(x),height_Seg(x)
    {}

	//Set the parameters with user defined values
    Box_3(double l,double w,double h,int x,int y):length(l),width(w),height(h),length_Seg(x),width_Seg(y),height_Seg(y)
    {}

    //Set the parameters with user defined values
    Box_3(double l,double w,double h,int x,int y,int z):length(l),width(w),height(h),length_Seg(x),width_Seg(y),height_Seg(z)
    {}

    Polyhedron Draw()
    {
        Polyhedron P;
        typedef Polyhedron::Point_3         Point;
        typedef Polyhedron::Halfedge_handle Halfedge_handle;

		min(length,0);
		min(width,0);
		min(height,0);
		maxmin(length_Seg,0,200);
		maxmin(width_Seg,0,200);
		maxmin(height_Seg,0,200);

        Halfedge_handle a, b, c, d;
        //Starting the BOX with tetrahedron, with width, length and height
        Halfedge_handle h = P.make_tetrahedron( Point(  width/2, -length/2, -height/2),
                                                Point( -width/2, -length/2,  height/2),
                                                Point( -width/2, -length/2, -height/2),
                                                Point( -width/2,  length/2, -height/2) );

        //editing on the tetrahedron to convert it to BOX
        Halfedge_handle g = h->next()->opposite()->next();            
        P.split_edge( h->next());
        P.split_edge( g->next());
        P.split_edge( g);                                             
        h->next()->vertex()->point()     = Point(  width/2, -length/2, height/2);
        g->next()->vertex()->point()     = Point( -width/2,  length/2, height/2);
        g->opposite()->vertex()->point() = Point(  width/2,  length/2, -height/2);           
        Halfedge_handle f = P.split_facet( g->next(),           
            g->next()->next()->next());
        Halfedge_handle e = P.split_edge( f);
        e->vertex()->point() = Point( width/2, length/2, height/2);
        P.split_facet( e, f->next()->next());
        Halfedge_handle o = e->opposite()->next();
        Halfedge_handle q = h->next()->next()->next();

        //arrays will be used in storing the lists of half wdges
        Halfedge_handle* arr_wU = new Halfedge_handle[width_Seg];
        Halfedge_handle* arr_wD = new Halfedge_handle[width_Seg];
        Halfedge_handle* arr_wF = new Halfedge_handle[width_Seg];
        Halfedge_handle* arr_wB = new Halfedge_handle[width_Seg];
        Halfedge_handle* arr_lL = new Halfedge_handle[length_Seg];
        Halfedge_handle* arr_lR = new Halfedge_handle[length_Seg];

        //Segments in any of the three faces must be more than 1 Segment
        if (width_Seg>1 || length_Seg>1 || height_Seg>1)
        {
            double w = width / width_Seg, l = length / length_Seg, ht = height / height_Seg;
            //Every time we must reset them to not ovelap
            Halfedge_handle a = h ,b = g->opposite() ,c = a->next()->next()->opposite(), d = e;
            //Loop on the segments of the width
            for (int i = 2 ; i <= width_Seg ; i++)
            {
                //Calculating the distance which the point will be put on
                double n = (i-1)*w;
                //Put the points
                Halfedge_handle aa = P.split_edge(a);
                Halfedge_handle bb = P.split_edge(b);
                Halfedge_handle cc = P.split_edge(c);
                Halfedge_handle dd = P.split_edge(d);
                //Moving points according to the distance n, so they will be divided normally
                aa->vertex()->point() = Point(n - width/2, -length/2, -height/2);
                bb->vertex()->point() = Point(n - width/2,  length/2, -height/2);
                cc->vertex()->point() = Point(n - width/2, -length/2,  height/2);
                dd->vertex()->point() = Point(n - width/2,  length/2,  height/2);
                //Connect between Points
                arr_wD[i-2] = P.split_facet( a->opposite(), aa->opposite()->next()->next() );
                arr_wB[i-2] = P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                arr_wF[i-2] = P.split_facet( c->opposite(), cc->opposite()->next()->next() );
                arr_wU[i-2] = P.split_facet( d->opposite(), dd->opposite()->next()->next() );
            }
            //Every time we must reset them to not ovelap
            a = o->opposite();
            b = a->next()->next()->opposite();
            c = f->opposite();
            d = f->next()->next();
            //Loop on the segments of the length
            for (int i = 2 ; i <= length_Seg ; i++)
            {
                //Calculating the distance which the point will be put on
                double n = (i - 1) * l;
                //Put the points
                Halfedge_handle aa = P.split_edge(a);
                Halfedge_handle bb = P.split_edge(b);
                Halfedge_handle cc = P.split_edge(c);
                Halfedge_handle dd = P.split_edge(d);
                //Moving points according to the distance n, so they will be divided normally
                bb->vertex()->point() = Point(-width/2, n - length/2, -height/2);
                aa->vertex()->point() = Point(-width/2, n - length/2,  height/2);
                dd->vertex()->point() = Point( width/2, n - length/2, -height/2);
                cc->vertex()->point() = Point( width/2, n - length/2,  height/2);
                //Connect between Points
                arr_lL[i-2] = P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                arr_lR[i-2] = P.split_facet( c->opposite(), cc->opposite()->next()->next() );
                if ( width_Seg >= 2 )
                {
                    double wid;
                    //loop on the width up array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wU[ j-1 ]->opposite();
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, n - length/2, height/2 );
                        P.split_facet ( xx, x->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( cc, c->next()->next() );
                    //loop on the width down array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wD[ j-1 ];
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, n - length/2, -height/2 );
                        P.split_facet ( x->opposite(), xx->opposite()->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( d->opposite(), dd->opposite()->next()->next() );
                }
                else
                {
                    P.split_facet( a->opposite(), aa->opposite()->next()->next() );
                    P.split_facet( d->opposite(), dd->opposite()->next()->next() );
                }
            }
            //Every time we must reset them to not ovelap
            a = q->opposite();
            b = o->opposite()->next()->opposite();
            c = h->next();
            d = e->next()->opposite();
            //Loop on the segments of the height
            for (int i = 2 ; i <= height_Seg ; i++)
            {
                //Calculating the distance which the point will be put on
                double n = (i - 1) * ht;
                //Put the points
                Halfedge_handle aa = P.split_edge(a);
                Halfedge_handle bb = P.split_edge(b);
                Halfedge_handle cc = P.split_edge(c);
                Halfedge_handle dd = P.split_edge(d);
                //Moving points according to the distance n, so they will be divided normally
                cc->vertex()->point() = Point( width/2, -length/2, n - height/2);
                bb->vertex()->point() = Point(-width/2,  length/2, n - height/2);
                aa->vertex()->point() = Point(-width/2, -length/2, n - height/2);
                dd->vertex()->point() = Point( width/2,  length/2, n - height/2);
                //if the width & length segments more than 2
                if ( width_Seg >= 2 && length_Seg >= 2 )
                {
                    double wid, len;
                    //loop on the width up array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wF[ j-1 ]->opposite();
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, -length/2, n - height/2 );
                        P.split_facet ( xx, x->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( cc, c->next()->next() );
                    //loop on the width down array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wB[ j-1 ];
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, length/2, n - height/2 );
                        P.split_facet ( x->opposite(), xx->opposite()->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( d->opposite(), dd->opposite()->next()->next() );

                    //loop on the length left array and cut halfedges with the length segment
                    for ( int j = 1 ; j < length_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_lL[ j-1 ];
                        Halfedge_handle xx = P.split_edge( x );
                        len = j * l;
                        xx->vertex()->point() = Point ( -width/2, len - length/2, n - height/2 );
                        P.split_facet ( x->opposite(), xx->opposite()->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                    //loop on the length right array and cut halfedges with the length segment
                    for ( int j = 1 ; j < length_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_lR[ j-1 ]->opposite();
                        Halfedge_handle xx = P.split_edge( x );
                        len = j * l;
                        xx->vertex()->point() = Point ( width/2, len - length/2, n - height/2 );
                        P.split_facet ( xx, x->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( dd, d->next()->next() );
                }
                else if ( width_Seg >= 2 && length_Seg < 2 )
                {
                    double wid;
                    //loop on the width up array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wF[ j-1 ]->opposite();
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, -length/2, n - height/2 );
                        P.split_facet ( xx, x->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( cc, c->next()->next() );
                    //loop on the width down array and cut halfedges with the length segment
                    for ( int j = 1 ; j < width_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_wB[ j-1 ];
                        Halfedge_handle xx = P.split_edge( x );
                        wid = j * w;
                        xx->vertex()->point() = Point ( wid - width/2, length/2, n - height/2 );
                        P.split_facet ( x->opposite(), xx->opposite()->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( d->opposite(), dd->opposite()->next()->next() );
                    //Connect between Points of the Length
                    P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                    P.split_facet( c->opposite(), cc->opposite()->next()->next() );
                }
                else if ( width_Seg < 2 && length_Seg >= 2 )
                {
                    double len;
                    //loop on the length left array and cut halfedges with the length segment
                    for ( int j = 1 ; j < length_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_lL[ j-1 ];
                        Halfedge_handle xx = P.split_edge( x );
                        len = j * l;
                        xx->vertex()->point() = Point ( -width/2, len - length/2, n - height/2 );
                        P.split_facet ( x->opposite(), xx->opposite()->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                    //loop on the length right array and cut halfedges with the length segment
                    for ( int j = 1 ; j < length_Seg ; j++ )
                    {
                        Halfedge_handle x = arr_lR[ j-1 ]->opposite();
                        Halfedge_handle xx = P.split_edge( x );
                        len = j * l;
                        xx->vertex()->point() = Point ( width/2, len - length/2, n - height/2 );
                        P.split_facet ( xx, x->next()->next() );
                    }
                    //connect between the last two points
                    P.split_facet( dd, d->next()->next() );
                    //Connect between Points of the Width
                    P.split_facet( a->opposite(), aa->opposite()->next()->next() );
                    P.split_facet( d->opposite(), dd->opposite()->next()->next() );
                }
                else
                {   
                //Connect between Points
                P.split_facet( a->opposite(), aa->opposite()->next()->next() );
                P.split_facet( b->opposite(), bb->opposite()->next()->next() );
                P.split_facet( c->opposite(), cc->opposite()->next()->next() );
                P.split_facet( d->opposite(), dd->opposite()->next()->next() );
                }
            }
        }
		Center = new Point_3(0, 0, 0);

		setMesh(P);
        return P;
    }
};