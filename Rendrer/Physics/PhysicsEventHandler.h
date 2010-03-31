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
		}
		return false;
	}

	void shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);

};