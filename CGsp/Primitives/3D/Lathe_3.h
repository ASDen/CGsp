class CGSP_CC Lathe_3 : public Primitives
{
	template <class HDS>
	class Build_lathe : public CGAL::Modifier_base<HDS> {
	public:
		std::vector<Point_3> Arr;
		Point_3* C;
		Axis R_A;
		int S;
		double R_an;
		
		Build_lathe(std::vector<Point_3> Array,Point_3* Center,Axis R_Axis,int Seg,double R_Angle):Arr(Array),C(Center),R_A(R_Axis),S(Seg),R_an(R_Angle)
		{}

		void operator()( HDS& hds)
		{
			if (Arr.size() == 0)
				return;

			CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			int numberOfSeparators;
			if (R_an >= 360)
			{
				R_an = 360;
				numberOfSeparators = S;//number of side segments
			}
			else if (R_an > 0 && R_an < 360)
				numberOfSeparators = S + 1;//number of side segments
			else
			{
				R_an = 0;
				numberOfSeparators = S;//number of side segments

				return;
			}
			double R = R_an * CGAL_PI / 180.0;
			double segmentRad = R / numberOfSeparators;//the angle of each circle
			
			int numVertices = numberOfSeparators * Arr.size();//number of Vertices
			int numFaces = numberOfSeparators * (Arr.size() - 1);//number of Faces

			//begin the builder with the number of faces and vertices
			B.begin_surface( numVertices, numFaces );

			switch (R_A)
			{
			case X_ax:
				{
					//add the points to the builder
					//loop on the array points
					for (int e = 0; e < Arr.size(); e++)
					{
						double add_ang;
						Point p = Arr[e];

						//get the radius of each circle
						double rad = sqrt (pow ( (p.y() - C->y()) ,2 ) + pow ( (p.z() - C->z()) ,2 ));
						double x_t = p.x();

						if (p.y() == 0 && p.z()== 0)
							continue;
						else if (p.y() == 0)
							add_ang = pow(sin(rad / p.z()),-1);
						else
							add_ang = pow(cos(rad / p.y()),-1);

						//loop on the side segments and add each point
						for (int s = 0; s < numberOfSeparators; s++)
						{
							double y_t = rad * cos(s * segmentRad);
							double z_t = rad * sin(s * segmentRad);
							B.add_vertex( Point( x_t, y_t, z_t) );
						}
					}
				}
			case Y_ax:
				{
					//add the points to the builder
					//loop on the array points
					for (int e = 0; e < Arr.size(); e++)
					{
						double add_ang;
						Point p = Arr[e];

						//get the radius of each circle
						double rad = sqrt (pow ( (p.x() - C->x()) ,2 ) + pow ( (p.z() - C->z()) ,2 ));
						double y_t = p.y();

						if (p.x() == 0 && p.z()== 0)
							continue;
						else if (p.z() == 0)
							add_ang = pow(sin(rad / p.x()),-1);
						else
							add_ang = pow(cos(rad / p.z()),-1);

						//loop on the side segments and add each point
						for (int s = 0; s < numberOfSeparators; s++)
						{
							double x_t = rad * sin(s * segmentRad);
							double z_t = rad * cos(s * segmentRad);
							B.add_vertex( Point( x_t, y_t, z_t) );
						}
					}
				}
			case Z_ax:
				{
					//add the points to the builder
					//loop on the array points
					for (int e = 0; e < Arr.size(); e++)
					{
						double add_ang;
						Point p = Arr[e];

						//get the radius of each circle
						double rad = sqrt (pow ( (p.x() - C->x()) ,2 ) + pow ( (p.y() - C->y()) ,2 ));
						double z_t = p.z();

						if (p.y() == 0 && p.x()== 0)
							continue;
						else if (p.y() == 0)
							add_ang = pow(cos(rad / p.x()),-1) * 2;
						else
							add_ang = pow(sin(rad / p.y()),-1) * 2;

						//loop on the side segments and add each point
						for (int s = 0; s < numberOfSeparators; s++)
						{
							double x_t = rad * cos(s * segmentRad + add_ang);
							double y_t = rad * sin(s * segmentRad + add_ang);
							B.add_vertex( Point( x_t, y_t, z_t) );
						}
					}
				}
			}
			

			//adding vertices to faces
			//loop on height segments
			for (int e = 0; e < Arr.size() - 1; e++)
			{
				//loop on side segments
				for (int i = 0; i < S; i++)
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

			//close the builder
			B.end_surface();
		}
	};

public:
	//The basic parameters in the Lathe Modifier
	std::vector<Point_3> Arr_Points;
	Point_3* Center;
	Axis R_Axis;
	int Seg;
	double R_Angle;

	//Set the parameters with user defined values
	Lathe_3(std::vector<Point_3> arr,Point_3* C,Axis R_A,int S,double R_an):Arr_Points(arr),Center(C),R_Axis(R_A),Seg(S),R_Angle(R_an)
	{}

	Lathe_3(){}
	
	Polyhedron Draw()
	{
		Polyhedron P;
		Build_lathe<HalfedgeDS> lathe(Arr_Points,Center,R_Axis,Seg,R_Angle);
		P.delegate( lathe );
		
		setMesh(P);
		return P;
	}
};