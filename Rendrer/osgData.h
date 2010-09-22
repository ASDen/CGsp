#pragma once

typedef Polyhedron::Edge_iterator	Edge_iterator;
typedef Polyhedron::Halfedge_const_iterator Halfedge_const_iterator;
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

	osg::ref_ptr<osg::PositionAttitudeTransform> Pos;
	osg::ref_ptr<osg::PositionAttitudeTransform> ModifiedPos;

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
		ModifiedPos = dynamic_cast<osg::PositionAttitudeTransform*> (Pos->clone(osg::CopyOp::DEEP_COPY_ALL));

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
	TexType TextureType;

	PolyhedronNode():WireFrame(true){}
	PolyhedronNode(pPrimitive iP,osg::Vec3 Pos=osg::Vec3(0,0,0)):P(iP),Position(Pos),PColor(1.0f,1.0f,1.0f) {}
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

				::glTexCoord2f(he->u(),he->v());
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

	
	osg::Vec4 Position;
	osg::Vec3 Direction;
	osg::Vec4 Ambient;
	osg::Vec4 Diffuse;
	osg::Vec4 Specular;
	double SpotCutoff;
	double SpotExponent;
	double ConstantAttenuation;

	LightNode()
	{
		myLight = new osg::Light;
		myLight->setLightNum(0);
		
		Position = osg::Vec4(0.0,0.0,0.0,1.0f);
		Direction = osg::Vec3(1.0f,1.0f,-0.5f);
		Ambient = osg::Vec4(0.0f,0.0f,1.0f,1.0f);
		Diffuse = osg::Vec4(0.0f,0.0f,1.0f,1.0f);
		Specular = osg::Vec4(0.0f,0.0f,0.0f,1.0f);
		SpotCutoff = 20.0f;
		SpotExponent = 5.0f;
		ConstantAttenuation = 0.5f;

		myLight->setPosition(Position);
		myLight->setAmbient(Ambient);
		myLight->setDiffuse(Diffuse);
		myLight->setSpecular(Specular);
		myLight->setSpotCutoff(SpotCutoff);
		myLight->setSpotExponent(SpotExponent);
		myLight->setDirection(Direction);
		myLight->setConstantAttenuation(ConstantAttenuation);
		
		lightS = new osg::LightSource; 
		rootStateSet = new osg::StateSet;

		lightS->setLight(myLight);
		lightS->setLocalStateSetModes(osg::StateAttribute::ON); 
		lightS->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);

	}
	~LightNode(){};
};

class MyGraphicsContext {
public:
	MyGraphicsContext()
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = 1;
		traits->height = 1;
		traits->windowDecoration = false;
		traits->doubleBuffer = false;
		traits->sharedContext = 0;
		traits->pbuffer = true;

		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		if (!_gc)
		{
			traits->pbuffer = false;
			_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		}

		if (_gc.valid()) 
		{
			_gc->realize();
			_gc->makeCurrent();
		}
	}

	bool valid() const { return _gc.valid() && _gc->isRealized(); }

private:
	osg::ref_ptr<osg::GraphicsContext> _gc;
};

class CGSP_CC osgPolyManager
{
public:
	std::vector<PolyhedronNode*> PolyBag;
	osg::ref_ptr<osg::Group> root;
	osg::Group* lightGroup;

	osgPolyManager()
	{
		root=new osg::Group();
		lightGroup = new osg::Group;
		root->addChild(lightGroup);
	}

	template <class Manager>
	void AddPolyhedron(PolyhedronNode* Pn, std::string fname = std::string(""), TexType ty = Tex_CGAL_General, int val = 0, float Hor = 0.09 , float Ver = 0.1)
	{
		typedef typename Manager::UpdateCallback UC;

		Pn->setUseDisplayList( false );
		PolyBag.push_back(Pn);
		osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
		pat->setPosition(Pn->Position);
		root->addChild(pat);
		osg::Geode* g = new osg::Geode();
		g->addDrawable(Pn);
		if(fname.size()>0)
		{
			g->setStateSet(AddTex(fname));
			Traingulate tr;
			switch(ty)
			{
			case Tex_CGAL_General:
				tr.Do(Pn->P->ModifiedMesh);
				Texture::CalcUV<Tex_CGAL_General>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			case Tex_Sphere:
				tr.Do(Pn->P->ModifiedMesh);
				Texture::CalcUV<Tex_Sphere>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			case Tex_Box:
				Texture::CalcUV<Tex_Box>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			case Tex_Cylinder:
				Texture::CalcUV<Tex_Cylinder>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			case Tex_Torus:
				Texture::CalcUV<Tex_Torus>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			case Tex_Tube:
				Texture::CalcUV<Tex_Tube>(Pn->P->ModifiedMesh, Pn->P, val, Hor, Ver);
				break;
			}
			
			Pn->P->setMesh(Pn->P->ModifiedMesh);
		}
		pat->setUpdateCallback(new UC(Pn));
		pat->addChild(g);

		Pn->Pos         = static_cast<osg::PositionAttitudeTransform*> (pat->clone(osg::CopyOp::DEEP_COPY_ALL));
		Pn->ModifiedPos = static_cast<osg::PositionAttitudeTransform*> (pat->clone(osg::CopyOp::DEEP_COPY_ALL));
	}


	osg::StateSet* AddTex(std::string fname)
	{
		MyGraphicsContext gc;
		if (!gc.valid()) 
		{
			osg::notify(osg::NOTICE)<<"Unable to create the graphics context required to build 3d image."<<std::endl;
			return 0;
		}


		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fname.c_str());

		if (!image) 
		{    
			std::cout << "Couldn't load texture." << std::endl;    
			return 0;
		} 
		GLint textureSize = osg::Texture3D::getExtensions(0,true)->maxTexture3DSize();
		if (textureSize > 256)
			int  textureSize = 256;
		image->scaleImage(textureSize,textureSize,1);  
		osg::Texture3D* texture = new osg::Texture3D; 
		texture->setFilter(osg::Texture3D::MIN_FILTER,osg::Texture3D::LINEAR);
		texture->setFilter(osg::Texture3D::MAG_FILTER,osg::Texture3D::LINEAR);
		texture->setWrap(osg::Texture3D::WRAP_R,osg::Texture3D::REPEAT);
		texture->setImage(image);
		osg::StateSet* stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
		return stateset;
	}

	void AddLight(LightNode* lightSrc)
	{
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