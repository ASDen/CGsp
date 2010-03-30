class Tube_3 : public Primitives
{
	template <class HDS>
	class Build_tube : public CGAL::Modifier_base<HDS> {
	public:
		int cap;//the cap segments
		int side;//the side segments
		int segm;//the height segments
		double h;//the height
		double r1;//the inner radius
		double r2;//the outter radius
		
		Build_tube(double rad1,double rad2,double height,int h_Seg,int c_Seg,int s_Seg):r1(rad1),r2(rad2),h(height),segm(h_Seg),side(s_Seg),cap(c_Seg)
		{}

		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			//if inner radius larger than outter radius, swap them
			if ( r1 > r2 )
			{
				double temp = r1;
				r1 = r2;
				r2 = temp;
			}
			double segmentRad = 2 * CGAL_PI / side;//the angle of each circle
			int numberOfSeparators = side;//number of side segments
			int numVertices = 2 * side * (segm + cap);//number of Vertices
			int numFaces = 2 * side * (segm + cap);//number of Faces

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the outter height segments
			for (int e = 0; e < segm; e++)
			{
				//get the radius of each circle
				double rad = r2;
				double z_t = e * h / segm - h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the upper cap segments
			for (int e = 0; e < cap; e++)
			{
				//get the radius of each circle
				double rad = r2 - e * ( r2 - r1) / cap;
				double z_t = h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the inner height segments
			for (int e = 0; e < segm; e++)
			{
				//get the radius of each circle
				double rad = r1;
				double z_t = (segm - e) * h / segm - h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the lower cap segments
			for (int e = 0; e < cap; e++)
			{
				//get the radius of each circle
				double rad = r1 + e * ( r2 - r1) / cap;
				double z_t = -h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}

			//adding vertices to faces
			//loop on height segments
			for (int e = 0; e < 2 * (segm + cap); e++)
			{
				//loop on side segments
				for (int i = 0; i < numberOfSeparators; i++)
				{
					B.begin_facet();//begin the facet
					//add four point of each facet
					//if we reach to tha last faces, connect them to the first ones
					if (e == 2 * (segm + cap) - 1)
					{
						B.add_vertex_to_facet((i + 1) % numberOfSeparators );
						B.add_vertex_to_facet( i );
					}
					else
					{
						B.add_vertex_to_facet( e * numberOfSeparators + (i + 1) % numberOfSeparators + numberOfSeparators );
						B.add_vertex_to_facet( e * numberOfSeparators + i + numberOfSeparators );
					}
					B.add_vertex_to_facet( e * numberOfSeparators + i );
					B.add_vertex_to_facet( e * numberOfSeparators +(i + 1) % numberOfSeparators );
					B.end_facet();//end the facet
				}
			}

			//close the builder
			B.end_surface();
		}
	};

public:
	//The basic parameters in the Tube
	double radius1;
	double radius2;
	double height;
	int height_Seg;
	int cap_Seg;
	int side_Seg;

	//Set the default parameters in the Tube
	Tube_3():radius1(25.0),radius2(20.0),height(50),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Tube_3(double r):radius1(r),radius2(20.0),height(50),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Tube_3(double r1,double r2):radius1(r1),radius2(r2),height(50),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}
	
	//Set the parameters with user defined values
	Tube_3(double r1,double r2,double h):radius1(r1),radius2(r2),height(h),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Tube_3(double r1,double r2,double h,int S):radius1(r1),radius2(r2),height(h),height_Seg(S),cap_Seg(S),side_Seg(S)
	{}

	//Set the parameters with user defined values
	Tube_3(double r1,double r2,double h,int h_S,int c_S,int s_S):radius1(r1),radius2(r2),height(h),height_Seg(h_S),cap_Seg(c_S),side_Seg(s_S)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;

		min(radius1,0);
		min(radius2,0);
		min(height,0);
		maxmin(height_Seg,0,200);
		maxmin(cap_Seg,0,200);
		maxmin(side_Seg,0,200);

		Build_tube<HalfedgeDS> tube(radius1,radius2,height,height_Seg,cap_Seg,side_Seg);
		P.delegate( tube );		
		
		Center = new Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};