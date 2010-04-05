
#if defined(_MSC_VER) && defined(OSG_DISABLE_MSVC_WARNINGS)
// disable warning "glutCreateMenu_ATEXIT_HACK' : unreferenced local function has been removed"
#pragma warning( disable : 4505 )
#endif

#include <iostream>
#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <osg/Config>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

osg::ref_ptr<osgViewer::Viewer> viewer;
osg::observer_ptr<osgViewer::GraphicsWindow> window;
osgPolyManager* Man;

int fnum=0;
float T0=0,Telp=0;
float rate = 2;

void display(void)
{
	Telp+=viewer->elapsedTime()-T0;
	T0=viewer->elapsedTime();
	if(Telp>=1/rate)
	{
		Telp=0;
		//update scence
		Man->UpdateFrame(fnum++);
		std::cout<<"Frame #"<<fnum<<std::endl;
		if (viewer.valid()) viewer->frame();
	}

	// render the scene graph
	

	// Swap Buffers
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape( int w, int h )
{
	// update the window dimensions, in case the window has been resized.
	if (window.valid()) 
	{
		window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
		window->getEventQueue()->windowResize(window->getTraits()->x, window->getTraits()->y, w, h );
	}
}

void mousebutton( int button, int state, int x, int y )
{
	if (window.valid())
	{
		if (state==0) window->getEventQueue()->mouseButtonPress( x, y, button+1 );
		else window->getEventQueue()->mouseButtonRelease( x, y, button+1 );
	}
}

void mousemove( int x, int y )
{
	if (window.valid())
	{
		window->getEventQueue()->mouseMotion( x, y );
	}
}

void keyboard( unsigned char key, int /*x*/, int /*y*/ )
{
	switch( key )
	{
	case 27:
		// clean up the viewer 
		if (viewer.valid()) viewer = 0;
		glutDestroyWindow(glutGetWindow());
		break;
	case 'r':
		fnum=0;
		break;
	case '+':
		rate++;
		std::cout<<"Frame Rate : "<<rate<<std::endl;
	default:
		if (window.valid())
		{
			window->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) key );
			window->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) key );
		}
		break;
	}
}


//class glutViewer
//{
//public:

	void gxlInit()
	{
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA );
		glutInitWindowPosition( 100, 100 );
		glutInitWindowSize( 800, 600 );
		glutCreateWindow( "Zlang - Rendering" );
		glutDisplayFunc( display );
		glutReshapeFunc( reshape );
		glutMouseFunc( mousebutton );
		glutMotionFunc( mousemove );
		glutKeyboardFunc( keyboard );
	}

	void gView(osgPolyManager* Mg)
	{
		Man=Mg;
		gxlInit();
		//glutInit();
		viewer = new osgViewer::Viewer;
		window = viewer->setUpViewerAsEmbeddedInWindow(100,100,800,600);
		viewer->setSceneData(Mg->root);
		viewer->setCameraManipulator(new osgGA::TrackballManipulator);
		viewer->addEventHandler(new osgViewer::StatsHandler);
		viewer->realize();

		glutMainLoop();
	}
//};