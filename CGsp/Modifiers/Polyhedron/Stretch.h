class Stretch : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> StAmount;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	Stretch() : StAmount(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&StAmount);
	}

	Stretch(double SAmount) : StAmount(SAmount),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&StAmount);
	}

	Stretch(double SAmount, Axis RAxis) : StAmount(SAmount),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&StAmount);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&StAmount);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis, bool Limit) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0)
	{
		props.push_back(&StAmount);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Stretch(double SAmount, Point_3* C, Axis RAxis, bool Limit, double max, double min) : StAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min)
	{
		props.push_back(&StAmount);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		Eigen::Transform3d t;
		Eigen::Vector3d org;
		double Scale;
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
					Scale = StAmount.val * p.x() / (x_max - x_min);
					x = p.x();
					y = p.y() + p.y() * Scale;
					z = p.z() + p.z() * Scale ;
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
					Scale = StAmount.val * p.y() / (y_max - y_min);
					x = p.x() + p.x() * Scale;
					y = p.y();
					z = p.z() + p.z() * Scale;
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

					if(StAmount.val >= 0)
					{
						if(p.z() >= 0)
							Scale = 1 / (StAmount.val + 1.0 - p.z() * StAmount.val / z_max);
						else
							Scale = 1 / (StAmount.val + 1.0 + p.z() * StAmount.val / z_max);
						z = p.z() + StAmount.val * p.z();
					}

					else
					{
						if(p.z() >= 0)
							Scale = 1 + (p.z() * StAmount.val / z_min - StAmount.val);
						else
							Scale = 1 - (p.z() * StAmount.val / z_min + StAmount.val);
						z = 1 / (1 - StAmount.val) * p.z();
					}
					x = p.x() * Scale;
					y = p.y() * Scale;
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