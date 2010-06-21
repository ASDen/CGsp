class CGSP_CC Bevel : public Modifier
{
public:
	AnimatablePropery<int,Interpolator> Fnum;
	AnimatablePropery<double,Interpolator> ExAmount;
	AnimatablePropery<double,Interpolator> OutlineAmount;

	Bevel(int FaceNum) : Fnum(FaceNum),ExAmount(5),OutlineAmount(1)
	{
		props.push_back(&Fnum);
		props.push_back(&ExAmount);
		props.push_back(&OutlineAmount);
	}

	Bevel(int FaceNum, double EAmount, double OAmount) : Fnum(FaceNum),ExAmount(EAmount),OutlineAmount(OAmount)
	{
		props.push_back(&Fnum);
		props.push_back(&ExAmount);
		props.push_back(&OutlineAmount);
	}

	void Do(Polyhedron &P)
	{ 
		Extrude E(Fnum.val, ExAmount.val);
		E.Do(P);

		Outline O(P.size_of_facets()-1, OutlineAmount.val);
		O.Do(P);
	}
};