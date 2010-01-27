class Bevel : public Modifier
{
public:
	int Fnum;
	double ExAmount;
	double OutlineAmount;
	
	Bevel(int FaceNum, double EAmount, double OAmount) : Fnum(FaceNum),ExAmount(EAmount),OutlineAmount(OAmount)
	{}

	void Do(Polyhedron &P)
	{ 
		Extrude E(Fnum, ExAmount);
		E.Do(P);

		Outline O(P.size_of_facets()-1, OutlineAmount);
		O.Do(P);
	}
};