class CGSP_CC XCapsule 
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn, NxReal Height, NxReal Rad)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;
		
		NxCapsuleShapeDesc CapsuleDesc;
		CapsuleDesc.radius = Rad;
		CapsuleDesc.height = Height;
		
		CapsuleDesc.localPose.t = NxVec3(0, 0, 0);
		
		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&CapsuleDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the box
			(
			Pn->Position.x() + 2.0,
			Pn->Position.z() + 2.0,
			Pn->Position.y() + 2.0
			);

		return scene->createActor(ActorDesc);
	}
};