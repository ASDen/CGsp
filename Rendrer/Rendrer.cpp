// Rendrer.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "RdInc.h"

///////////////////
PhysicsManager* pxm;
KeyFrameManager* kfm;
osgPolyManager* pman;

#include "TestSit.h" 


int main( int argc, char **argv )
{
	pxm  = new PhysicsManager();
	kfm  = new KeyFrameManager();
	pman = new osgPolyManager();
	pxm->InitOsg();
	//kfm->InitOsg();
	pxm->setPolyManager(pman);
	//kfm->setPolyManager(pman);

	/*std::vector <Point_3> arr;
	arr.push_back(Point_3 (2,0,2));
	arr.push_back(Point_3 (6,0,3));
	arr.push_back(Point_3 (8,0,4));
	arr.push_back(Point_3 (3,0,5));
	arr.push_back(Point_3 (5,0,6));
	arr.push_back(Point_3 (8,0,7));

	arr.push_back(Point_3 (0,1.75,8));
	arr.push_back(Point_3 (0,1.50,8));
	arr.push_back(Point_3 (0,1.25,8));

	arr.push_back(Point_3 (0,1,8));
	arr.push_back(Point_3 (0,1,7));
	arr.push_back(Point_3 (0,1,6));
	arr.push_back(Point_3 (0,1,5));
	arr.push_back(Point_3 (0,1,4));
	arr.push_back(Point_3 (0,1,3));

	arr.push_back(Point_3 (0,1.25,3));
	arr.push_back(Point_3 (0,1.50,3));
	arr.push_back(Point_3 (0,1.75,3));

	Point_3* Center = new Point_3(0,0,0);

	Lathe_3* s = new Lathe_3(arr,Center,Z_ax,20,360);
    
    s->Draw();
    
    PolyhedronNode* c2 = new PolyhedronNode(s,osg::Vec3(0,0,0));
    c2->RigidActor = XLathe::Construct(pxm->gScene,pxm->gPhysicsSDK,c2);
    c2->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c2);*/

	//Lathe();
	//cloth_test();

	/*Box_3* s = new Box_3(6,6,6,15,15,15);
	s->Draw();
	PolyhedronNode* c1 = new PolyhedronNode(s,osg::Vec3(0,0,0));

	pman->AddPolyhedron<KeyFrameManager>(c1);

	Twist* t=new Twist(0);
	s->ApplyModifier(t);

	FrameCreater::FillFrames(0,50.0,&Twist::TwAngle,*t);
	FrameCreater::FillFrames(60,6.0,&Twist::TwAngle,*t);
	FrameCreater::FillFrames(1500,20000.0,&Twist::TwAngle,*t);*/
	
	/*BoxPropsI* bp=new BoxPropsI(&Box_3::length_Seg,s,4);
	s->ApplyModifier(bp);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp);*/

	/*BoxPropsI* bp2=new BoxPropsI(&Box_3::width_Seg,s,4);
	s->ApplyModifier(bp2);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp2);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp2);

	BoxPropsI* bp3=new BoxPropsI(&Box_3::height_Seg,s,4);
	s->ApplyModifier(bp3);

	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp3);*/

	//Twist* t=new Twist(45);
	////t->Center = new Point_3(2,2,2);
	//t->X_Center.FrameValues[0] = 5;
	//s->ApplyModifier(t);

	//FrameCreater::FillFrames(36,360.0,&Twist::TwAngle,*t);
	//FrameCreater::FillFrames(72,0.0,&Twist::TwAngle,*t);

	//Translate* tr=new Translate(0);
	//c2->ApplyModifier(tr);
	//FrameCreater::FillFrames(40,30.0,&Translate::tx,*tr);

	//Rotate* rr=new Rotate(0);
	//c2->ApplyModifier(rr);
	//FrameCreater::FillFrames(40,6.0,&Rotate::ax,*rr);

	//Scale* sr=new Scale(1);
	//c2->ApplyModifier(sr);
	//FrameCreater::FillFrames(40,4.0,&Scale::sx,*sr);

	//Plane_3* s = new Plane_3(30,30,100,100);

	//s->Draw();
	//PolyhedronNode* c1 = new PolyhedronNode(s,osg::Vec3(0,0,0));

	//pman->AddPolyhedron<KeyFrameManager>(c1);
	////Noise* No = new Noise(5,0.3,0,s->Center,Z_ax);
	////s->ApplyModifier(No);

	//LightNode* l1 = new LightNode();
	//l1->myLight->setDirection(osg::Vec3(10,0,0));
	//l1->myLight->setDiffuse(osg::Vec4(1,1,0.7,0.1));
	//	
	//LightNode* l2 = new LightNode();
	//l2->myLight->setDirection(osg::Vec3(-10,0,0));
	//l2->myLight->setDiffuse(osg::Vec4(0,1,0.7,0.1));

	//LightNode* l3 = new LightNode();
	//l3->myLight->setDirection(osg::Vec3(0,10,0));
	//l3->myLight->setDiffuse(osg::Vec4(1,0,0.7,0.1));
	//	
	//LightNode* l4 = new LightNode();
	//l4->myLight->setDirection(osg::Vec3(0,-10,0));

	//pman->addlight(l1);
	//pman->addlight(l2);
	//pman->addlight(l3);
	//pman->addlight(l4);

	//Box_Wall_Spring();
	//Spring_Test();
	//Spring_Test_2();
	//Tube_Test();
	cloth_test();
	//CowTex();

	
	pxm->DisplayLoop();	
	//kfm->DisplayLoop();	
	
	return 0;
}

