class Torus_3 : public Primitives
{
	template <class HDS>
	class Build_torus : public CGAL::Modifier_base<HDS> {
	public:
		int side;//number of side segments
		int segm;//number of height segments
		double r1;//radius of the torus
		double r2;//radius of torus + 2 * radius of tube
		double t;//the angle of twist of the torus
		double r;//the angle of rotation of the torus
		
		//make the constructor with input values
		Build_torus(double rad1,double rad2,double rotation,double twist,int Seg,int s_Seg):r1(rad1),r2(rad2),r(rotation),t(twist),segm(Seg),side(s_Seg)
		{}

		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int numberOfSeparators = side;//number of side segments
			int numVertices = segm * side;//number of Vertices
			int numFaces = segm * side;//number of faces
			t = t / 180.0;
			r = r / 180.0;

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the height segments
			for (int e = 0; e < segm; e++)
			{
				//loop on the side segments and add each point
				for (int s = 0; s < numberOfSeparators; s++)
				{
					//get the radius of each circle
					double rad = r1 + (r2 - r1) / 2 * cos(2 * (e + r) * CGAL_PI / segm + t * s * CGAL_PI / side);
					double x_t = rad * cos(2 * s * CGAL_PI / side);
					double y_t = rad * sin(2 * s * CGAL_PI / side);
					double z_t = (r2 - r1) / 2 * sin(2 * (e + r) * CGAL_PI / segm + t * s * CGAL_PI / side);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}

			//adding vertices to faces
			//loop on height segments
			for (int e = 0; e < segm; e++)
			{
				//loop on side segments
				for (int i = 0; i < numberOfSeparators; i++)
				{
					B.begin_facet();//begin the facet
					//add four point of each facet
					//if we reach to tha last faces, connect them to the first ones
					if (e == segm - 1)
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
	//The basic parameters in the Torus
	double radius1;
	double radius2;
	double rotation;
	double twist;
	int Seg;
	int side_Seg;
	Point_3* Center;

	//Set the default parameters in the Torus
	Torus_3():radius1(25.0),radius2(10.0),rotation(0),twist(0),Seg(24),side_Seg(12)
	{}

	//Set the parameters with user defined values
	Torus_3(double r):radius1(r),radius2(10.0),rotation(0),twist(0),Seg(24),side_Seg(12)
	{}

	//Set the parameters with user defined values
	Torus_3(double r1,double r2):radius1(r1),radius2(r2),rotation(0),twist(0),Seg(24),side_Seg(12)
	{}

	//Set the parameters with user defined values
	Torus_3(double r1,double r2,double r,double t,int S,int s_S):radius1(r1),radius2(r2),rotation(r),twist(t),Seg(S),side_Seg(s_S)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;
		
		min(radius1,0);
		min(radius2,0);
		min(rotation,0);
		min(twist,0);
		maxmin(Seg,0,200);
		maxmin(side_Seg,0,200);

		Build_torus<HalfedgeDS> torus(radius1,radius2,rotation,twist,Seg,side_Seg);
		P.delegate( torus );

		Center = &Point_3(0, 0, 0);

		setMesh(P);
		return P;
	}
};