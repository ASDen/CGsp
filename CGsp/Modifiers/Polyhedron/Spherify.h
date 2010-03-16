class Spherify : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> SphAmount;

	Spherify() : SphAmount(100)
	{
		props.push_back(&SphAmount);
	}

	Spherify(double SAmount) : SphAmount(SAmount)
	{
		props.push_back(&SphAmount);
	}

	void Do(Polyhedron &P)
	{
		if (SphAmount.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double Radius = 0, Rad, Scale;
		double x, y, z;

		Point_3 Center = calc_Center(P);
		double x_c = Center.x();
		double y_c = Center.y();
		double z_c = Center.z();

		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i)
		{
			Point_3 p = i->point();

			Rad = sqrt( pow( (p.x() - x_c), 2) + pow( (p.y() - y_c), 2) + pow( (p.z() - z_c), 2));

			if( Radius < Rad)
				Radius = Rad;
		}

		t.setIdentity();
		org=Eigen::Vector3d(x_c,y_c,z_c);
		t.pretranslate(-org);
		ApplyTransformToPolyhedron(P,t);

		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
		{
			Point_3 p = i->point();

			Rad = sqrt( pow(p.x(), 2) + pow(p.y(), 2) + pow(p.z(), 2));
			Scale = SphAmount.val * (Radius - Rad) / 100;
			
			x = p.x() + Scale * p.x() / Rad;
			y = p.y() + Scale * p.y() / Rad;
			z = p.z() + Scale * p.z() / Rad;
			
			i->point() = Point_3 (x ,y ,z);
		}

		t.setIdentity();
		t.pretranslate(org);
		ApplyTransformToPolyhedron(P,t);
	}
};