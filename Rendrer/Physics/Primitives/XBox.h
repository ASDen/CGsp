
class XBox 
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;
		
		NxBoxShapeDesc BoxDesc;
		Kernel::Iso_cuboid_3 c3 = CGAL::bounding_box
			(Pn->P->ModifiedMesh.points_begin (), Pn->P->ModifiedMesh.points_end ());
		BoxDesc.dimensions = NxVec3( (c3.xmax()-c3.xmin())/2.0, (c3.ymax()-c3.ymin())/2.0, (c3.zmax()-c3.zmin())/2.0 );
		BoxDesc.localPose.t = NxVec3(0, 0, 0);
		//std::cout<<Pn->Position.x()+(c3.xmax()-c3.xmin())/2.0<<" "<<Pn->Position.z()+(c3.ymax()-c3.ymin())/2.0
		//	<<" "<<Pn->Position.y()+(c3.zmax()-c3.zmin())/2.0<<std::endl;
		
		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&BoxDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the box
			(
			Pn->Position.x()+(c3.xmax()-c3.xmin())/2.0,
			Pn->Position.z()+(c3.ymax()-c3.ymin())/2.0,
			Pn->Position.y()+(c3.zmax()-c3.zmin())/2.0
			);

		return scene->createActor(ActorDesc);
	}
};