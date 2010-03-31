class CGSP_CC XCapsule 
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn)
	{
		Capsule_3* Obj = dynamic_cast<Capsule_3*>(Pn->P);
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;
		
		NxCapsuleShapeDesc CapsuleDesc;
		CapsuleDesc.radius = Obj->radius;
		CapsuleDesc.height = Obj->height / 2;
		
		CapsuleDesc.localPose.t = NxVec3(0, 0, 0);
		
		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&CapsuleDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the capsule
			(
			-Pn->Position.x(),
			 Pn->Position.z(),
			 Pn->Position.y()
			);

		return scene->createActor(ActorDesc);
	}
};