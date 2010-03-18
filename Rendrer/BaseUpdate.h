
class BaseUpdateCallBack : public osg::NodeCallback
{
public:

	PolyhedronNode* Pn;

	BaseUpdateCallBack(PolyhedronNode* P):Pn(P)
	{}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)=0;
};