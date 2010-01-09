class Bridge : public Modifier
{
public:
	int Face1;
	int Face2;

	Bridge(int F1, int F2) : Face1(F1),Face2(F2)
	{}

	void Do(Polyhedron &P)
	{
		if(Face1 < 0 || Face1 > P.size_of_facets () || Face2 < 0 || Face2 > P.size_of_facets ())
			return;

		  
	}
};