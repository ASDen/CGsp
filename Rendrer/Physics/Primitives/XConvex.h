#define MAX_NX_CX_FACES 255

class CGSP_CC GeometrySimplifyReducer
{
public:
	static void Reduce(Polyhedron *CxP)
	{
		//Traingulate factets first
		Traingulate tr;
		tr.Do(*CxP);

		//Simplify
		int numE = 384; // resulting in 254 faces < 255
		namespace SMS = CGAL::Surface_mesh_simplification;
		SMS::Count_stop_predicate< Polyhedron > stop(numE); // target #edges
		SMS::edge_collapse( *CxP, stop,
			CGAL::vertex_index_map(boost::get(CGAL::vertex_external_index,*CxP))
			.edge_index_map(boost::get(CGAL::edge_external_index,*CxP)));

		std::cout<<"Reduced geometry simplification to "<<CxP->size_of_facets()<<" Faces "<<std::endl;
	}
};

class CGSP_CC ConvexHullReducer
{
public:
	static void Reduce(Polyhedron *CxP)
	{
		int threshold = 5;
		int oldF , newF;
		//*CxP = P; // copy polyhedron before iterative convex hull construction
		oldF = CxP->size_of_facets();
		while(true)
		{
			CGAL::convex_hull_3(CxP->points_begin(),CxP->points_end(),*CxP);
			newF = CxP->size_of_facets();

			if(oldF - newF <threshold )
				break;
			oldF = newF;
		}
		std::cout<<"Reduced using Convex Hulls to "<<newF<<" Faces "<<std::endl;

		if (newF > MAX_NX_CX_FACES)
			GeometrySimplifyReducer::Reduce(CxP);

	}
};

class CGSP_CC XConvex
{
public:

	template<class ReductionClass>
	static NxActor* Construct(NxScene* scene,NxPhysicsSDK* gPhysicsSDK,PolyhedronNode* Pn,bool StaticObj = false)
	{
		NxBodyDesc BodyDesc;
		Polyhedron *CxP = new Polyhedron;
		*CxP = Pn->P->Mesh ;

		ReductionClass::Reduce(CxP);
		
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
		ActorDesc.body			= (StaticObj)?NULL : &BodyDesc;
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