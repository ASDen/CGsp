// Rendrer.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "osgData.h"
#include "glutViewer.h"


int main( int argc, char **argv )
{

	//Box_3* s = new Box_3(20,8,40,20,20,30);
	//Capsule_3* s = new Capsule_3(30,200,10,10);
	//ChamferCyl_3* s = new ChamferCyl_3(30,60,10,10,5,5,15);
	//Cone_3* s = new Cone_3(2,5,10,11,2,3);
	//Cylinder_3* s = new Cylinder_3(3,20,20,9,30);
	//Lathe_3* s = new Lathe_3(arr,Center,Z_ax,20,360);
	//Pyramid_3* s = new Pyramid_3(100,200,200,25,25,25);
	//Sphere_3* s = new Sphere_3(20,50);
	Spindle_3* s = new Spindle_3(40,70,20,25,25,25);
	//Spring_3* s = new Spring_3(20,2.5,200,10,10,40);
	//Torus_3* s = new Torus_3(20,5,0,0,10,20);
	//Tube_3* s = new Tube_3(4,3,5,4,5,20);
	
	s->Draw();


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




	FrameCreater::FillFrames(0,100,0.0,100.0,&Stretch::StAmount,St);

	s->ApplyModifier(&St);

	PolyhedronNode* c = new PolyhedronNode(s,osg::Vec3(5,5,5));
	c->WireFrame = false;


	osgPolyManager* pman = new osgPolyManager;
	pman->AddPolyhedron(c);

	glutInit(&argc, argv);
	gView(pman);
	
	return 0;
}

