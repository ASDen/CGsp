#pragma once

typedef CGAL::Cartesian_d<double>                Kd;
typedef CGAL::Min_sphere_annulus_d_traits_d<Kd>  Txaits;
typedef CGAL::Min_sphere_d<Txaits>               Min_sphere;
typedef Kd::Point_d                              Pointd;


class CGSP_CC XSphere 
{
public:

	static NxActor* Construct(NxScene* scene,PolyhedronNode* Pn,bool StaticObj = false)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;
		
		NxSphereShapeDesc SphereDesc;
		
		int n = Pn->P->ModifiedMesh.size_of_vertices();
		Pointd* ps=new Pointd[n];
		int j=0;
		for(Point_iterator i =Pn->P->ModifiedMesh.points_begin();i!=Pn->P->ModifiedMesh.points_end();i++,j++ )
			ps[j] = Pointd( 3,i->cartesian_begin(),i->cartesian_end() );
		Min_sphere  ms (ps, ps+n);
		
		SphereDesc.radius = std::sqrt( ms.squared_radius() );		
		SphereDesc.localPose.t = NxVec3(0, 0, 0);
		
		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&SphereDesc);
		ActorDesc.body			= (StaticObj)?NULL : &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3 //get center of the box
			(
			-1*(Pn->Position.x())/*+SphereDesc.radius)*//*ms.center().x()*/,
			Pn->Position.z()/*+SphereDesc.radius*//*ms.center().z()*/,
			Pn->Position.y()/*+SphereDesc.radius*//*ms.center().y()*/
			);

		return scene->createActor(ActorDesc);
	}
};