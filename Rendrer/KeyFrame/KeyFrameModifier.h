class CGSP_CC KeyFrameModifier : public Modifier
{
public:

	virtual void Do(osg::PositionAttitudeTransform* Pn) = 0;

	void Do(Polyhedron&) {}

	void DoAtFrame(osg::PositionAttitudeTransform* P,int Frame)
	{
		propsVector::iterator i;
		for(i=props.begin();i!=props.end();i++)
		{
			(*i)->SetAtFrame(Frame);
		}
		Do(P);
	}
};