#pragma once

class CGSP_CC RigidBodyUpdateCallBack : public BaseUpdateCallBack
{
public:

	//NxActor* _actor;
	float m[16];

	RigidBodyUpdateCallBack(PolyhedronNode* P):BaseUpdateCallBack(P)
	{}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		// get the transformation matrix
		Pn->RigidActor->getGlobalPose().getColumnMajor44(m);
		// quaternion
		NxQuat _quat = Pn->RigidActor->getGlobalOrientationQuat();
		// now the graphic object
		osg::PositionAttitudeTransform* _mat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
		if (_mat)
		{
			// apply the position + orientation to the graphic object
			_mat->setPosition(osg::Vec3f(-m[12],m[14],m[13]));
			_mat->setAttitude( osg::Quat(-_quat.x, _quat.z, _quat.y, _quat.w) );
		}

		traverse(node, nv);
	}
};