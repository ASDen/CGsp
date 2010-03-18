
class KeyFrameUpdateCallBack : public BaseUpdateCallBack
{
public:

	static int fnum;
	
	KeyFrameUpdateCallBack(PolyhedronNode* P):BaseUpdateCallBack(P)
	{}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		Pn->P->UpdateAtFrame(fnum);
		Pn->dirtyBound();
		traverse(node, nv);
	}

};
int KeyFrameUpdateCallBack::fnum;