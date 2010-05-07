#pragma once

class CGSP_CC PhysicsEventHandler : public BaseEventHandler
{
public:

	float shootForce;

	PhysicsEventHandler(BaseManager* bm) : BaseEventHandler(bm),shootForce(1000000.0)
	{}

	bool handle(const osgGA::GUIEventAdapter& gea, osgGA::GUIActionAdapter& adp)
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&adp);
		if (!viewer) return false;

		BaseEventHandler::handle(gea,adp);
		switch(gea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYUP):
			{
				switch (gea.getKey())
				{
				case 's':
					shoot(gea,viewer);
					break;
				case 'a':
					shootForce += 50000;
					std::cout<<"Force Now :"<<shootForce<<std::endl;
					break;
				case 'd':
					shootForce -= 50000;
					break;
				}
			}
		case(osgGA::GUIEventAdapter::DOUBLECLICK): 
			{
				if (gea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					pick(gea,viewer);					  
				}  
				return true;
			}
		}
		return false;
	}

	NxRay convertToCameraDir(osg::Vec3f eye, osg::Vec3f center)
	{

		osg::Vec3f dir = center - eye;

		NxRay _worldRay;
		NxVec3 gDir( - dir.x(), dir.z(), dir.y());
		gDir.normalize();

		_worldRay.orig.x	= - eye.x();
		_worldRay.orig.y	= eye.z();
		_worldRay.orig.z	= eye.y();

		_worldRay.dir	= gDir;
		_worldRay.dir.normalize();

		return _worldRay;

	}

	osg::Vec3f getOsgMousePoint(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
	{
		osg::Viewport* viewport = viewer->getCamera()->getViewport();
		float mx = viewport->x() + (int)((float)viewport->width()*(ea.getXnormalized()*0.5f+0.5f));
		float my = viewport->y() + (int)((float)viewport->height()*(ea.getYnormalized()*0.5f+0.5f));

		osg::Matrix VPW = viewer->getCamera()->getViewMatrix() *
			viewer->getCamera()->getProjectionMatrix() *
			viewport->computeWindowMatrix();

		VPW.invert(VPW);

		return osg::Vec3f(mx,my,0) * VPW;
	}

	void shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);

	void pick(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);

};