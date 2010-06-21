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
	kfm->InitOsg();
	kfm->setPolyManager(pman);

	Box_3* s = new Box_3(4,8,4,6,6,6);
	s->Draw();
	PolyhedronNode* c2 = new PolyhedronNode(s,osg::Vec3(0,0,0));
    pman->AddPolyhedron<KeyFrameManager>(c2);

	Twist* t=new Twist(45);
	//t->Center = new Point_3(2,2,2);
	t->X_Center.FrameValues[0] = 2;
	s->ApplyModifier(t);

	FrameCreater::FillFrames(36,360.0,&Twist::TwAngle,*t);
	FrameCreater::FillFrames(72,0.0,&Twist::TwAngle,*t);
	

	//Box_Wall_Spring();
	//Spring_Test();
	//Spring_Test_2();
	//Tube_Test();
	//cloth_test();

	
	kfm->DisplayLoop();	
	
	return 0;
}

