#pragma once

class CGSP_CC BaseManager
{
public:
	//Data..
	osgPolyManager* PolyMan;
	//Viewing Actors..
	osg::ref_ptr<osgViewer::Viewer> viewer;
	osg::ref_ptr<osg::GraphicsContext> gc;
	osg::ref_ptr<osg::Camera> camera;
	//Frames..
	int fnum;
	float T0,Telp;
	float rate;
	bool isPaused;

	virtual void InitOsg(int width=640,int height=480) 
	{
		viewer= new osgViewer::Viewer;

		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 100;
		traits->y = 100;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		if (gc.valid())
		{
			gc->setClearColor(osg::Vec4f(0.8f,0.8f,0.8f,1.0f));
			gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, width, height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);

		viewer->addSlave(camera.get());
		viewer->getCamera()->setViewport(0,0,width/2,height);

		osgGA::TrackballManipulator* manip = new osgGA::TrackballManipulator();
		//manip->setHomePosition(osg::Vec3f(10,10,10), osg::Vec3f(0,0,0), osg::Vec3f(0,0,1));
		viewer->setCameraManipulator( manip);
		viewer->getCamera()->getOrCreateStateSet()->setDataVariance(osg::Object::DYNAMIC );
		//viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);

		fnum = 0 ;
		T0 = Telp = 0;
		rate = 20 ;
		isPaused = false;
	}

	void setPolyManager(osgPolyManager* m)
	{
		PolyMan=m;
		viewer->setSceneData(PolyMan->root);
	}

	void DisplayLoop()
	{
		viewer->realize();
		while (!viewer->done())
		{
			viewer->advance(USE_REFERENCE_TIME);
			viewer->eventTraversal();
			//do we need to update scene ?
			Telp+=viewer->elapsedTime()-T0;
			T0=viewer->elapsedTime();
			if(Telp>=1/rate && !isPaused)
			{
				//Telp = 0;
				//Telp-=1/rate;
				Telp = fmod(Telp,1/rate);
				UpdateScene();
				std::cout<<"Frame #"<<fnum<<std::endl;
				viewer->updateTraversal();
				fnum++;
				
			}
			if(isPaused)
				viewer->updateTraversal();
			viewer->renderingTraversals();
		}
	}

	virtual void UpdateScene() = 0 ;

};