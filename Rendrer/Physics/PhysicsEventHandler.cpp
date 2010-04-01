#include "stdafx.h"
//#include "../RdInc.h"
#include "../osgData.h"
////#include "glutViewer.h"
#include "../BaseUpdate.h"
#include "../BaseManager.h"
#include "../BaseEventHandler.h"
//
////Physics
//#include "../Physics/Primitives/XBox.h"
//#include "../Physics/Primitives/XPlane.h"
#include "../Physics/Primitives/XSphere.h"
//
#include "../Physics/PhysicsUpdate.h"
#include "../Physics/PhysicsEventHandler.h"
#include "../Physics/PhysicsManager.h"

void PhysicsEventHandler::shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{	
	osg::Viewport* viewport = viewer->getCamera()->getViewport();
	float mx = viewport->x() + (int)((float)viewport->width()*(ea.getXnormalized()*0.5f+0.5f));
	float my = viewport->y() + (int)((float)viewport->height()*(ea.getYnormalized()*0.5f+0.5f));

	osg::Matrix VPW = viewer->getCamera()->getViewMatrix() *
		viewer->getCamera()->getProjectionMatrix() *
		viewport->computeWindowMatrix();

	VPW.invert(VPW);

	osg::Vec3f point = osg::Vec3f(mx,my,0) * VPW;
	osg::Vec3f eye, center, up(0,0,1);
	double dist(1.0);

	viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up, dist);
	osg::Vec3f dir;
	dir = point - eye;			  
	dir.normalize();

	Sphere_3* sph = new Sphere_3(2,20);
	sph->Draw();

	PolyhedronNode* pnd = new PolyhedronNode(sph,osg::Vec3f(eye.x()-2, eye.y()-2, eye.z()-2) );
	pnd->Actor = XSphere::Construct(dynamic_cast<PhysicsManager*>(Manager)->gScene,pnd);
	pnd->WireFrame = false;

	Manager->PolyMan->AddPolyhedron<PhysicsManager>(pnd);
	NxVec3 _force(shootForce*NxVec3(-dir.x(), dir.z(), dir.y()));
	pnd->Actor->addForce(_force);

}