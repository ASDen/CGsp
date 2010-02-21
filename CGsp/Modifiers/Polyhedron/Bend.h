class Bend : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> BeAngle;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	Bend() : BeAngle(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&BeAngle);
	}

	Bend(double BAngle) : BeAngle(BAngle),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&BeAngle);
	}

	Bend(double BAngle, Axis RAxis) : BeAngle(BAngle),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&BeAngle);
	}

	Bend(double BAngle, Point_3* C, Axis RAxis) : BeAngle(BAngle),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&BeAngle);
	}

	Bend(double BAngle, Point_3* C, Axis RAxis, bool Limit) : BeAngle(BAngle),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0)
	{
		props.push_back(&BeAngle);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bend(double BAngle, Point_3* C, Axis RAxis, bool Limit, double max, double min) : BeAngle(BAngle),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min)
	{
		props.push_back(&BeAngle);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		if (BeAngle.val == 0)
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

		if (Center == NULL)
		{
			Center = &calc_Center(P);
		}
		double x_c = Center->x();
		double y_c = Center->y();
		double z_c = Center->z();

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

				if (!Limited)
				{
					Upper.val = x_max;
					Lower.val = x_min;
				}

				if (x_max > Upper.val)
					x_max = Upper.val;
				if (x_min < Lower.val)
					x_min = Lower.val;

				double k = -BeAngle.val * CGAL_PI / 180 / (Upper.val - Lower.val);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (p.x() > Upper.val)
					{
						theta = k * Upper.val;
						x = -sin(theta) * (p.z() - (1 / k)) + cos(theta) * (p.x() - x_max);
						z = cos(theta) * (p.z() - (1 / k)) + 1 / k + sin(theta) * (p.x() - x_max);
					}

					else if (p.x() < Lower.val)
					{
						theta = k * Lower.val;
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

				if (!Limited)
				{
					Upper.val = y_max;
					Lower.val = y_min;
				}

				if (y_max > Upper.val)
					y_max = Upper.val;
				if (y_min < Lower.val)
					y_min = Lower.val;

				double k = BeAngle.val * CGAL_PI / 180 / (Upper.val - Lower.val);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					z = p.z();

					if (p.y() > Upper.val)
					{
						theta = k * Upper.val;
					
						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.y() - y_max);
						y = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.y() - y_max);
					}

					else if (p.y() < Lower.val)
					{
						theta = k * Lower.val;
					
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

				if (!Limited)
				{
					Upper.val = z_max;
					Lower.val = z_min;
				}

				if (z_max > Upper.val)
					z_max = Upper.val;
				if (z_min < Lower.val)
					z_min = Lower.val;

				double k = BeAngle.val * CGAL_PI / 180 / (Upper.val - Lower.val);
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (p.z() > Upper.val)
					{
						theta = k * Upper.val;

						x = cos(theta) * (p.x() - (1 / k)) + 1 / k + sin(theta) * (p.z() - z_max);
						z = -sin(theta) * (p.x() - (1 / k)) + cos(theta) * (p.z() - z_max);
					}

					else if (p.z() < Lower.val)
					{
						theta = k * Lower.val;

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