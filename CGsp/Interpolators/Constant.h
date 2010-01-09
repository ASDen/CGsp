class ConstantInterpolator : public Interpolator
{
public:
	template<class Type>
	static void Interpolate( AnimatablePropery<Type,ConstantInterpolator>& prop)
	{
		std::map<int,Type>::iterator i;
		prop.FrameValues.assign(prop.NumberOfFrames,Type());
		for(i=prop.KeyFrames.begin();i!=prop.KeyFrames.end();i++)
		{
			std::fill(prop.FrameValues.begin()+i->first,prop.FrameValues.end(),i->second);
		}
	}
};