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

	Lathe();

	/*BoxPropsI* bp=new BoxPropsI(&Box_3::length_Seg,s,4);
	s->ApplyModifier(bp);

	FrameCreater::FillFrames(30,50,&BoxPropsI::PolyP,*bp);
	FrameCreater::FillFrames(60,6,&BoxPropsI::PolyP,*bp);

	BoxPropsI* bp2=new BoxPropsI(&Box_3::width_Seg,s,4);
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

	

	//Box_Wall_Spring();
	//Spring_Test();
	//Spring_Test_2();
	//Tube_Test();
	//cloth_test();

	
	kfm->DisplayLoop();	

	
	return 0;
}

