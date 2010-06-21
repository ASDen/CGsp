class CGSP_CC Bulge : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> BuAmount;
	Point_3* Center;
	Axis RoAxis;
	BulgeType BuType;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Bulge() : BuAmount(0),Center(NULL),RoAxis(Z_ax),BuType(BRadial),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount) : BuAmount(BAmount),Center(NULL),RoAxis(Z_ax),BuType(BRadial),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, BulgeType BType) : BuAmount(BAmount),Center(NULL),RoAxis(Z_ax),BuType(BType),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Axis RAxis) : BuAmount(BAmount),Center(NULL),RoAxis(RAxis),BuType(BRadial),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BRadial),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis, BulgeType BType) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BType),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis, bool Limit) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BRadial),Limited(Limit),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis, BulgeType BType, bool Limit) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BType),Limited(Limit),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis, bool Limit, double max, double min) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BRadial),Limited(Limit),Upper(max),Lower(min),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Bulge(double BAmount, Point_3* C, Axis RAxis, BulgeType BType, bool Limit, double max, double min) : BuAmount(BAmount),Center(C),RoAxis(RAxis),BuType(BType),Limited(Limit),Upper(max),Lower(min),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&BuAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		if (BuAmount.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;
		double delta;

		Vertex_iterator Begin = P.vertices_begin();
		x_max = x_min = Begin->point().x();
		y_max = y_min = Begin->point().y();
		z_max = z_min = Begin->point().z();

		if (Center == NULL)
		{
			Center = &calc_Center(P);

			X_Center = Center->x();
			Y_Center = Center->y();
			Z_Center = Center->z();
		}

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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (x_max - p.x()) * (p.x() - x_min) / pow((Upper.val - Lower.val), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;
					
					x = p.x();

					if (p.x() > Upper.val)
					{
						y = p.y();
						z = p.z();
					}

					else if (p.x() < Lower.val)
					{
						y = p.y();
						z = p.z();
					}

					else
					{
						if (BuType == BRadial)
						{
							delta = (Rad + k * BuAmount.val) / Rad;

							y = p.y() * delta;
							z = p.z() * delta;
						}
						else if (BuType == BLinear)
						{
							delta = k * BuAmount.val;

							y = p.y() + delta;
							z = p.z() + delta;
						}
						else
							return;
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (y_max - p.y()) * (p.y() - y_min) / pow((Upper.val - Lower.val), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;
					
					y = p.y();

					if (p.y() > Upper.val)
					{
						x = p.x();
						z = p.z();
					}

					else if (p.y() < Lower.val)
					{
						x = p.x();
						z = p.z();
					}

					else
					{
						if (BuType == BRadial)
						{
							delta = (Rad + k * BuAmount.val) / Rad;

							x = p.x() * delta;
							z = p.z() * delta;
						}
						else if (BuType == BLinear)
						{
							delta = k * BuAmount.val;

							x = p.x() + delta;
							z = p.z() + delta;
						}
						else
							return;
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (z_max - p.z()) * (p.z() - z_min) / pow((Upper.val - Lower.val), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;
					
					z = p.z();

					if (p.z() > Upper.val)
					{
						x = p.x();
						y = p.y();
					}

					else if (p.z() < Lower.val)
					{
						x = p.x();
						y = p.y();
					}

					else
					{
						if (BuType == BRadial)
						{
							delta = (Rad + k * BuAmount.val) / Rad;

							x = p.x() * delta;
							y = p.y() * delta;
						}
						else if (BuType == BLinear)
						{
							delta = k * BuAmount.val;

							x = p.x() + delta;
							y = p.y() + delta;
						}
						else
							return;
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