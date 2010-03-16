class Capsule_3 : Primitives
{
	template <class HDS>
	class Build_capsule : public CGAL::Modifier_base<HDS> {
	public:
		int n;//number of side segments
		int h_s;//number of height segments
		double r;//radius
		double h;//height
		
		//make the constructor with input values
		Build_capsule(double rad,double ht,int seg,int h_seg):r(rad),h(ht),n(seg),h_s(h_seg)
		{}
		
		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int e;
			double segmentRad = 2 * CGAL_PI / n;//the angle of each circle
			int numberOfSeparators = n;//number of side segments
			int numVertices = (n - 1 + h_s) * (numberOfSeparators) + 2;//number of Vertices
			int numFaces = (n / 2 * 2 + h_s) * numberOfSeparators;//number of Faces

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the lower half-sphere segments
			for (e = 1; e < n / 2; e++)
			{
				double i = n / 2 - e;
				double rad = r * cos(1.5 * CGAL_PI + e * CGAL_PI / n);
				double z_s = r - r * sin(i * CGAL_PI / n);

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_s = rad * cos(segmentRad * s);
					double y_s = rad * sin(segmentRad * s);
					B.add_vertex( Point( x_s, y_s, z_s) );
				}
			}

			//loop on the heigth segments
			for (e = 0; e < h_s; e++)
			{
				//get the radius of each circle
				double rad = r;
				double z_s = r + e * ( h - 2 * r) / h_s;

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_s = rad * cos(segmentRad * s);
					double y_s = rad * sin(segmentRad * s);
					B.add_vertex( Point( x_s, y_s, z_s) );
				}
			}

			//loop on the upper half-sphere segments
			for (e = 0; e < n / 2; e++)
			{
				//get the radius of each circle
				double rad = r * cos(e * CGAL_PI / n);
				double z_s = h - r + r * sin(e * CGAL_PI / n);

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_s = rad * cos(segmentRad * s);
					double y_s = rad * sin(segmentRad * s);
					B.add_vertex( Point( x_s, y_s, z_s) );
				}
			}

			//add the upper and lower points of the Sphere
			B.add_vertex( Point( 0, 0, h) );
			B.add_vertex( Point( 0, 0, 0) );

			//adding vertices to faces
			//loop on height segments
			for (e = 0; e < n / 2 * 2 - 2 + h_s; e++)
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
	//The basic parameters in the Capsule
	double radius;
	double heigth;
	int Segs;
	int h_Segs;
	Point_3* Center;

	//Set the default parameters in the Capsule
	Capsule_3 ():radius(15.0),heigth(25.0),Segs(4),h_Segs(1)
	{}

	//Set the parameters with user defined values
	Capsule_3 (double r):radius(r),heigth(2*r),Segs(4),h_Segs(1)
	{}
	
	//Set the parameters with user defined values
	Capsule_3 (double r,int s):radius(r),heigth(2*r),Segs(s),h_Segs(s)
	{}
	
	//Set the parameters with user defined values
	Capsule_3 (double r,double h,int s):radius(r),heigth(h),Segs(s),h_Segs(s)
	{}

	//Set the parameters with user defined values
	Capsule_3 (double r,double h,int s,int h_s):radius(r),heigth(h),Segs(s),h_Segs(h_s)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;

		min(radius,0);
		min(heigth,0);
		maxmin(h_Segs,0,200);
		maxmin(Segs,0,200);

		Build_capsule<HalfedgeDS> capsule(radius,heigth,Segs,h_Segs);
		P.delegate( capsule );

		Center = new Point_3(0, 0, heigth/2);

		setMesh(P);
		return P;
	}
};