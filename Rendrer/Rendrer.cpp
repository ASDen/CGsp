// Rendrer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "osgData.h"
#include "glutViewer.h"


int main( int argc, char **argv )
{
	/*Box_3* s=new Box_3(2,4,8,50,50,50);
	s->Draw();*/
	Sphere_3* s=new Sphere_3(20,250);
    s->Draw ();
	Sphere_3* s2=new Sphere_3(20,150);
    s2->Draw ();
	

	//Outline oA(10,1.0);
	//FrameCreater::FillFrames(0,15,0.1,2.0,&Outline::OutlineAmount,oA);
	//FrameCreater::FillFrames(15,30,2.0,0.1,&Outline::OutlineAmount,oA);

	Point_3 Center = Point_3(1,2,4);
	Twist Tw(90,Center,X_ax,40,-40);
	Twist Tw2(90,Center,Y_ax,40,-40);
	Twist Tw3(90,Center,Z_ax,40,-40);

	Bend Be(900,Center,X_ax,40,-40);
	Bend Be2(900,Center,Y_ax,40,-40);
	Bend Be3(900,Center,Z_ax,40,-40);


	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Twist::TwAngle,Tw);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Twist::TwAngle,Tw2);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Twist::TwAngle,Tw3);

	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be2);
	FrameCreater::FillFrames(0,1000,0.0,9000.0,&Bend::BeAngle,Be3);

	
	//FrameCreater::FillFrames(0,5,2,2,&Outline::Fnum,oA);
	//FrameCreater::FillFrames(6,12,5,5,&Outline::Fnum,oA);

	s->ApplyModifier(&Tw);
	s->ApplyModifier(&Tw2);
	s->ApplyModifier(&Tw3);

	//s2->ApplyModifier(&Tw);
	////s->ApplyModifier(&Tw2);
	//s2->ApplyModifier(&Tw3);
	
	s->ApplyModifier(&Be);
	s->ApplyModifier(&Be2);
	s->ApplyModifier(&Be3);


	std::vector<PolyhedronNode*> asd;
	osgPolyManager* pman=new osgPolyManager;
	for(int i=0;i<1;i++)
	{
		PolyhedronNode* c=new PolyhedronNode(s,osg::Vec3(-40*i,5,5));
		asd.push_back( c );
		c->WireFrame=false;
		pman->AddPolyhedron(c);

	}
	//PolyhedronNode* c00=new PolyhedronNode(s,osg::Vec3(0,5,5));
	//PolyhedronNode* c01=new PolyhedronNode(s,osg::Vec3(-40,5,5));
	//PolyhedronNode* c02=new PolyhedronNode(s,osg::Vec3(-80,5,5));

	//PolyhedronNode* c2=new PolyhedronNode(s2,osg::Vec3(-40,5,5));
	////c->WireFrame=false;
	//


	//
	//pman->AddPolyhedron(c00);
	//pman->AddPolyhedron(c01);
	//pman->AddPolyhedron(c02);

	//pman->AddPolyhedron(c2);

	

	//
	//glutViewer gv;
	//gv.View(pman);
	glutInit(&argc, argv);
	gView(pman);
	
	return 0;
}

