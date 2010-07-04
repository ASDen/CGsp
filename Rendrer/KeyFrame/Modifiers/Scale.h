class CGSP_CC Scale : public KeyFrameModifier
{
public:
	
	AnimatablePropery<double,Interpolator> sx;
	AnimatablePropery<double,Interpolator> sy;
	AnimatablePropery<double,Interpolator> sz;

	inline void AddThem()
	{
		props.push_back(&sx);
		props.push_back(&sy);
		props.push_back(&sz);
	}

	Scale() : sx(0),sy(0),sz(0)
	{
		AddThem();
	}

	Scale(double val) : sx(val),sy(val),sz(val)
	{
		AddThem();
	}

	Scale(double x,double y, double z) : sx(x),sy(y),sz(z)
	{
		AddThem();
	}

	void Do(osg::PositionAttitudeTransform* Pn)
	{
		Pn->setScale( osg::Vec3d(sx.val,sy.val,sz.val) );
	}
};