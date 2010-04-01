

class AnimatableProperyParent
{
public:
	virtual void SetAtFrame(int Frame)=0;
};

template<class Type,class Interp>
class AnimatablePropery : public AnimatableProperyParent
{
public:

	Type val;
	std::vector<Type> FrameValues;
	std::map<int,Type> KeyFrames;
	Interp InterpTech;
	int NumberOfFrames;

	AnimatablePropery(Type nval):val(nval),NumberOfFrames(1)
	{
		FrameValues.push_back(val);
	}

	void SetAtFrame(int Frame)
	{
		val=FrameValues[std::min(Frame,NumberOfFrames-1)];
	}
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
		(ModObject.*Property).NumberOfFrames=std::max( EFrame+1 , (ModObject.*Property).NumberOfFrames );
		
		Interp::Interpolate( ModObject.*Property );
	}
};