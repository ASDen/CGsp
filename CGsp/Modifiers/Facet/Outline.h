class Outline : Modifier
{
public:
	int Fnum;
	AnimatablePropery<double,ConstantInterpolator> OutlineAmount;

	Outline(int FaceNum ,double OAmount) : Fnum(FaceNum),OutlineAmount(OAmount)
	{}

	void Do(Polyhedron &P)
	{
		Plane_const_iterator px=P.planes_begin();
		Facet_iterator     iter=P.facets_begin ();
		Eigen::Transform3d t;

		std::advance (iter,Fnum);
		std::advance (px  ,Fnum);
		Point_3 o=iter->facet_begin()->vertex()->point(); //arbitary face vertex
		
		t.setIdentity();
		t.scale(OutlineAmount.val);
		Aff3 aff= CalcExecInFaceCoordSys(t,iter,px,o);
		ApplyTransformOnFace(aff,iter);
	}
};