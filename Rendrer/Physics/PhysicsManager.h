#pragma once

class CGSP_CC PhysicsManager : public BaseManager
{
public:
	typedef PhysicsUpdateCallBack UpdateCallback;

	NxPhysicsSDK*     gPhysicsSDK;
	NxScene*          gScene;
	UserAllocator*	  gAllocator;
	NxVec3            gDefaultGravity;
	ErrorStream       gErrorStream;
	bool              gHardwareSimulation;
	bool              gCookingInitialized;


	bool InitNx(void)
	{
		// Initialize PhysicsSDK
		NxPhysicsSDKDesc desc;
		NxSDKCreateError errorCode = NXCE_NO_ERROR;
		gAllocator = new UserAllocator;
		gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, gAllocator, &gErrorStream, desc, &errorCode);
		if(gPhysicsSDK == NULL) 
		{
			//printf("\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK.\n\n", errorCode, getNxSDKCreateError(errorCode));
			return false;
		}

#if PHYSX_USE_VRD
		// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
		if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger() && !gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected())
			gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

		NxHWVersion hwCheck = gPhysicsSDK->getHWVersion();
		if (hwCheck == NX_HW_VERSION_NONE) 
		{
			printf("\nWarning: Unable to find a PhysX card...\n\n");
			gHardwareSimulation = false;
		}

		if (!gCookingInitialized)
		{
			gCookingInitialized = true;
			if (!InitCooking(NULL, &gErrorStream)) 
			{
				printf("\nError: Unable to initialize the cooking library.\n\n");
				return false;
			}
		}
		// Set the physics parameters
		gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

		// Set the debug visualization parameters
		gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
		//gPhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_MESH, 1);
		//gPhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_VALIDBOUNDS, 1);

		// setup the scene
		NxSceneDesc sceneDesc;
		sceneDesc.gravity = gDefaultGravity;
		gScene = gPhysicsSDK->createScene(sceneDesc);
		if(gScene == NULL) 
		{
			printf("\nError: Unable to create a PhysX scene.\n\n");
			return false;
		}

		// Create the default material
		NxMaterialDesc m; 
		m.restitution = 0.5f;
		m.staticFriction = 0.2f;
		m.dynamicFriction= 0.2f;
		NxMaterial * mat = gScene->getMaterialFromIndex(0);
		mat->loadFromDesc(m); 

		//gScene->setTiming(1.0f/60.0f, 5, NX_TIMESTEP_FIXED);

		return true;
	}

	// releases physx
	void ReleaseNx()
	{
		if (gCookingInitialized) CloseCooking();
		gCookingInitialized = false;
		if(gPhysicsSDK != NULL)
		{
			if(gScene != NULL) gPhysicsSDK->releaseScene(*gScene);
			gScene = NULL;
			NxReleasePhysicsSDK(gPhysicsSDK);
			gPhysicsSDK = NULL;
		}
	}

	// physx reset
	void ResetNx()
	{
		ReleaseNx();
		if (!InitNx()) exit(0);
	}

	void InitOsg(int width=640,int height=480)
	{
		BaseManager::InitOsg();
		viewer->addEventHandler(new PhysicsEventHandler(this));
		InitNx();
	}

	NxActor* AddActor(PolyhedronNode* Pn)
	{
		return gScene->createActor((Pn->ActorDesc));
	}

	void UpdateScene()
	{
		//update scence
		gScene->simulate(1/rate);
		gScene->flushStream();
		gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);	
		std::cout<<"Frame #"<<fnum<<std::endl;
	}

	PhysicsManager()
	{
		gCookingInitialized = false;
		gHardwareSimulation = true;
		gDefaultGravity = NxVec3(0,-9.8,0);
	}
};
