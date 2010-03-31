class CGSP_CC XTorus
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn,int Num)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;

		NxBoxShapeDesc* BoxDesc = new NxBoxShapeDesc[Num];
		
		NxActorDesc ActorDesc;

		Torus_3* Obj = dynamic_cast<Torus_3*>(Pn->P);
		NxReal Width, Length, Height, Radius;

		Width = Obj->radius2;
		Length = (Obj->radius1 + Obj->radius2) * std::sin(CGAL_PI / Num);
		Height = Obj->radius2;

		Radius = Obj->radius1 + Obj->radius2;
		
		for (int i = 0; i < Num; i++)
		{
			double x = Radius * std::sin(2 * i * CGAL_PI / Num);
			double z = Radius * std::cos(2 * i * CGAL_PI / Num);
		
			BoxDesc[i].dimensions = NxVec3( Length, Height, Width );
			BoxDesc[i].localPose.M.rotY(2 * i * CGAL_PI / Num);
			BoxDesc[i].localPose.t = NxVec3(x, 0, z);

			//std::cout<<BoxDesc.localPose.M;
		
			ActorDesc.shapes.pushBack(&BoxDesc[i]);
		}

		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the box
			(
			-Pn->Position.x(),
			 Pn->Position.z(),
			 Pn->Position.y()
			);

		return scene->createActor(ActorDesc);
	}
};