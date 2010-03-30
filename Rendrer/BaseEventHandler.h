#pragma once

class CGSP_CC BaseEventHandler : public osgGA::GUIEventHandler
{
public:
	BaseManager* Manager;

	BaseEventHandler(BaseManager* bm):Manager(bm)
	{}

	virtual bool handle(const osgGA::GUIEventAdapter& gea, osgGA::GUIActionAdapter& adp)
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&adp);
		if (!viewer) return false;

		switch(gea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYUP):
			{
				switch (gea.getKey())
				{
				case '+':
					Manager->rate++;
					std::cout<<"Frame Rate : "<<Manager->rate<<std::endl;
					break;
				case '-':
					Manager->rate--;
					std::cout<<"Frame Rate : "<<Manager->rate<<std::endl;
					break;
				}
			}
		}
		return false;
	}

};