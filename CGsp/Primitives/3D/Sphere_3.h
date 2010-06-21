// Algorithm source 
// http://www.codeproject.com/KB/WPF/XamlUVSphere.aspx

class CGSP_CC Sphere_3 : public Primitives
{
	template <class HDS>
	class Build_sphere : public CGAL::Modifier_base<HDS> {
	public:
		int n;//number of side segments
		double r;//radius
		
		//make the constructor with input values
		Build_sphere(double rad,int seg):n(seg),r(rad)
		{}
		
		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int e;
			double segmentRad = 2 * CGAL_PI / n;//the angle of each circle
			int numberOfSeparators = n;//number of side segments
			int numVertices = (n / 2 - 1) * (numberOfSeparators) + 2;//number of Vertices
			int numFaces = n / 2 * numberOfSeparators;//number of Faces

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the height segments
			for (e = 1; e <= n / 2 - 1; e++)
			{
				double i = e - n / 4.0;
				//get the radius of each circle
				double rad = r * cos(segmentRad * i);
				double z_s = r * sin(segmentRad * i);

				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_s = rad * cos(segmentRad * s);
					double y_s = rad * sin(segmentRad * s);
					B.add_vertex( Point( x_s, y_s, z_s) );
				}
			}

			//add the upper and lower points of the Sphere
			B.add_vertex( Point( 0, 0, r) );
			B.add_vertex( Point( 0, 0, -1*r) );

			//adding vertices to faces
			//loop on height segments
			for (e = 0; e < n / 2 - 2; e++)
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
	//The basic parameters in the Sphere
	double radius;
	int Segs;

	//Set the default parameters in the Sphere
	Sphere_3():radius(25.0),Segs(4)
	{}

	//Set the parameters with user defined values
	Sphere_3(double r):radius(r),Segs(4)
	{}

	//Set the parameters with user defined values
	Sphere_3(double r,int s):radius(r),Segs(s)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;

		min(radius,0);
		maxmin(Segs,0,200);

		Build_sphere<HalfedgeDS> sphere(radius,Segs);
		P.delegate( sphere );

		Center = new Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};