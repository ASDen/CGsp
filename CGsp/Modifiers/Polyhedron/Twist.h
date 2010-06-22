class CGSP_CC Twist : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> TwAngle;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Twist() : TwAngle(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Twist(double TAngle) : TwAngle(TAngle),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Twist(double TAngle, Axis RAxis) : TwAngle(TAngle),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Twist(double TAngle, Point_3* C, Axis RAxis) : TwAngle(TAngle),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Twist(double TAngle, Point_3* C, Axis RAxis, bool Limit) : TwAngle(TAngle),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Twist(double TAngle, Point_3* C, Axis RAxis, bool Limit, double max, double min) : TwAngle(TAngle),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&TwAngle);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		if (TwAngle.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;
		double TwAmount;

		Vertex_iterator Begin = P.vertices_begin();
		x_max = x_min = Begin->point().x();
		y_max = y_min = Begin->point().y();
		z_max = z_min = Begin->point().z();

		/*if (Center == NULL)
		{
			Center = &calc_Center(P);
		}
		X_Center = Center->x();
		Y_Center = Center->y();
		Z_Center = Center->z();*/
		
		double x_c = X_Center.val;
		double y_c = Y_Center.val;
		double z_c = Z_Center.val;

		
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
			default:
				{
					return;
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

				TwAmount = TwAngle.val / (x_max - x_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					x = p.x();

					if (x > Upper.val)
					{
						y = p.z() * sin(TwAmount * Upper.val) + p.y() * cos(TwAmount * Upper.val);
						z = p.z() * cos(TwAmount * Upper.val) - p.y() * sin(TwAmount * Upper.val);
					}

					else if (x < Lower.val)
					{
						y = p.z() * sin(TwAmount * Lower.val) + p.y() * cos(TwAmount * Lower.val);
						z = p.z() * cos(TwAmount * Lower.val) - p.y() * sin(TwAmount * Lower.val);
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

				if (!Limited)
				{
					Upper.val = y_max;
					Lower.val = y_min;
				}

				TwAmount = TwAngle.val / (y_max - y_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();

					if (y > Upper.val)
					{
						x = p.z() * sin(TwAmount * Upper.val) + p.x() * cos(TwAmount * Upper.val);
						z = p.z() * cos(TwAmount * Upper.val) - p.x() * sin(TwAmount * Upper.val);
					}

					else if (y < Lower.val)
					{
						x = p.z() * sin(TwAmount * Lower.val) + p.x() * cos(TwAmount * Lower.val);
						z = p.z() * cos(TwAmount * Lower.val) - p.x() * sin(TwAmount * Lower.val);
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

				if (!Limited)
				{
					Upper.val = z_max;
					Lower.val = z_min;
				}

				TwAmount = TwAngle.val / (z_max - z_min) * CGAL_PI / 180;
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					z = p.z();

					if (z > Upper.val)
					{
						x = p.x() * cos(TwAmount * Upper.val) - p.y() * sin(TwAmount * Upper.val);
						y = p.x() * sin(TwAmount * Upper.val) + p.y() * cos(TwAmount * Upper.val);
					}

					else if (z < Lower.val)
					{
						x = p.x() * cos(TwAmount * Lower.val) - p.y() * sin(TwAmount * Lower.val);
						y = p.x() * sin(TwAmount * Lower.val) + p.y() * cos(TwAmount * Lower.val);
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

		default:
			{
				return;
			}
		}

		t.setIdentity();
		t.pretranslate(org);
		ApplyTransformToPolyhedron(P,t);
	}
};