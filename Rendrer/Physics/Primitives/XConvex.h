class CGSP_CC XConvex
{
public:

	static void ConvexHullDirectReduce(Polyhedron& P,Polyhedron *CxP)
	{
		int threshold = 5;
		int oldF , newF;
		*CxP = P; // copy polyhedron before iterative convex hull construction
		oldF = P.size_of_facets();
		while(true)
		{
			CGAL::convex_hull_3(CxP->points_begin(),CxP->points_end(),*CxP);
			newF = CxP->size_of_facets();
			
			if(oldF - newF <threshold )
				break;
			oldF = newF;
		}
		std::cout<<"Reduced using Convex Hulls to "<<newF<<" Faces "<<std::endl;
	}

	static NxActor* Construct(NxScene* scene,NxPhysicsSDK* gPhysicsSDK,PolyhedronNode* Pn)
	{
		NxBodyDesc BodyDesc;
		Polyhedron *CxP = new Polyhedron;
		
		ConvexHullDirectReduce(Pn->P->Mesh,CxP);
		
		NxU32 vertCount = CxP->size_of_facets()*3;
		NxVec3 *verts=new NxVec3[vertCount];
		int h=0;
		for (  Facet_iterator i = CxP->facets_begin(); i != CxP->facets_end(); ++i) 
		{
			Halfedge_facet_circulator j=i->facet_begin();
			do{
				verts[h++] = NxVec3( - j->vertex()->point().x(),j->vertex()->point().z(),j->vertex()->point().y()) ;
			}while( ++j!=i->facet_begin());
		}
		
		// Create descriptor for convex mesh
		NxConvexMeshDesc* convexDesc	= new NxConvexMeshDesc();
		convexDesc->numVertices          = vertCount;
		convexDesc->pointStrideBytes     = sizeof(NxVec3);
		convexDesc->points               = verts;
		convexDesc->flags                = NX_CF_COMPUTE_CONVEX;


		NxConvexShapeDesc convexShapeDesc;
		convexShapeDesc.localPose.t		= NxVec3(0,0,0);
		
		NxInitCooking();
		// Cooking from memory
	    MemoryWriteBuffer buf;
		bool status = NxCookConvexMesh(*convexDesc, buf);
	
		NxConvexMesh *pMesh			= gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
		convexShapeDesc.meshData	= pMesh;
		NxCloseCooking();

		pMesh->saveToDesc(*convexDesc);

		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&convexShapeDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 
			(
			- Pn->Position.x(),
			Pn->Position.z(),
			Pn->Position.y()
			);
		return scene->createActor(ActorDesc);
	}
};