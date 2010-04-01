// Rendrer.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#define PHYSX_USE_VRD 1

#include "RdInc.h"



int main( int argc, char **argv )
{
	Box_3* s = new Box_3(4,8,4,1,1,1);
	//Capsule_3* s4 = new Capsule_3(5,21,25,25);
	//ChamferCyl_3* s4 = new ChamferCyl_3(6,12,2,20,15,15,15);
	//Cone_3* s4 = new Cone_3(2,8,10,55,55,55);
	Cylinder_3* s4 = new Cylinder_3(6,10,2,2,15);
	//Lathe_3* s = new Lathe_3(arr,Center,Z_ax,20,360);
	//Pyramid_3* s4 = new Pyramid_3(16,16,16,1,1,1);
	Sphere_3* s3 = new Sphere_3(2,60);
	Spindle_3* s5 = new Spindle_3(4,5,2,48,48,48);
	Plane_3* s2 = new Plane_3(100);
	//Spring_3* s = new Spring_3(20,2.5,200,10,10,40);
	//Torus_3* s4 = new Torus_3(20,5,0,0,10,20);
	Tube_3* s6 = new Tube_3(20,15,30,20,10,20);
	
	s->Draw();
	s2->Draw();
	s3->Draw();
	s4->Draw();
	s5->Draw();

	Bevel m(18,1.25,1.25);
	/*Eigen::Transform3d T;
	Eigen::Vector3d Original(0,0,s4->height/2);
	T.setIdentity();
	T.pretranslate (-Original);
	m.ApplyTransformToPolyhedron(s4->Mesh,T);
	s4->setMesh(s4->Mesh);*/

	Bevel Be(18,1.25,1.25);

	Bridge Br(18,20);

	Extrude Ex(18,1.25);
	
	Outline Ou(5,2.25);



	Bend Ben(90,s->Center,Z_ax,false,3,-3);

	Bulge Bu(36,s->Center,Z_ax,BRadial,false,5,-5);

	Cylindrical_Wave CylWa(1,10,0,s->Center,Z_ax);

	Linear_Wave LiWaX(1,4,0,s->Center,Z_ax,X_ax);
	Linear_Wave LiWaY(1,4,0,s->Center,Z_ax,Y_ax);

	//Polyhedron E;
	//E = s1.Draw();
	//Morph Mor(E,50);

	Skew Sk(30,s->Center,Z_ax,false,20,-20);

	Spherify Sph(100);

	Squeeze Sq(5,s->Center,Z_ax,false,10,-10);

	Stretch St(0,s->Center,Z_ax,false,4,-4);
	
	Taper Ta(3,s->Center,X_ax,false,20,-20);

	Twist Tw(90,s->Center,Z_ax,true,4,-4);

	PhysicsManager pxm;
	osgPolyManager* pman = new osgPolyManager;
	pxm.InitOsg();

	//FrameCreater::FillFrames(0,100,0.0,100.0,&Stretch::StAmount,St);

	//s->ApplyModifier(&St);
	//s2->ApplyModifier(&St);


	PolyhedronNode* c2 = new PolyhedronNode(s2,osg::Vec3(0,0,0));
	c2->Actor = XPlane::Construct(pxm.gScene,c2);
	c2->WireFrame = true;

	/*PolyhedronNode* c = new PolyhedronNode(s,osg::Vec3(50,50,20));
	c->Actor = XBox::Construct(pxm.gScene,c);
	c->WireFrame = true;*/

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10-i;j++)
		{
			PolyhedronNode* c1 = new PolyhedronNode(s,osg::Vec3((14+0.5*8*i)+j*8,50,i*4+2));
			c1->Actor = XBox::Construct(pxm.gScene,c1);
			c1->WireFrame = true;
			c1->AntialisedLines = true;
			pman->AddPolyhedron<PhysicsManager>(c1);
		}
	}
	PolyhedronNode* cx = new PolyhedronNode(s4,osg::Vec3(50,20,20));
	cx->Actor = XConvex::Construct<ConvexHullReducer>(pxm.gScene,pxm.gPhysicsSDK,cx);
	cx->WireFrame = false;
	cx->AntialisedLines = true;
	pman->AddPolyhedron<PhysicsManager>(cx);

	/*PolyhedronNode* cxy = new PolyhedronNode(s5,osg::Vec3(50,20,40));
	cxy->Actor = XConvex::Construct<GeometrySimplifyReducer>(pxm.gScene,pxm.gPhysicsSDK,cxy);
	cx->WireFrame = false;

	pman->AddPolyhedron<PhysicsManager>(cxy);*/
	
	
	PolyhedronNode* c3 = new PolyhedronNode(s6,osg::Vec3(50,50,100));
	c3->Actor = XTube::Construct(pxm.gScene,c3,8);
	c3->WireFrame = false;
	c3->AntialisedLines = true;
	pman->AddPolyhedron<PhysicsManager>(c3);
	
	
	//pman->AddPolyhedron<PhysicsManager>(c);
	pman->AddPolyhedron<PhysicsManager>(c2);
	//pman->AddPolyhedron<PhysicsManager>(c1);
	//pman->AddPolyhedron<PhysicsManager>(c3);
	
	//LightNode* ln=new LightNode();
	//pman->addlight(ln);

	pxm.setPolyManager(pman);
	pxm.DisplayLoop();

	//glutInit(&argc, argv);
	//gView(pman);
	//KeyFrameManager kfm;
	//kfm.InitOsg();
	//kfm.setPolyManager(pman);
	//kfm.DisplayLoop();

	
	return 0;
}

