// Rendrer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "osgData.h"
#include "glutViewer.h"


int main( int argc, char **argv )
{

	Box_3* s1 = new Box_3(20,8,40,20,20,30);
	s1->Draw();
	
	//Spring_3* s2 = new Spring_3(30,3,200,6,20,30);
	//s2->Draw();
	//Polyhedron P;
	//P = s2->Draw();

	//Outline oA(10,1.0);
	//FrameCreater::FillFrames(0,15,0.1,2.0,&Outline::OutlineAmount,oA);
	//FrameCreater::FillFrames(15,30,2.0,0.1,&Outline::OutlineAmount,oA);

	Point_3* Center = new Point_3(4,10,20);
	
	//Twist Tw(900,s1->Center,Z_ax,true,40,-40);
	//Skew Tw2(1,s1->Center,Y_ax,false,4,-4);
	Bend Tw(270,Center,Z_ax,true,0,0);
	//Bevel Tw2(18,1.25,1.25);
	//Morph Tw(P,5);
	


	//Bend Be(900,Center,X_ax,true,40,-40);
	//Bend Be2(900,Center,Y_ax,true,40,-40);
	//Bend Be3(900,Center,Z_ax,true,40,-40);


	FrameCreater::FillFrames(0,100,20.0,3.0,&Bend::Upper,Tw);
	FrameCreater::FillFrames(0,100,-20.0,-3.0,&Bend::Lower,Tw);
	FrameCreater::FillFrames(0,100,0.0,180.0,&Bend::BeAngle,Tw);
	
	FrameCreater::FillFrames(101,200,3.0,20.0,&Bend::Upper,Tw);
	FrameCreater::FillFrames(101,200,-3.0,-20.0,&Bend::Lower,Tw);
	FrameCreater::FillFrames(101,200,180.0,0.0,&Bend::BeAngle,Tw);
	
	FrameCreater::FillFrames(201,300,20.0,3.0,&Bend::Upper,Tw);
	FrameCreater::FillFrames(201,300,-20.0,-3.0,&Bend::Lower,Tw);
	FrameCreater::FillFrames(201,300,0.0,-180.0,&Bend::BeAngle,Tw);

	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be2);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be3);

	
	//FrameCreater::FillFrames(0,5,2,2,&Outline::Fnum,oA);
	//FrameCreater::FillFrames(6,12,5,5,&Outline::Fnum,oA);


	s1->ApplyModifier(&Tw);
	//s1->ApplyModifier(&Tw2);
	//s1->ApplyModifier(&Tw3);

	//s2->ApplyModifier(&Tw);
	//s2->ApplyModifier(&Tw2);
	//s2->ApplyModifier(&Tw3);
	
	s->ApplyModifier(&Be);
	s->ApplyModifier(&Be2);
	s->ApplyModifier(&Be3);



	PolyhedronNode* c=new PolyhedronNode(s1,osg::Vec3(5,5,5));
	//PolyhedronNode* c2=new PolyhedronNode(s2,osg::Vec3(-40,5,5));
	c->WireFrame=false;
	


	osgPolyManager* pman=new osgPolyManager;
	pman->AddPolyhedron(c);
	//pman->AddPolyhedron(c2);
	std::cout<<s1->Center<<std::endl;

	//glutViewer gv;
	//gv.View(pman);
	glutInit(&argc, argv);
	gView(pman);
	
	return 0;
}

