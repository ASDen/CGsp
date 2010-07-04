template<class Prim,class T>
class CGSP_CC Property : public Modifier
{
public:
	AnimatablePropery<T,Interpolator> PolyP;
	Prim* PrimM;
	T Prim::*Prop;
	bool commit;

	Property(T Prim::*pr , Prim* P , T Pval) : Prop(pr),PrimM(P),PolyP(Pval),commit(false)
	{
		props.push_back(&PolyP);
	}

	void Do(Polyhedron &P)
	{
		(PrimM->*Prop) = PolyP.val;
		if(commit)
			P = PrimM->Draw();

	}

};
template class Property<Box_3,double>;
template class Property<Box_3,int>;
template class Property<Capsule_3,double>;
template class Property<Capsule_3,int>;
template class Property<ChamferCyl_3,double>;
template class Property<ChamferCyl_3,int>;
template class Property<Cone_3,double>;
template class Property<Cone_3,int>;
template class Property<Cylinder_3,double>;
template class Property<Cylinder_3,int>;
template class Property<Lathe_3,double>;
template class Property<Lathe_3,int>;
template class Property<Plane_3,double>;
template class Property<Plane_3,int>;
template class Property<Pyramid_3,double>;
template class Property<Pyramid_3,int>;
template class Property<Sphere_3,double>;
template class Property<Sphere_3,int>;
template class Property<Spindle_3,double>;
template class Property<Spindle_3,int>;
template class Property<Spring_3,double>;
template class Property<Spring_3,int>;
template class Property<Torus_3,double>;
template class Property<Torus_3,int>;
template class Property<Tube_3,double>;
template class Property<Tube_3,int>;

typedef CGSP_CC Property<Box_3,double> BoxPropsD;
typedef CGSP_CC Property<Box_3,int> BoxPropsI;
typedef CGSP_CC Property<Capsule_3,double> CapsulePropsD;
typedef CGSP_CC Property<Capsule_3,int> CapsulePropsI;
typedef CGSP_CC Property<ChamferCyl_3,double> ChamferCylPropsD;
typedef CGSP_CC Property<ChamferCyl_3,int> ChamferCylPropsI;
typedef CGSP_CC Property<Cone_3,double> ConePropsD;
typedef CGSP_CC Property<Cone_3,int> ConePropsI;
typedef CGSP_CC Property<Cylinder_3,double> CylinderPropsD;
typedef CGSP_CC Property<Cylinder_3,int> CylinderPropsI;
typedef CGSP_CC Property<Lathe_3,double> LathePropsD;
typedef CGSP_CC Property<Lathe_3,int> LathePropsI;
typedef CGSP_CC Property<Plane_3,double> PlanePropsD;
typedef CGSP_CC Property<Plane_3,int> PlanePropsI;
typedef CGSP_CC Property<Pyramid_3,double> PyramidPropsD;
typedef CGSP_CC Property<Pyramid_3,int> PyramidPropsI;
typedef CGSP_CC Property<Sphere_3,double> SpherePropsD;
typedef CGSP_CC Property<Sphere_3,int> SpherePropsI;
typedef CGSP_CC Property<Spindle_3,double> SpindlePropsD;
typedef CGSP_CC Property<Spindle_3,int> SpindlePropsI;
typedef CGSP_CC Property<Spring_3,double> SpringPropsD;
typedef CGSP_CC Property<Spring_3,int> SpringPropsI;
typedef CGSP_CC Property<Torus_3,double> TorusPropsD;
typedef CGSP_CC Property<Torus_3,int> TorusPropsI;
typedef CGSP_CC Property<Tube_3,double> TubePropsD;
typedef CGSP_CC Property<Tube_3,int> TubePropsI;

typedef CGSP_CC Property<Lathe_3,double> LathePropsD;
typedef CGSP_CC Property<Lathe_3,int> LathePropsI;