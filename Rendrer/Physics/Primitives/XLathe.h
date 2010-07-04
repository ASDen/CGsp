class CGSP_CC XLathe
{
public:

	static NxActor* Construct(NxScene* scene,NxPhysicsSDK* gPhysicsSDK,PolyhedronNode* Pn,bool StaticObj = false)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;

		Lathe_3* Obj = dynamic_cast<Lathe_3*>(Pn->P);

		int Length = Obj->Arr_Points.size() - 1;
		int Side = (Obj->R_Angle < 360)? (Obj->Seg + 1) : Obj->Seg;
		
		// Create descriptor for convex mesh
		NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc[Length * Side];
		NxConvexShapeDesc* convexShapeDesc = new NxConvexShapeDesc[Length * Side];
		
		NxActorDesc ActorDesc;

		NxU32 vertCount = 6;
		
 		for (int i = 0; i < Length * Side; i++)
		{
			Facet_iterator face = Pn->P->Mesh.facets_begin();
			for ( int j = 0; j < i; j++) 
			{
				face++;
			}

			NxVec3* verts = new NxVec3[vertCount];

			int h = 0;
			Halfedge_facet_circulator n = face->facet_begin();
			for ( int k = 0; k < 6; k++) 
			{
				verts[h++] = NxVec3( - n->vertex()->point().x(),n->vertex()->point().z(),n->vertex()->point().y());
				n++;
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