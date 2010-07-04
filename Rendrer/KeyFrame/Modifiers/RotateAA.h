class CGSP_CC RotateAA : public KeyFrameModifier
{
public:
	
	AnimatablePropery<double,Interpolator> ax;
	AnimatablePropery<double,Interpolator> ay;
	AnimatablePropery<double,Interpolator> az;

	AnimatablePropery<double,Interpolator> g;

	inline void AddThem()
	{
		props.push_back(&ax);
		props.push_back(&ay);
		props.push_back(&az);
		props.push_back(&g);
	}

	RotateAA() : ax(0),ay(0),az(0),g(0)
	{
		AddThem();
	}

	RotateAA(double a,double v) : ax(a),ay(a),az(a),g(v)
	{
		AddThem();
	}

	RotateAA(double x,double y, double z,double a) : ax(x),ay(y),az(z),g(a)
	{
		AddThem();
	}

	void Do(osg::PositionAttitudeTransform* Pn)
	{
		Pn->setAttitude( osg::Quat(g.val,osg::Vec3f(ax.val,ay.val,az.val)) );
	}
};