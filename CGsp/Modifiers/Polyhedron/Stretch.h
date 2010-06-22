class CGSP_CC Stretch : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> StAmount;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Stretch() : StAmount(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount) : StAmount(SAmount),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount, Axis RAxis) : StAmount(SAmount),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis, bool Limit) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis, bool Limit, double max, double min) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&StAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		if (StAmount.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;
		double delta, Scale;

		Vertex_iterator Begin = P.vertices_begin();
		x_max = x_min = Begin->point().x();
		y_max = y_min = Begin->point().y();
		z_max = z_min = Begin->point().z();

		/*if (Center == NULL)
		{
			Center = &calc_Center(P);

			X_Center = Center->x();
			Y_Center = Center->y();
			Z_Center = Center->z();
		}*/
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
				
				if (x_max > Upper.val)
					x_max = Upper.val;
				if (x_min < Lower.val)
					x_min = Lower.val;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					if (StAmount.val / (x_max - x_min) >= 0.9)
					{
						Scale = 0.9;
						x = p.x() * (1 + StAmount.val / (x_max - x_min));
					}
					else if (StAmount.val / (x_max - x_min) <= -0.9)
					{
						Scale = StAmount.val / (x_max - x_min);
						x = p.x() * (0.1 / (x_max - x_min));
					}
					else
					{
						Scale = StAmount.val / (x_max - x_min);
						x = p.x() * (1 + StAmount.val / (x_max - x_min));
					}

					if (p.x() > Upper.val)
					{
						delta = Upper.val * Upper.val * Scale / x_max / x_max - Scale + 1.0;
						y = p.y() * delta;
						z = p.z() * delta;
					}

					else if (p.x() < Lower.val)
					{
						delta = Lower.val * Lower.val * Scale / x_min / x_min - Scale + 1.0;
						y = p.y() * delta;
						z = p.z() * delta;
					}

					else
					{
						if (x_max > -x_min)
							delta = p.x() * p.x() * Scale / x_max / x_max - Scale + 1.0;
						else
							delta = p.x() * p.x() * Scale / x_min / x_min - Scale + 1.0;
						y = p.y() * delta;
						z = p.z() * delta;
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					if (StAmount.val / (y_max - y_min) >= 0.9)
					{
						Scale = 0.9;
						y = p.y() * (1 + StAmount.val / (y_max - y_min));
					}
					else if (StAmount.val / (y_max - y_min) <= -0.9)
					{
						Scale = StAmount.val / (y_max - y_min);
						y = p.y() * (0.1);
					}
					else
					{
						Scale = StAmount.val / (y_max - y_min);
						y = p.y() * (1 + StAmount.val / (y_max - y_min));
					}

					if (p.y() > Upper.val)
					{
						delta = Upper.val * Upper.val * Scale / y_max / y_max - Scale + 1.0;
						x = p.x() * delta;
						z = p.z() * delta;
					}

					else if (p.y() < Lower.val)
					{
						delta = Lower.val * Lower.val * Scale / y_min / y_min - Scale + 1.0;
						x = p.x() * delta;
						z = p.z() * delta;
					}

					else
					{
						if (y_max > -y_min)
							delta = p.y() * p.y() * Scale / y_max / y_max - Scale + 1.0;
						else
							delta = p.y() * p.y() * Scale / y_min / y_min - Scale + 1.0;
						x = p.x() * delta;
						z = p.z() * delta;
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					if (StAmount.val / (z_max - z_min) >= 0.9)
					{
						Scale = 0.9;	
						z = p.z() * (1 + StAmount.val / (z_max - z_min));
					}
					else if (StAmount.val / (z_max - z_min) <= -0.9)
					{
						Scale = StAmount.val / (z_max - z_min);
						z = p.z() * (0.1 / (z_max - z_min));
					}
					else
					{
						Scale = StAmount.val / (z_max - z_min);
						z = p.z() * (1 + StAmount.val / (z_max - z_min));
					}

					if (p.z() > Upper.val)
					{
						delta = Upper.val * Upper.val * Scale / z_max / z_max - Scale + 1.0;
						x = p.x() * delta;
						y = p.y() * delta;
					}

					else if (p.z() < Lower.val)
					{
						delta = Lower.val * Lower.val * Scale / z_min / z_min - Scale + 1.0;
						x = p.x() * delta;
						y = p.y() * delta;
					}

					else
					{
						if (z_max > -z_min)
							delta = p.z() * p.z() * Scale / z_max / z_max - Scale + 1.0;
						else
							delta = p.z() * p.z() * Scale / z_min / z_min - Scale + 1.0;
						
						x = p.x() * delta;
						y = p.y() * delta;
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