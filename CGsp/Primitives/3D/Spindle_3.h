class CGSP_CC Spindle_3 : public Primitives
{
	template <class HDS>
	class Build_Spindle : public CGAL::Modifier_base<HDS> {
	public:
		int cap;//the cap segments
		int side;//the side segments
		int segm;//the height segments
		double c_h;//the height of the cap
		double h;//the height
		double r;//the inner radius
		
		Build_Spindle(double rad,double height,double cap_height,int h_Seg,int c_Seg,int s_Seg):r(rad),h(height),c_h(cap_height),segm(h_Seg),side(s_Seg),cap(c_Seg)
		{}

		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int e;
			double segmentRad = 2 * CGAL_PI / side;//the angle of each circle
			int numberOfSeparators = side;//number of side segments
			int numVertices = side * (segm + 2 * cap);//number of Vertices
			int numFaces = side * (segm + 2 * cap);//number of Faces

			if ( 2 * c_h > h )
			{
				c_h = h / 2;
				segm = 0;
			}

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the lower cap segments
			for (e = 1; e < cap; e++)
			{
				//get the radius of each circle
				double rad = e * r / cap;
				double z_t = e * c_h / cap - h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the height segments
			for (e = 0; e < segm; e++)
			{
				//get the radius of each circle
				double rad = r;
				double z_t = c_h + e * (h - 2 * c_h) / segm - h/2;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the upper cap segments
			for (e = cap; e > 0; e--)
			{
				//get the radius of each circle
				double rad =  e * r / cap;
				double z_t = h/2 - e * c_h / cap;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}

			//add the upper and lower points of the Sphere
			B.add_vertex( Point( 0, 0, h/2) );
			B.add_vertex( Point( 0, 0,-h/2) );

			//adding vertices to faces
			//loop on height segments
			for (e = 0; e < 2 * (cap - 1) + segm; e++)
			{
				//loop on side segments
				for (int i = 0; i < numberOfSeparators; i++)
				{
					B.begin_facet();//begin the facet
					//add four point of each facet
					B.add_vertex_to_facet( e * numberOfSeparators + (i + 1) % numberOfSeparators + numberOfSeparators );
					B.add_vertex_to_facet( e * numberOfSeparators + i + numberOfSeparators );
					B.add_vertex_to_facet( e * numberOfSeparators + i );
					B.add_vertex_to_facet( e * numberOfSeparators +(i + 1) % numberOfSeparators );
					B.end_facet();//end the facet
				}
			}

			//filling up & down holes
			//loop on side segments
			for (int i = 0; i < numberOfSeparators; i++)
			{
				B.begin_facet();//begin the upper facet
				//add three point of each facet for the upper point of the sphere
				B.add_vertex_to_facet( e * numberOfSeparators + (i + 1) %numberOfSeparators );
				B.add_vertex_to_facet( numberOfSeparators * (e + 1) );
				B.add_vertex_to_facet( e * numberOfSeparators + i );
				B.end_facet();//end the uper facet

				B.begin_facet();//begin the lower facet
				//add three point of each facet for the upper point of the sphere
				B.add_vertex_to_facet( numberOfSeparators * (e + 1) + 1 );
				B.add_vertex_to_facet( (i + 1) % numberOfSeparators );
				B.add_vertex_to_facet( i );
				B.end_facet();//end the lower facet
			}

			//close the builder
			B.end_surface();
		}
	};

public:
	//The basic parameters in the Spindle
	double radius;
	double height;
	double cap_height;
	int height_Seg;
	int cap_Seg;
	int side_Seg;

	//Set the default parameters in the Spindle
	Spindle_3():radius(25.0),height(50),cap_height(5.0),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Spindle_3(double r):radius(r),height(2*r),cap_height(r/2),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Spindle_3(double r,int S):radius(r),height(2*r),cap_height(r),height_Seg(S),cap_Seg(S),side_Seg(S)
	{}

	//Set the parameters with user defined values
	Spindle_3(double r,double h,double c_h):radius(r),height(h),cap_height(c_h),height_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	Spindle_3(double r,double h,double c_h,int S):radius(r),height(h),cap_height(c_h),height_Seg(S),cap_Seg(S),side_Seg(S)
	{}

	//Set the parameters with user defined values
	Spindle_3(double r,double h,double c_h,int h_S,int c_S,int s_S):radius(r),height(h),cap_height(c_h),height_Seg(h_S),cap_Seg(c_S),side_Seg(s_S)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;

		min(radius,0);
		min(cap_height,0);
		min(height,0);

		Build_Spindle<HalfedgeDS> Spindle(radius,height,cap_height,height_Seg,cap_Seg,side_Seg);
		P.delegate( Spindle );
		
		Center = new Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};