class CGSP_CC Morph : public Modifier
{
public:
	Polyhedron End;
	AnimatablePropery<double,Interpolator> Morph_Rate;

	Morph(Polyhedron E) : End(E),Morph_Rate(10)
	{
		props.push_back(&Morph_Rate);
	}

	Morph(Polyhedron E,double MR) : End(E),Morph_Rate(MR)
	{
		props.push_back(&Morph_Rate);
	}

	void Do(Polyhedron &Start)
	{
		if(Start.size_of_vertices() <= End.size_of_vertices())
		{
			double x, y, z;
			Vertex_iterator j = End.vertices_begin();
			for (Vertex_iterator i = Start.vertices_begin(); i != Start.vertices_end(); i++)
			{
				Point_3 P_start = i->point();
				Point_3 P_end = j->point();
				
				x = P_start.x() + Morph_Rate.val * (P_end.x() - P_start.x()) / 100;
				y = P_start.y() + Morph_Rate.val * (P_end.y() - P_start.y()) / 100;
				z = P_start.z() + Morph_Rate.val * (P_end.z() - P_start.z()) / 100;
				
				i->point() = Point_3(x, y, z);
				j++;
			}
		}
	}
};