
class CGSP_CC KeyFrameEventHandler : public BaseEventHandler
{
public:

	KeyFrameEventHandler(BaseManager* bm) : BaseEventHandler(bm)
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
				case 'r':
					Manager->fnum = 0;
					break;
				}
			}
		}
		return false;
	}

	void shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);

};