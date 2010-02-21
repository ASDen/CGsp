class Skew : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> SkAmount;
	Point_3* Center;
	Axis RoAxis;
	bool Limited;
	AnimatablePropery<double,Interpolator> Upper;
	AnimatablePropery<double,Interpolator> Lower;

	Skew() : SkAmount(0),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&SkAmount);
	}

	Skew(double SAmount) : SkAmount(SAmount),Center(NULL),RoAxis(Z_ax),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&SkAmount);
	}

	Skew(double SAmount, Axis RAxis) : SkAmount(SAmount),Center(NULL),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&SkAmount);
	}

	Skew(double SAmount, Point_3* C, Axis RAxis) : SkAmount(SAmount),Center(C),RoAxis(RAxis),Limited(false),Upper(0),Lower(0)
	{
		props.push_back(&SkAmount);
	}

	Skew(double SAmount, Point_3* C, Axis RAxis, bool Limit) : SkAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(0),Lower(0)
	{
		props.push_back(&SkAmount);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	Skew(double SAmount, Point_3* C, Axis RAxis, bool Limit, double max, double min) : SkAmount(SAmount),Center(C),RoAxis(RAxis),Limited(Limit),Upper(max),Lower(min)
	{
		props.push_back(&SkAmount);
		props.push_back(&Upper);
		props.push_back(&Lower);
	}

	void Do(Polyhedron &P)
	{
		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;
		double Scale;

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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					x = p.x();
					y = p.y();

					if (x > Upper.val)
					{
						Scale = SkAmount.val * Upper.val / (Upper.val - Lower.val);

						z = p.z() - Scale;
					}

					else if (x < Lower.val)
					{
						Scale = SkAmount.val * Lower.val / (Upper.val - Lower.val);

						z = p.z() - Scale;
					}

					else
					{
						Scale = SkAmount.val * p.x() / (Upper.val - Lower.val);

						z = p.z() - Scale;
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
					y = p.y();
					z = p.z();

					if (y > Upper.val)
					{
						Scale = SkAmount.val * Upper.val / (Upper.val - Lower.val);

						x = p.x() + Scale;
					}

					else if (y < Lower.val)
					{
						Scale = SkAmount.val * Lower.val / (Upper.val - Lower.val);

						x = p.x() + Scale;
					}

					else
					{
						Scale = SkAmount.val * p.y() / (Upper.val - Lower.val);

						x = p.x() + Scale;
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
					y = p.y();
					z = p.z();

					if (z > Upper.val)
					{
						Scale = SkAmount.val * Upper.val / (Upper.val - Lower.val);

						x = p.x() + Scale;
					}

					else if (z < Lower.val)
					{
						Scale = SkAmount.val * Lower.val / (Upper.val - Lower.val);

						x = p.x() + Scale;
					}

					else
					{
						Scale = SkAmount.val * p.z() / (Upper.val - Lower.val);

						x = p.x() + Scale;
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