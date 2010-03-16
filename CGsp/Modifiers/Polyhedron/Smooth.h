class Smooth : public Modifier
{
public:
	AnimatablePropery<int,Interpolator> S_Deg;

	Smooth() : S_Deg(1)
	{
		props.push_back(&S_Deg);
	}

	Smooth(double SD) : S_Deg(SD)
	{
		props.push_back(&S_Deg);
	}

	void Do(Polyhedron &Start)
	{
		CGAL::Subdivision_method_3::CatmullClark_subdivision(Start,S_Deg.val);
	}
};