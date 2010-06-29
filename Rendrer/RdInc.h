#pragma once
#pragma message ("In Renderer Inlcudes ")

#define PHYSX_USE_VRD 1

#include "KeyFrame/KeyFrameModifier.h"

#include "osgData.h"
//#include "glutViewer.h"
#include "BaseUpdate.h"
#include "BaseManager.h"
#include "BaseEventHandler.h"

//KeyFramed
#include "KeyFrame/KeyFrameUpdate.h"
#include "KeyFrame/KeyFrameEventHandler.h"
#include "KeyFrame/KeyFrameManager.h"

#include "KeyFrame/Modifiers/Translate.h"
#include "KeyFrame/Modifiers/Scale.h"
#include "KeyFrame/Modifiers/Rotate.h"
#include "KeyFrame/Modifiers/RotateAA.h"

//Physics
#include "Physics/Primitives/XBox.h"
#include "Physics/Primitives/XCapsule.h"
#include "Physics/Primitives/XConvex.h"
#include "Physics/Primitives/XPlane.h"
#include "Physics/Primitives/XSphere.h"
#include "Physics/Primitives/XSpring.h"
#include "Physics/Primitives/XTorus.h"
#include "Physics/Primitives/XTube.h"
#include "Physics/Primitives/XCloth.h"

#include "Physics/UpdateCallback/RigidBodyUpdate.h"
#include "Physics/UpdateCallback/ClothUpdate.h"
#include "Physics/PhysicsEventHandler.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Managers/RigidBodyManager.h"
#include "Physics/Managers/ClothManager.h"

