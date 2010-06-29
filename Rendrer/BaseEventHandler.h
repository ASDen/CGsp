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
				case 'p':
					Manager->isPaused=!Manager->isPaused;
					if(Manager->isPaused)
						std::cout<<"Paused"<<std::endl;
					else
						std::cout<<"Returned !!"<<std::endl;
					break;
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