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

	BoxPropsI* bp=new BoxPropsI(&Box_3::length_Seg,s,4);
	s->ApplyModifier(bp);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp);

	BoxPropsI* bp2=new BoxPropsI(&Box_3::width_Seg,s,4);
	s->ApplyModifier(bp2);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp2);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp2);

	BoxPropsI* bp3=new BoxPropsI(&Box_3::height_Seg,s,4);
	s->ApplyModifier(bp3);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp3);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp3);

	Twist* t=new Twist(45);
	//t->Center = new Point_3(2,2,2);
	t->X_Center.FrameValues[0] = 5;
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

