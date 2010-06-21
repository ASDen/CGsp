

class CGSP_CC AnimatableProperyParent
{
public:
	virtual void SetAtFrame(int Frame)=0;
};

template<class Type,class Interp>
class CGSP_CC AnimatablePropery : public AnimatableProperyParent
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
		KeyFrames[0] = val;
	}

	void SetAtFrame(int Frame)
	{
		val=FrameValues[std::min(Frame,NumberOfFrames-1)];
	}
};

class CGSP_CC FrameCreater
{
public:
	template< class Type , class Mod , class Interp >
		static void FillFrames(
				int SFrame,Type SVal,
				AnimatablePropery<Type,Interp> Mod::*Property,
				Mod& ModObject
				)
	{
		(ModObject.*Property).KeyFrames[SFrame]=SVal;
		(ModObject.*Property).NumberOfFrames=std::max( SFrame+1 , (ModObject.*Property).NumberOfFrames );
		
		Interp::Interpolate( ModObject.*Property );
	}


	template< class Type , class Mod , class Interp >
		static void FillFramesDouble(
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