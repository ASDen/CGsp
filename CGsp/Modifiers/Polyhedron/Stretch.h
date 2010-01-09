class Stretch : public Modifier
{
public:
	double StAmount;
	Point_3 Center;
	char StAxis;

	Stretch(double SAmount,Point_3 C, char SAxis) : StAmount(SAmount),Center(C),StAxis(SAxis)
	{}

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

		double x_c = Center.x();
		double y_c = Center.y();
		double z_c = Center.z();

		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i)
		{
			Point_3 p = i->point();
			switch (StAxis)
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

		switch (StAxis)
		{
		case X_ax:
			{
				x_max -= x_c;
				x_min -= x_c;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					Scale = StAmount * p.x() / (x_max - x_min);
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					Scale = StAmount * p.y() / (y_max - y_min);
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

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					if(StAmount >= 0)
					{
						if(p.z() >= 0)
							Scale = 1 / (StAmount + 1.0 - p.z() * StAmount / z_max);
						else
							Scale = 1 / (StAmount + 1.0 + p.z() * StAmount / z_max);
						z = p.z() + StAmount * p.z();
					}

					else
					{
						if(p.z() >= 0)
							Scale = 1 + (p.z() * StAmount / z_min - StAmount);
						else
							Scale = 1 - (p.z() * StAmount / z_min + StAmount);
						z = 1 / (1 - StAmount) * p.z();
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