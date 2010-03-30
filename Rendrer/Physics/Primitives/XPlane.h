
class CGSP_CC XPlane 
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn)
	{
		NxPlaneShapeDesc planeDesc;
		//planeDesc.normal = NxVec3(0.0f,1.0f,0.0f);
		planeDesc.d = Pn->Position.z();
		NxActorDesc actorDesc;
		actorDesc.shapes.pushBack(&planeDesc);
		return scene->createActor(actorDesc);	
	}
};