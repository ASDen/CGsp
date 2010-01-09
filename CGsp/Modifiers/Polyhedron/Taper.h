class Taper : public Modifier
{
public:
	double TaAmount;
	Point_3 Center;
	char TaAxis;
	double Upper;
	double Lower;

	Taper(double TAmount,Point_3 C, char TAxis, double max, double min) : TaAmount(TAmount),Center(C),TaAxis(TAxis),Upper(max),Lower(min)
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
			switch (TaAxis)
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

		switch (TaAxis)
		{
		case X_ax:
			{
				x_max -= x_c;
				x_min -= x_c;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					x = p.x();
					
					if (x > Upper)
					{
						Scale = TaAmount * Upper / (x_max - x_min);

						y = p.y() + p.y() * Scale;
						z = p.z() + p.z() * Scale;
					}

					else if (x < Lower)
					{
						Scale = TaAmount * Lower / (x_max - x_min);

						y = p.y() + p.y() * Scale;
						z = p.z() + p.z() * Scale;
					}

					else
					{
						Scale = TaAmount * p.x() / (x_max - x_min);

						y = p.y() + p.y() * Scale;
						z = p.z() + p.z() * Scale;
					}

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
					y = p.y();
					
					if (y > Upper)
					{
						Scale = TaAmount * Upper / (y_max - y_min);

						x = p.x() + p.x() * Scale;
						z = p.z() + p.z() * Scale;
					}

					else if (y < Lower)
					{
						Scale = TaAmount * Lower / (y_max - y_min);

						x = p.x() + p.x() * Scale;
						z = p.z() + p.z() * Scale;
					}

					else
					{
						Scale = TaAmount * p.y() / (y_max - y_min);

						x = p.x() + p.x() * Scale;
						z = p.z() + p.z() * Scale;
					}

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
					z = p.z();
					
					if (z > Upper)
					{
						Scale = TaAmount * Upper / (z_max - z_min);

						x = p.x() + p.x() * Scale;
						y = p.y() + p.y() * Scale;
					}

					else if (z < Lower)
					{
						Scale = TaAmount * Lower / (z_max - z_min);

						x = p.x() + p.x() * Scale;
						y = p.y() + p.y() * Scale;
					}

					else
					{
						Scale = TaAmount * p.z() / (z_max - z_min);

						x = p.x() + p.x() * Scale;
						y = p.y() + p.y() * Scale;
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