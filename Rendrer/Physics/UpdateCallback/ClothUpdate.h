
class CGSP_CC ClothUpdateCallback : public BaseUpdateCallBack
{
public:

	ClothUpdateCallback(PolyhedronNode* P):BaseUpdateCallBack(P)
	{}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		NxClothMeshDesc desc;
	
		NxMeshData msh = Pn->ClothActor->getMeshData();
	
		//int* n = (int*)msh.numVerticesPtr;
		//if(*n<=0)return;
	
		NxVec3 *p = (NxVec3*)malloc(sizeof(NxVec3) * (Pn->P->ModifiedMesh.size_of_vertices()));
		Pn->ClothActor->getPositions(p);
	
		int j=0;
		for (Vertex_iterator i = Pn->P->ModifiedMesh.vertices_begin(); i != Pn->P->ModifiedMesh.vertices_end(); ++i,j++)
		{
			i->point() = Point_3(-p[j].x,p[j].z,p[j].y);
		}

		osg::PositionAttitudeTransform* _mat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
		_mat->setPosition(osg::Vec3f(0,0,0));

		//std::transform(Pn->P->ModifiedMesh.facets_begin(), Pn->P->ModifiedMesh.facets_end(), Pn->P->ModifiedMesh.planes_begin(), Normal_vector());
		
		/*NxVec3 *buff = (NxVec3*)malloc(sizeof(NxVec3) * (*n));
		if (buff)
		{
			Pn->ClothActor->getPositions(buff);
			osg::Geometry* geo = dynamic_cast<osg::Geometry*>(drawable);
			if (geo && p)
			{
				osg::Vec3Array* vertices = new osg::Vec3Array;			
				for (int i = 0; i < *n; i++) 
				{			
					vertices->push_back(osg::Vec3f(buff->x, buff->z, buff->y));
					buff++;
				}	
				geo->setVertexArray(vertices);
			}
		}*/		

	}

};
