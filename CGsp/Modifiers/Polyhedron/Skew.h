class Skew : public Modifier
{
public:
	double SkAmount;
	Point_3 Center;
	Axis SkAxis;
	double Upper;
	double Lower;

	Skew(double SAmount,Point_3 C, Axis SAxis, double max, double min) : SkAmount(SAmount),Center(C),SkAxis(SAxis),Upper(max),Lower(min)
	{}

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

		double x_c = Center.x();
		double y_c = Center.y();
		double z_c = Center.z();
		
		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i)
		{
			Point_3 p = i->point();
			switch (SkAxis)
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

		switch (SkAxis)
		{
		case X_ax:
			{
				x_max -= x_c;
				x_min -= x_c;

				if (x_max > Upper)
					x_max = Upper;
				if (x_min < Lower)
					x_min = Lower;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					x = p.x();
					y = p.y();

					if (x > Upper)
					{
						Scale = SkAmount * Upper / (Upper - Lower);

						z = p.z() - Scale;
					}

					else if (x < Lower)
					{
						Scale = SkAmount * Lower / (Upper - Lower);

						z = p.z() - Scale;
					}

					else
					{
						Scale = SkAmount * p.x() / (Upper - Lower);

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

				if (y_max > Upper)
					y_max = Upper;
				if (y_min < Lower)
					y_min = Lower;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();
					z = p.z();

					if (y > Upper)
					{
						Scale = SkAmount * Upper / (Upper - Lower);

						x = p.x() + Scale;
					}

					else if (y < Lower)
					{
						Scale = SkAmount * Lower / (Upper - Lower);

						x = p.x() + Scale;
					}

					else
					{
						Scale = SkAmount * p.y() / (Upper - Lower);

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

				if (z_max > Upper)
					z_max = Upper;
				if (z_min < Lower)
					z_min = Lower;

				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();
					y = p.y();
					z = p.z();

					if (z > Upper)
					{
						Scale = SkAmount * Upper / (Upper - Lower);

						x = p.x() + Scale;
					}

					else if (z < Lower)
					{
						Scale = SkAmount * Lower / (Upper - Lower);

						x = p.x() + Scale;
					}

					else
					{
						Scale = SkAmount * p.z() / (Upper - Lower);

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