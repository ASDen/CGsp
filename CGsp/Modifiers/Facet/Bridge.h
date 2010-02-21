class Bridge : public Modifier
{
public:
	AnimatablePropery<int,Interpolator> Face1;
	AnimatablePropery<int,Interpolator> Face2;

	Bridge(int F1, int F2) : Face1(F1),Face2(F2)
	{
		props.push_back(&Face1);
		props.push_back(&Face2);
	}

	void Do(Polyhedron &P)
	{
		if(Face1.val < 0 || Face1.val > P.size_of_facets () || Face2.val < 0 || Face2.val > P.size_of_facets ())
			return;
	}
};