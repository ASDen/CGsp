class Spring_3 : public Primitives
{
	template <class HDS>
	class Build_Spring : public CGAL::Modifier_base<HDS> {
	public:
		int side;//number of side segments
		int segm;//number of height segments
		int turn;//number of turns
		double h;//height of the Spring
		double r1;//radius of the Spring
		double r2;//radius of the Tube
		
		//make the constructor with input values
		Build_Spring(double rad1,double rad2,double height,int turn_num,int Seg,int s_Seg):r1(rad1),r2(rad2),turn(turn_num),h(height),segm(Seg),side(s_Seg)
		{}

		void operator()( HDS& hds)
		{
			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int numberOfSeparators = turn * side + 1;//number of side segments
			int numVertices = segm * (turn * side + 1);//number of Vertices
			int numFaces = segm * turn * side;//number of faces

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			//add the points to the builder
			//loop on the height segments
			for (int e = 0; e < segm; e++)
			{
				//get the radius of each circle
				double rad = r1 + r2 * cos(2 * e * CGAL_PI / segm);

				for (int s = 0; s < numberOfSeparators; s++)
				{
					double x_t = rad * cos(2 * s * CGAL_PI / side);
					double y_t = rad * sin(2 * s * CGAL_PI / side);
					//get the Z point
					double z_t = r2 * sin(2 * e * CGAL_PI / segm) + s * h / (side * turn);
					B.add_vertex( Point( x_t, y_t, z_t) );
				}
			}

			//adding vertices to faces
			//loop on height segments
			for (int e = 0; e < segm; e++)
			{
				//loop on side segments
				for (int i = 0; i < numberOfSeparators - 1; i++)
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

			//close the lower face
			B.begin_facet();//begin the facet
			for (int e = 0; e < segm; e++)
			{
				B.add_vertex_to_facet( e * numberOfSeparators );
			}
			B.end_facet();//end the facet

			//close the upper face
			B.begin_facet();//begin the facet
			for (int e = segm; e >= 1; e--)
			{
				B.add_vertex_to_facet( e * numberOfSeparators - 1 );
			}
			B.end_facet();//end the facet

			//close the builder
			B.end_surface();
		}
	};

public:
	//The basic parameters in the Spring
	double radius1;
	double radius2;
	double height;
	int turn_number;
	int Seg;
	int side_Seg;

	//Set the default parameters in the Spring
	Spring_3():radius1(25.0),radius2(10.0),height(0),turn_number(1),Seg(24),side_Seg(12)
	{}

	//Set the parameters with user defined values
	Spring_3(double r1,double r2,double h,int t,int S,int s_S):radius1(r1),radius2(r2),height(h),turn_number(t),Seg(S),side_Seg(s_S)
	{}
	
	Polyhedron Draw()
	{
		Polyhedron P;
		Build_Spring<HalfedgeDS> Spring(radius1,radius2,height,turn_number,Seg,side_Seg);
		P.delegate( Spring );
		setMesh(P);
		return P;
	}
};