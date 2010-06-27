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

	kfm->DisplayLoop();
	
	return 0;
}

