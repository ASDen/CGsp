// Rendrer.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "RdInc.h"

///////////////////
PhysicsManager* pxm;
osgPolyManager* pman;

#include "TestSit.h"

int main( int argc, char **argv )
{
	pxm = new PhysicsManager();
	pman = new osgPolyManager;
	pxm->InitOsg();

	//Box_Wall_Spring();
	//Spring_Test();
	//Spring_Test_2();
	//Tube_Test();
	cloth_test();

	pxm->setPolyManager(pman);
	pxm->DisplayLoop();	
	
	return 0;
}

