
class KeyFrameManager : public BaseManager
{
public:
	typedef KeyFrameUpdateCallBack UpdateCallback;

	void InitOsg(int width=640,int height=480)
	{
		BaseManager::InitOsg();
		viewer->addEventHandler(new KeyFrameEventHandler(this));
	}

	void UpdateScene()
	{
		//update scence
		//Man->UpdateFrame(fnum);
		KeyFrameUpdateCallBack::fnum = fnum ;
		std::cout<<"Frame #"<<fnum<<std::endl;
	}
};