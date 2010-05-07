//this could have also been implemented by delegation
class Plane_3 : public Primitives
{
public:
    //The basic parameters in the Plane
    double width;
    double length;
    int width_Seg;
    int length_Seg;
    Point_3* Center;

    //Set the default parameters in the plane
    Plane_3():width(25.0),length(25.0),width_Seg(4),length_Seg(4)
    {}

    //Set the parameters with user defined values
    Plane_3(double w):width(w),length(w),width_Seg(4),length_Seg(4)
    {}

    //Set the parameters with user defined values
    Plane_3(double w,int x):width(w),length(w),width_Seg(x),length_Seg(x)
    {}

    //Set the parameters with user defined values
    Plane_3(double w,double l,int x,int y):width(w),length(l),width_Seg(x),length_Seg(y)
    {}

    Polyhedron Draw()
    {
        Polyhedron P;
        typedef Polyhedron::Point_3         Point;
        typedef Polyhedron::Halfedge_handle Halfedge_handle;

        min(width, 0);
        min(length, 0);
        maxmin(width_Seg, 0, 200);
        maxmin(length_Seg, 0, 200);

        //Starting the plane with triangle, with width and length
        Halfedge_handle h1 = P.make_triangle ( Point( width/2,-length/2, 0),
                                               Point(-width/2, length/2, 0),
                                               Point(-width/2,-length/2, 0) );
        //editing on the triangle to convert it to plane
        Halfedge_handle h2 = h1->next();
        Halfedge_handle h3 = P.split_edge ( h2 );
        h3->vertex()->point() = Point ( width/2, length/2, 0 );
        Halfedge_handle h4 = h2->next();

        //array will be used to store the width segments
        Halfedge_handle* arr_w = new Halfedge_handle[width_Seg];

        //draw width segments
        if( width_Seg >= 2 )
        {
            double w = width / width_Seg ;
            h2 = h2->opposite();
            //loop on width segments and draw them
            for ( int i = 1 ; i < width_Seg ; i++ )
            {
                //split the two halfedges and put the points on the right position, then reach beteen them
                Halfedge_handle h11 = P.split_edge ( h1 );
                Halfedge_handle h22 = P.split_edge ( h2 );
                h11->vertex()->point() = Point ( w*i - width/2,-length/2, 0 );
                h22->vertex()->point() = Point ( w*i - width/2, length/2, 0 );
                arr_w[i-1] = P.split_facet ( h11, h1->next()->next() );
            }
        }
       
        //draw length segments if exists, using width segments array
        if( length_Seg >= 2 )
        {
            double l = length / length_Seg ;
            double w = width / width_Seg ;
            h4 = h4->opposite();
            //loop on length segments and draw them
            for ( int i = 1 ; i < length_Seg ; i++ )
            {
                //split the first two basic halfedges and put the points on the right position, then reach beteen them
                Halfedge_handle h33 = P.split_edge ( h3 );
                Halfedge_handle h44 = P.split_edge ( h4 );
                h33->vertex()->point() = Point ( width/2, l*i - length/2, 0 );
                h44->vertex()->point() = Point (-width/2, l*i - length/2, 0 );
                //using the array to loop between width segments and draw the length segment
                for ( int j = 1 ; j < width_Seg ; j++ )
                {
                    //split the halfedge in array and reach between it and the last halfedge
                    Halfedge_handle a = P.split_edge ( arr_w[j-1] );
                    a->vertex()->point() = Point ( w*j - width/2, l*i - length/2 ,0 );
                    P.split_facet ( a, arr_w[j-1]->next()->next());
                }
                //connect between the last two width segments
                P.split_facet ( h33, h3->next()->next());
            }
        }
        Center = new Point_3(0, 0, 0);

        setMesh(P);
        return P;
    }
};