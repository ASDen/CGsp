class Twist : Modifier
{
public:
	double TwAngle;
	Point_3 Center;
	Axis TwAxis;
	double Upper;
	double Lower;

	Twist(double TAngle,Point_3 C, Axis TAxis, double max, double min) : TwAngle(TAngle),Center(C),TwAxis(TAxis),Upper(max),Lower(min)
	{}

	void Do(Polyhedron &P)
	{
		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double TwAmount;
		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;

		Vertex_iterator Begin = P.vertices_begin();
		x_max = x_min = Begin->point().x();
		y_max = y_min = Begin->point().y();
		z_max = z_min = Begin->point().z();

		double x_c = Center.x();
		double y_c = Center.y();
		double z_c = Center.z();

		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i)
		{
			Point_3 p = i->point();
			switch (TwAxis)
			{
				double v;
			case X_ax:
				v = p.x();
				if (v > x_max)
					x_max = v;
				else if (v < x_min)
					x_min = v;
				break;
			case Y_ax:
				v = p.y();
				if (v > y_max)
					y_max = v;
				else if (v < y_min)
					y_min = v;
				break;
			case Z_ax:
				v = p.z();
				if (v > z_max)
					z_max = v;
				else if (v < z_min)
					z_min = v;
				break;
			}
		}

		t.setIdentity();
		org=Eigen::Vector3d(x_c,y_c,z_c);
		t.pretranslate(-org);
		ApplyTransformToPolyhedron(P,t);

		switch (TwAxis)
		{
		case X_ax:
			{
				x_max -= x_c;
				x_min -= x_c;

				TwAmount = TwAngle / (x_max - x_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					x = p.x();

					if (x > Upper)
					{
						y = p.z() * sin(TwAmount * Upper) + p.y() * cos(TwAmount * Upper);
						z = p.z() * cos(TwAmount * Upper) - p.y() * sin(TwAmount * Upper);
					}

					else if (x < Lower)
					{
						y = p.z() * sin(TwAmount * Lower) + p.y() * cos(TwAmount * Lower);
						z = p.z() * cos(TwAmount * Lower) - p.y() * sin(TwAmount * Lower);
					}

					else
					{
						y = p.z() * sin(TwAmount * p.x()) + p.y() * cos(TwAmount * p.x());
						z = p.z() * cos(TwAmount * p.x()) - p.y() * sin(TwAmount * p.x());
					}
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Y_ax:
			{
				y_max -= y_c;
				y_min -= y_c;

				TwAmount = TwAngle / (y_max - y_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (y > Upper)
					{
						x = p.z() * sin(TwAmount * Upper) + p.x() * cos(TwAmount * Upper);
						z = p.z() * cos(TwAmount * Upper) - p.x() * sin(TwAmount * Upper);
					}

					else if (y < Lower)
					{
						x = p.z() * sin(TwAmount * Lower) + p.x() * cos(TwAmount * Lower);
						z = p.z() * cos(TwAmount * Lower) - p.x() * sin(TwAmount * Lower);
					}

					else
					{
						x = p.z() * sin(TwAmount * p.y()) + p.x() * cos(TwAmount * p.y());
						z = p.z() * cos(TwAmount * p.y()) - p.x() * sin(TwAmount * p.y());
					}
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Z_ax:
			{
				z_max -= z_c;
				z_min -= z_c;

				TwAmount = TwAngle / (z_max - z_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					z = p.z();

					if (z > Upper)
					{
						x = p.x() * cos(TwAmount * Upper) - p.y() * sin(TwAmount * Upper);
						y = p.x() * sin(TwAmount * Upper) + p.y() * cos(TwAmount * Upper);
					}

					else if (z < Lower)
					{
						x = p.x() * cos(TwAmount * Lower) - p.y() * sin(TwAmount * Lower);
						y = p.x() * sin(TwAmount * Lower) + p.y() * cos(TwAmount * Lower);
					}

					else
					{
						x = p.x() * cos(TwAmount * p.z()) - p.y() * sin(TwAmount * p.z());
						y = p.x() * sin(TwAmount * p.z()) + p.y() * cos(TwAmount * p.z());
					}
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}
		}

		t.setIdentity();
		t.pretranslate(org);
		ApplyTransformToPolyhedron(P,t);
	}
};