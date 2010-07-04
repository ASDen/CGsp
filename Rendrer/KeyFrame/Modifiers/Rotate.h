class CGSP_CC Rotate : public KeyFrameModifier
{
public:
	
	AnimatablePropery<double,Interpolator> ax;
	AnimatablePropery<double,Interpolator> ay;
	AnimatablePropery<double,Interpolator> az;

	inline void AddThem()
	{
		props.push_back(&ax);
		props.push_back(&ay);
		props.push_back(&az);
	}

	Rotate() : ax(0),ay(0),az(0)
	{
		AddThem();
	}

	Rotate(double val) : ax(val),ay(val),az(val)
	{
		AddThem();
	}

	Rotate(double x,double y, double z) : ax(x),ay(y),az(z)
	{
		AddThem();
	}

	void Do(osg::PositionAttitudeTransform* Pn)
	{
		Pn->setAttitude( osg::Quat(ax.val,osg::X_AXIS,ay.val,osg::Y_AXIS,az.val,osg::Z_AXIS) );
	}
};