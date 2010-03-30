#pragma once

class CGSP_CC PhysicsEventHandler : public BaseEventHandler
{
public:

	PhysicsEventHandler(BaseManager* bm) : BaseEventHandler(bm)
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
				}
			}
		}
		return false;
	}

	void shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);

};