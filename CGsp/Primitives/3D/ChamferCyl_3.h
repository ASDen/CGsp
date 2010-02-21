class ChamferCyl_3 : public Primitives
{
	template <class HDS>
	class Build_ChamferCyl : public CGAL::Modifier_base<HDS> {
	public:
		int cap;//the cap segments
		int side;//the side segments
		int segm;//the height segments
		int fill;//the fillet segments
		double f;//the height of the cap
		double h;//the height
		double r;//the inner radius
		
		Build_ChamferCyl(double rad,double height,double fillet,int h_Seg,int f_Seg,int c_Seg,int s_Seg):r(rad),h(height),f(fillet),segm(h_Seg),fill(f_Seg),side(s_Seg),cap(c_Seg)
		{}

		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int e;
			double segmentRad = 2 * CGAL_PI / side;//the angle of each circle
			int numberOfSeparators = side;//number of side segments
			int numVertices = side * (segm + 2 * cap + 2 * fill);//number of Vertices
			int numFaces = side * (segm + 2 * cap + 2 * fill);//number of Faces

			if (2 * f >= h || f >= r)
			{
				if (h / 2 < r)
				{
					f = h / 2;
					segm = 0;
				}
				else
				{
					f = r;
					cap = 0;
				}
			}

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the lower cap segments
			for (e = 1; e <= cap; e++)
			{
				//get the radius of each circle
				double rad = e * (r - f) / cap;
				double z_t = 0;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the lower fillet segments
			for (e = 1; e < fill; e++)
			{
				double i = fill - e;
				double rad = r - f + f * cos(1.5 * CGAL_PI + e * CGAL_PI / fill / 2.0);
				double z_t = f - f * sin(i * CGAL_PI / fill / 2.0);

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
				double z_t = f + e * (h - 2 * f) / segm;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}
			//loop on the upper fillet segments
			for (e = 0; e < fill; e++)
			{
				//get the radius of each circle
				double rad = r - f + f * cos(e * CGAL_PI / fill / 2.0);
				double z_t = h - f + f * sin(e * CGAL_PI / fill / 2.0);

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
				double rad = e * (r - f) / cap;
				double z_t = h;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(s * segmentRad);
					double y_t = rad * sin(s * segmentRad);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}

			//add the upper and lower points of the Sphere
			B.add_vertex( Point( 0, 0, h) );
			B.add_vertex( Point( 0, 0, 0) );

			//adding vertices to faces
			//loop on height segments
			for (e = 0; e < 2 * (fill + (cap - 1)) + segm; e++)
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
	//The basic parameters in the ChamferCyl
	double radius;
	double height;
	double fillet;
	int height_Seg;
	int fillet_Seg;
	int cap_Seg;
	int side_Seg;
	Point_3* Center;

	//Set the default parameters in the ChamferCyl
	ChamferCyl_3():radius(25.0),height(50),fillet(5.0),height_Seg(1),fillet_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	ChamferCyl_3(double r):radius(r),height(2*r),fillet(r),height_Seg(1),fillet_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	ChamferCyl_3(double r,int S):radius(r),height(2*r),fillet(r),height_Seg(S),fillet_Seg(S),cap_Seg(S),side_Seg(S)
	{}

	//Set the parameters with user defined values
	ChamferCyl_3(double r,double h,double f):radius(r),height(h),fillet(f),height_Seg(1),fillet_Seg(1),cap_Seg(1),side_Seg(24)
	{}

	//Set the parameters with user defined values
	ChamferCyl_3(double r,double h,double f,int S):radius(r),height(h),fillet(f),height_Seg(S),fillet_Seg(S),cap_Seg(S),side_Seg(S)
	{}

	//Set the parameters with user defined values
	ChamferCyl_3(double r,double h,double f,int h_S,int f_S,int c_S,int s_S):radius(r),height(h),fillet(f),height_Seg(h_S),fillet_Seg(f_S),cap_Seg(c_S),side_Seg(s_S)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;

		min(radius,0);
		min(height,0);
		min(fillet,0);
		maxmin(height_Seg,0,200);
		maxmin(fillet_Seg,0,200);
		maxmin(cap_Seg,0,200);
		maxmin(side_Seg,0,200);

		Build_ChamferCyl<HalfedgeDS> ChamferCyl(radius,height,fillet,height_Seg,fillet_Seg,cap_Seg,side_Seg);
		P.delegate( ChamferCyl );

		Center = &Point_3(0, 0, height/2);

		setMesh(P);
		return P;
	}
};