class CGSP_CC Squeeze : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> SqAmount;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Squeeze() : SqAmount(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Squeeze(double SAmount) : SqAmount(SAmount),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Squeeze(double SAmount, Axis RAxis) : SqAmount(SAmount),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Squeeze(double SAmount, Point_3* C, Axis RAxis) : SqAmount(SAmount),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Squeeze(double SAmount, Point_3* C, Axis RAxis, bool Limit) : SqAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Squeeze(double SAmount, Point_3* C, Axis RAxis, bool Limit, double max, double min) : SqAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&SqAmount);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		if (SqAmount.val == 0)
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
			double v;

			v = p.x();
			if (v > x_max)
				x_max = v;
			else if (v < x_min)
				x_min = v;

			v = p.y();
			if (v > y_max)
				y_max = v;
			else if (v < y_min)
				y_min = v;

			v = p.z();
			if (v > z_max)
				z_max = v;
			else if (v < z_min)
				z_min = v;
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
				
				y_max -= y_c;
				y_min -= y_c;

				z_max -= z_c;
				z_min -= z_c;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (y_max - p.y()) * (p.y() - y_min) / pow((y_max - y_min), 2)
							+ 2 * (z_max - p.z()) * (p.z() - z_min) / pow((z_max - z_min), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;

					delta = (Rad + k * SqAmount.val) / Rad / 2;
					
					x = p.x() + p.x() * delta;
					y = p.y();
					z = p.z();
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Y_ax:
			{
				x_max -= x_c;
				x_min -= x_c;
				
				y_max -= y_c;
				y_min -= y_c;

				z_max -= z_c;
				z_min -= z_c;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (x_max - p.x()) * (p.x() - x_min) / pow((x_max - x_min), 2)
							+ 2 * (z_max - p.z()) * (p.z() - z_min) / pow((z_max - z_min), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;

					delta = (Rad + k * SqAmount.val) / Rad / 2;
					
					x = p.x();
					y = p.y() + p.y() * delta;
					z = p.z();
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Z_ax:
			{
				x_max -= x_c;
				x_min -= x_c;
				
				y_max -= y_c;
				y_min -= y_c;

				z_max -= z_c;
				z_min -= z_c;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					double k = 2 * (x_max - p.x()) * (p.x() - x_min) / pow((x_max - x_min), 2)
							+ 2 * (y_max - p.y()) * (p.y() - y_min) / pow((y_max - y_min), 2);
					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;

					delta = (Rad + k * SqAmount.val) / Rad;

					x = p.x();
					y = p.y();
					z = p.z() + p.z() * delta;
					
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