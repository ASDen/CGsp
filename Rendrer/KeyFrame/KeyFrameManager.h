
class KeyFrameManager : public BaseManager
{
public:
	osgPolyManager* Man;

	void InitOsg(int width=640,int height=480)
	{
		BaseManager::InitOsg();
		viewer->addEventHandler
			(new KeyFrameEventHandler(this));
	}

	void setPolyManager(osgPolyManager* m)
	{
		Man=m;
		viewer->setSceneData(Man->root);
	}

	void UpdateScene()
	{
		//update scence
		Man->UpdateFrame(fnum);
		std::cout<<"Frame #"<<fnum<<std::endl;
	}
};