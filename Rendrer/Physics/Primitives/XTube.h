class CGSP_CC XTube
{
public:

	static NxActor* Construct(NxScene* scene,NxPhysicsSDK* gPhysicsSDK,PolyhedronNode* Pn,bool StaticObj = false)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;

		Tube_3* Obj = dynamic_cast<Tube_3*>(Pn->P);

		// Create descriptor for convex mesh
		NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc[Obj->side_Seg*Obj->height_Seg];
		NxConvexShapeDesc* convexShapeDesc = new NxConvexShapeDesc[Obj->side_Seg*Obj->height_Seg];
		
		NxActorDesc ActorDesc;

		NxU32 vertCount = 12;
		
 		for (int i = 0; i < Obj->side_Seg*Obj->height_Seg; i++)
		{
			Facet_iterator face = Pn->P->Mesh.facets_begin();
			for ( int j = 0; j < i; j++) 
			{
				face++;
			}

			NxVec3* verts = new NxVec3[vertCount];

			int h = 0;
			for ( int j = 0; j < 2; j++) 
			{
				Halfedge_facet_circulator n = face->facet_begin();
				for ( int k = 0; k < 6; k++) 
				{
					verts[h++] = NxVec3( - n->vertex()->point().x(),n->vertex()->point().z(),n->vertex()->point().y());
					n++;
				}
				int u = i / Obj->side_Seg;
			
				for (int l = 0 ; l < 2*Obj->height_Seg*Obj->side_Seg + Obj->cap_Seg*Obj->side_Seg - (2*u+1)*Obj->side_Seg; l++)
				{
					face++;
				}
			}

			convexDesc[i].numVertices          = vertCount;
			convexDesc[i].pointStrideBytes     = sizeof(NxVec3);
			convexDesc[i].points               = verts;
			convexDesc[i].flags                = NX_CF_COMPUTE_CONVEX;

			convexShapeDesc[i].localPose.t		= NxVec3(0,0,0);
			NxInitCooking();
			// Cooking from memory
			MemoryWriteBuffer buf;
			bool status = NxCookConvexMesh(convexDesc[i], buf);

			NxConvexMesh *pMesh			= gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
			convexShapeDesc[i].meshData	= pMesh;
			NxCloseCooking();

			pMesh->saveToDesc(convexDesc[i]);
		
			ActorDesc.shapes.pushBack(&convexShapeDesc[i]);
		}

		ActorDesc.body			= (StaticObj)?NULL : &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the torus
			(
			-Pn->Position.x(),
			 Pn->Position.z(),
			 Pn->Position.y()
			);

		return scene->createActor(ActorDesc);
	}
};