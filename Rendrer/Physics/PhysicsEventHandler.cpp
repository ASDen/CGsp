#include "stdafx.h"
#include "../RdInc.h"

void PhysicsEventHandler::shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{	
	osg::Vec3f point = getOsgMousePoint(ea,viewer);
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

void PhysicsEventHandler::pick(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{
	osg::Vec3f point = getOsgMousePoint(ea,viewer);
	osg::Vec3f eye, center, up;
	double dist(4.0);

	viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up, dist);
	NxRay ray = convertToCameraDir(eye, point);

	NxRaycastHit _hit;
	NxVec3 gDir = ray.dir;
	NxShape* _shape = dynamic_cast<PhysicsManager*>(Manager)->gScene->raycastClosestBounds(ray, NX_DYNAMIC_SHAPES, _hit);
	if (_shape && _hit.shape)
	{
		NxActor* _selection = &_shape->getActor();
		NxVec3 _force( shootForce * ray.dir);			
		_selection->addForce(_force);
		//_selection->addForceAtPos( _force, NxVec3(_hit.worldImpact));
	}
}