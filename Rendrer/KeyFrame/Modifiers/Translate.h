class CGSP_CC Translate : public KeyFrameModifier
{
public:
	
	AnimatablePropery<double,Interpolator> tx;
	AnimatablePropery<double,Interpolator> ty;
	AnimatablePropery<double,Interpolator> tz;

	inline void AddThem()
	{
		props.push_back(&tx);
		props.push_back(&ty);
		props.push_back(&tz);
	}

	Translate() : tx(0),ty(0),tz(0)
	{
		AddThem();
	}

	Translate(double val) : tx(val),ty(val),tz(val)
	{
		AddThem();
	}

	Translate(double x,double y, double z) : tx(x),ty(y),tz(z)
	{
		AddThem();
	}

	void Do(osg::PositionAttitudeTransform* Pn)
	{
		Pn->setPosition( osg::Vec3d(tx.val,ty.val,tz.val) );
	}
};