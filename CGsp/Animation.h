class Interpolator
{
public:

	/*
	template<class Type>
	static virtual void Interpolate(std::map<int,Type> KeyFrames,std::vector<Type>& FrameValues) = 0;
	*/
};

template<class Type,class Interp>
class AnimatablePropery
{
public:

	Type val;
	std::vector<Type> FrameValues;
	std::map<int,Type> KeyFrames;
	Interp InterpTech;
	int NumberOfFrames;

	AnimatablePropery(Type nval):val(nval),NumberOfFrames(0)
	{}
};

class FrameCreater
{
public:
	template< class Type , class Mod , class Interp >
		static void FillFrames(
				int SFrame,int EFrame,
				Type SVal,Type EVal,
				AnimatablePropery<Type,Interp> Mod::*Property,
				Mod& ModObject
				)
	{
		(ModObject.*Property).KeyFrames[SFrame]=SVal;
		(ModObject.*Property).KeyFrames[EFrame]=EVal;
		(ModObject.*Property).NumberOfFrames=std::max( EFrame , (ModObject.*Property).NumberOfFrames );
		
		Interp::Interpolate( ModObject.*Property );
	}
};