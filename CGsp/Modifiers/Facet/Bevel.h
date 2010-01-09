class Bevel : Modifier
{
public:
	int Fnum;
	double ExAmount;
	double OutlineAmount;
	Type BeType;

	Bevel(int FaceNum, double EAmount, double OAmount, Type BType) : Fnum(FaceNum),ExAmount(EAmount),OutlineAmount(OAmount),BeType(BType)
	{}

	void Do(Polyhedron &P)
	{ 
		Extrude E(Fnum, ExAmount, BeType);
		E.Do(P);

		Outline O(P.size_of_facets()-1, OutlineAmount);
		O.Do(P);
	}
};