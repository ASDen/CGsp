#pragma once

typedef Polyhedron::Edge_iterator	Edge_iterator;
typedef Primitives* pPrimitive;
typedef NxActorDesc* pNxActorDesc;
typedef KeyFrameModifier* pKeyFrameModifier;

class CGSP_CC PolyhedronNode : public osg::Drawable
{
public:

	union {
		NxActor* RigidActor;
		NxCloth* ClothActor;
	};

	std::vector<pKeyFrameModifier> ModStack;
	osg::PositionAttitudeTransform* Pos;
	osg::PositionAttitudeTransform* ModifiedPos;

	void ApplyModifier(pKeyFrameModifier M)
	{
		ModStack.push_back(M);
	}

	void UpdateAtFrame(int Fnum)
	{
		const_cast<PolyhedronNode*>(this)->getParent(0)->getParent(0)->asTransform()->asPositionAttitudeTransform()->setPosition(ModifiedPos->getPosition());
		const_cast<PolyhedronNode*>(this)->getParent(0)->getParent(0)->asTransform()->asPositionAttitudeTransform()->setAttitude(ModifiedPos->getAttitude());
		const_cast<PolyhedronNode*>(this)->getParent(0)->getParent(0)->asTransform()->asPositionAttitudeTransform()->setScale(ModifiedPos->getScale());

		std::vector<pKeyFrameModifier>::iterator i;
		ModifiedPos=static_cast<osg::PositionAttitudeTransform*> (Pos->clone(osg::CopyOp::DEEP_COPY_ALL));;
		for(i=ModStack.begin();i!=ModStack.end();i++)
		{
			(*i)->DoAtFrame(ModifiedPos,Fnum);
		}
	}

	pPrimitive P;
	NxActorDesc ActorDesc;
	osg::Vec3 Position;
	osg::Vec3 PColor;
	bool WireFrame;
	bool AntialisedLines;

	PolyhedronNode():WireFrame(true){}
	PolyhedronNode(pPrimitive iP,osg::Vec3 Pos=osg::Vec3(0,0,0)):P(iP),Position(Pos),PColor(0.0f,0.0f,1.0f) {}
	PolyhedronNode(const PolyhedronNode& poly,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY):
	osg::Drawable(poly,copyop) {}

	META_Object(myPoly,PolyhedronNode)
	virtual void drawImplementation(osg::RenderInfo&) const
	{
		//if(this->getNumParents()>0)
		//if(RigidActor == NULL && ClothActor == NULL)
		{
		
		}
		
		DrawPolyhedron();
		const_cast<PolyhedronNode*>(this)->dirtyBound();
	}
	virtual osg::BoundingBox computeBound() const
	{
		osg::BoundingBox bbox;
		Kernel::Iso_cuboid_3 c3 = CGAL::bounding_box(P->ModifiedMesh.points_begin (), P->ModifiedMesh.points_end ());
		bbox.set (CGAL::to_double(c3.xmin()),CGAL::to_double(c3.ymin()),CGAL::to_double(c3.zmin()),CGAL::to_double(c3.xmax()),CGAL::to_double(c3.ymax()),CGAL::to_double(c3.zmax()));
		return bbox;
	}
	void DrawPolyhedron() const
	{
		::glColor3f(PColor.x(),PColor.y(),PColor.z()); // change the color of facets
		Facet_iterator f;
		for(f = P->ModifiedMesh.facets_begin();
			f != P->ModifiedMesh.facets_end();
			f++)
		{
			::glBegin(GL_POLYGON);
			Vector_3 n = compute_facet_normal<Facet,Kernel>(*f);
			::glNormal3d(CGAL::to_double(n.x()),CGAL::to_double(n.y()),CGAL::to_double(n.z()));
			Halfedge_facet_circulator he = f->facet_begin();
			Halfedge_facet_circulator end = he;
			CGAL_For_all(he,end)
			{
				const Point_3& p = he->vertex()->point();
				::glVertex3d(CGAL::to_double(p.x()),CGAL::to_double(p.y()),CGAL::to_double(p.z()));
			}
			::glEnd();

		}

		if(!WireFrame)return;

		::glColor3f(0.0f,0.0f,0.0f); // change the color of lines
		::glEnable(GL_POLYGON_OFFSET_FILL);
		::glPolygonOffset(1.0f,1.0f);
		if(AntialisedLines)
		{
			::glEnable(GL_LINE_SMOOTH);
			::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		}
		::glBegin(GL_LINES);
		Edge_iterator he;
		for(he = P->ModifiedMesh.edges_begin();
			he != P->ModifiedMesh.edges_end();
			he++)
		{
			const Point_3& a = he->vertex()->point();
			const Point_3& b = he->opposite()->vertex()->point();
			::glVertex3d(CGAL::to_double(a.x()),CGAL::to_double(a.y()),CGAL::to_double(a.z()));
			::glVertex3d(CGAL::to_double(b.x()),CGAL::to_double(b.y()),CGAL::to_double(b.z()));
		}
		::glEnd();
	}
}; 

class CGSP_CC LightNode 
{
	
public:
	osg::Light* myLight ;
	osg::LightSource* lightS;
	osg::StateSet* rootStateSet;

	LightNode()
	{
		myLight = new osg::Light;
		myLight->setLightNum(0);
		myLight->setPosition(osg::Vec4(50.0,30.0,25.0,1.0f));
		myLight->setAmbient(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
		myLight->setDiffuse(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
		myLight->setSpecular(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
		myLight->setSpotCutoff(20.0f);
		myLight->setSpotExponent(5.0f);
		myLight->setDirection(osg::Vec3(1.0f,1.0f,-0.5f));
		myLight->setConstantAttenuation(0.5f);
		lightS = new osg::LightSource; 
		rootStateSet=new osg::StateSet;
		lightS->setLight(myLight);
		lightS->setLocalStateSetModes(osg::StateAttribute::ON); 
		lightS->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);

	}
	~LightNode(){};
};

class CGSP_CC osgPolyManager
{
public:
	std::vector<PolyhedronNode*> PolyBag;
	osg::ref_ptr<osg::Group> root;
	osg::Group* lightGroup;
	//osg::Group* root;

	osgPolyManager()
	{
		root=new osg::Group();
		lightGroup = new osg::Group;
		root->addChild(lightGroup);
	}

	template <class Manager>
	void AddPolyhedron(PolyhedronNode* Pn)
	{
		typedef typename Manager::UpdateCallback UC;

		Pn->setUseDisplayList( false );
		//Pn->setUseVertexBufferObjects( false ); 
		PolyBag.push_back(Pn);
		osg::PositionAttitudeTransform* pat=new osg::PositionAttitudeTransform();
		pat->setPosition(Pn->Position);
		root->addChild(pat);
		osg::Geode* g = new osg::Geode();
		g->addDrawable(Pn);
		pat->setUpdateCallback(new UC(Pn));
		pat->addChild(g);

		Pn->Pos         = static_cast<osg::PositionAttitudeTransform*> (pat->clone(osg::CopyOp::DEEP_COPY_ALL));
		Pn->ModifiedPos = static_cast<osg::PositionAttitudeTransform*> (pat->clone(osg::CopyOp::DEEP_COPY_ALL));
		
		//*(Pn->ModifiedPos) = *pat;
	}

	void addlight(LightNode* lightSrc)
	{
		//osg::LightSource* lightS1 = new osg::LightSource; 
		//osg::StateSet* rootStateSet=new osg::StateSet;
		
		//lightS1->setLight(light1);
		//lightS1->setLocalStateSetModes(osg::StateAttribute::ON); 

		//lightS1->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);
		lightGroup->addChild(lightSrc->lightS);

	}

	void UpdateFrame(int Fnum)
	{
		std::vector<PolyhedronNode*>::iterator i;
		for(i=PolyBag.begin();i!=PolyBag.end();i++)
		{
			(*i)->P->UpdateAtFrame(Fnum);
			(*i)->dirtyBound();
		}
	}
};