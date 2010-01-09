class Bend : Modifier
{
public:
	double BeAngle;
	Point_3 Center;
	Axis RoAxis;
	double Upper;
	double Lower;

	Bend(double BAngle,Point_3 C, Axis RAxis, double max, double min) : BeAngle(BAngle),Center(C),RoAxis(RAxis),Upper(max),Lower(min)
	{}

	void Do(Polyhedron &P)
	{
		if (BeAngle == 0)
			return;
		Eigen::Transform3d t;
		Eigen::Vector3d org;
		double theta;
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
			switch (RoAxis)
			{
				double v;
			case X_ax:
				{
					v = p.x();
					if (v > x_max)
						x_max = v;
					else if (v < x_min)
						x_min = v;
					break;
				}
			case Y_ax:
				{
					v = p.y();
					if (v > y_max)
						y_max = v;
					else if (v < y_min)
						y_min = v;
					break;
				}
			case Z_ax:
				{
					v = p.z();
					if (v > z_max)
						z_max = v;
					else if (v < z_min)
						z_min = v;
					break;
				}
			}
		}

		t.setIdentity();
		org=Eigen::Vector3d(x_c,y_c,z_c);
		t.pretranslate(-org);
		ApplyTransformToPolyhedron(P,t);

		switch (RoAxis)
		{
		case X_ax:
			{
				x_max -= x_c;
				x_min -= x_c;

				if (x_max > Upper)
					x_max = Upper;
				if (x_min < Lower)
					x_min = Lower;

				double k = -BeAngle * CGAL_PI / 180 / (Upper - Lower);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (p.x() > Upper)
					{
						theta = k * Upper;
						x = -sin(theta) * (p.z() - (1 / k)) + cos(theta) * (p.x() - x_max);
						z = cos(theta) * (p.z() - (1 / k)) + 1 / k + sin(theta) * (p.x() - x_max);
					}

					else if (p.x() < Lower)
					{
						theta = k * Lower;
						x = -sin(theta) * (p.z() - (1 / k)) + cos(theta) * (p.x() - x_min);
						z = cos(theta) * (p.z() - (1 / k)) + 1 / k + sin(theta) * (p.x() - x_min);
					}

					else
					{
						theta = k * p.x();
						x = -sin(theta) * (p.z() - (1 / k));
						z = cos(theta) * (p.z() - (1 / k)) + 1 / k;
					}
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Y_ax:
			{
				y_max -= y_c;
				y_min -= y_c;

				if (y_max > Upper)
					y_max = Upper;
				if (y_min < Lower)
					y_min = Lower;

				double k = BeAngle * CGAL_PI / 180 / (Upper - Lower);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					z = p.z();

					if (p.y() > Upper)
					{
						theta = k * Upper;
					
						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.y() - y_max);
						y = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.y() - y_max);
					}

					else if (p.y() < Lower)
					{
						theta = k * Lower;
					
						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.y() - y_min);
						y = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.y() - y_min);
					}

					else
					{
						theta = k * p.y();
					
						x = cos(theta) * (p.x() - (1 / k)) + 1 / k;
						y = -sin(theta) * (p.x() - (1 / k));
					}
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Z_ax:
			{
				z_max -= z_c;
				z_min -= z_c;

				if (z_max > Upper)
					z_max = Upper;
				if (z_min < Lower)
					z_min = Lower;

				double k = BeAngle * CGAL_PI / 180 / (Upper - Lower);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (p.z() > Upper)
					{
						theta = k * Upper;

						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.z() - z_max);
						z = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.z() - z_max);
					}

					else if (p.z() < Lower)
					{
						theta = k * Lower;

						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.z() - z_min);
						z = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.z() - z_min);
					}

					else
					{
						theta = k * p.z();

						x = cos(theta) * (p.x() - (1 / k)) + 1 / k;
						z = -sin(theta) * (p.x() - (1 / k));
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