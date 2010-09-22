#define TEAR_MEMORY_FACTOR 2

class CGSP_CC XCloth
{
public:
    static NxCloth* Construct(NxScene* scene,NxPhysicsSDK* gPhysicsSDK,PolyhedronNode* Pn,bool app_press=false)
    {
        NxClothDesc clothDesc;
        NxClothMeshDesc meshDesc;
        NxMeshData mReceiveBuffers;
        NxClothMesh *mClothMesh;

        clothDesc.flags |= NX_CLF_COLLISION_TWOWAY;
        clothDesc.flags |= NX_CLF_VISUALIZATION;
        clothDesc.flags |= NX_CLF_SELFCOLLISION;
        //clothDesc.flags |= NX_CLF_HARDWARE;
		if(app_press)
            clothDesc.flags |= NX_CLF_PRESSURE;

        genPolyMeshDesc(meshDesc,Pn);

        NxInitCooking();
        MemoryWriteBuffer wb;
        NxCookClothMesh(meshDesc, wb);
        MemoryReadBuffer rb(wb.data);
        mClothMesh = scene->getPhysicsSDK().createClothMesh(rb);
        NxCloseCooking();

        releaseMeshDescBuffers(meshDesc);

        allocateReceiveBuffers(meshDesc.numVertices, meshDesc.numTriangles,mReceiveBuffers);

        clothDesc.clothMesh = mClothMesh;
        clothDesc.meshData  = mReceiveBuffers;
        //clothDesc.thickness = thick;

        clothDesc.globalPose.t = NxVec3
            (
            -Pn->Position.x(),
             Pn->Position.z(),
             Pn->Position.y()
            );
       
        return scene->createCloth(clothDesc);
    }

    static void genPolyMeshDesc(NxClothMeshDesc &desc,PolyhedronNode* Pn)
    {
        int j=0 , k=0;
        Polyhedron *CxP = & Pn->P->ModifiedMesh ;
        //triangulate mesh , PhysX requires triangle meshes
        Traingulate tr;
        tr.Do(*CxP);
        //CGAL::Subdivision_method_3::Sqrt3_subdivision(*CxP,1);
       

        desc.numVertices                = CxP->size_of_vertices();
        desc.numTriangles                = CxP->size_of_facets();
        desc.pointStrideBytes            = sizeof(NxVec3);
        desc.triangleStrideBytes        = 3*sizeof(NxU32);
        desc.vertexMassStrideBytes        = sizeof(NxReal);
        desc.vertexFlagStrideBytes        = sizeof(NxU32);
        desc.points                        = (NxVec3*)malloc(desc.pointStrideBytes * desc.numVertices);
        desc.triangles                    = (NxU32*) malloc(desc.triangleStrideBytes * desc.numTriangles);
        desc.vertexMasses                = 0;
        desc.vertexFlags                = 0;
        desc.flags                        = 0;

        for (Vertex_iterator i = CxP->vertices_begin(); i != CxP->vertices_end(); ++i,j++)
        {
            Point_3 p = i->point();
            ((NxVec3*)desc.points)    [j] = NxVec3(-p.x() , p.z() , p.y() );
        }

        for (  Facet_iterator i = CxP->facets_begin(); i != CxP->facets_end(); ++i)
        {
            Halfedge_facet_circulator j = i->facet_begin();
            do
            {
                ((NxU32*)desc.triangles)[k++] = std::distance(CxP->vertices_begin(), j->vertex());
            } while ( --j != i->facet_begin());
        }
    }

    static void releaseMeshDescBuffers(const NxClothMeshDesc& desc)
    {
        NxVec3* p = (NxVec3*)desc.points;
        NxU32* t = (NxU32*)desc.triangles;
        NxReal* m = (NxReal*)desc.vertexMasses;
        NxU32* f = (NxU32*)desc.vertexFlags;
        free(p);
        free(t);
        free(m);
        free(f);
    }

    static void allocateReceiveBuffers(int numVertices, int numTriangles, NxMeshData mReceiveBuffers)
    {
        // here we setup the buffers through which the SDK returns the dynamic cloth data
        // we reserve more memory for vertices than the initial mesh takes
        // because tearing creates new vertices
        // the SDK only tears cloth as long as there is room in these buffers
        NxU32 maxVertices = TEAR_MEMORY_FACTOR * numVertices;
        mReceiveBuffers.verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
        mReceiveBuffers.verticesNormalBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
        mReceiveBuffers.verticesPosByteStride = sizeof(NxVec3);
        mReceiveBuffers.verticesNormalByteStride = sizeof(NxVec3);
        mReceiveBuffers.maxVertices = maxVertices;
        mReceiveBuffers.numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));

        // the number of triangles is constant, even if the cloth is torn
        NxU32 maxIndices = 3*numTriangles;
        mReceiveBuffers.indicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxIndices);
        mReceiveBuffers.indicesByteStride = sizeof(NxU32);
        mReceiveBuffers.maxIndices = maxIndices;
        mReceiveBuffers.numIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

        // the parent index information would be needed if we used textured cloth
        NxU32 maxParentIndices = maxVertices;
        mReceiveBuffers.parentIndicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxParentIndices);
        mReceiveBuffers.parentIndicesByteStride = sizeof(NxU32);
        mReceiveBuffers.maxParentIndices = maxParentIndices;
        mReceiveBuffers.numParentIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

        // init the buffers in case we want to draw the mesh
        // before the SDK as filled in the correct values
        *mReceiveBuffers.numVerticesPtr = 0;
        *mReceiveBuffers.numIndicesPtr = 0;
    }
};
