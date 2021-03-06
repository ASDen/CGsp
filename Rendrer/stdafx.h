// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>
#include <tchar.h>
//#include <stdint.h>

//CGsp
#include "../CGsp/stdafx.h"


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
#include "Texture/Polyhedron_ex.h"

typedef Polyhedron_ex                                       Polyhedron;
typedef Polyhedron::Point_3                                 Point_3;
typedef Polyhedron::Vector_3                                Vector_3;
typedef Polyhedron::HalfedgeDS								HalfedgeDS;
typedef Polyhedron::Facet_iterator							Facet_iterator;
typedef Polyhedron::Vertex_iterator							Vertex_iterator;
typedef Polyhedron::Point_iterator							Point_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator		Halfedge_facet_circulator;
typedef Polyhedron::Halfedge_handle							Halfedge_handle;
typedef Polyhedron::Plane_const_iterator					Plane_const_iterator;
typedef Polyhedron::Facet									Facet;
typedef Kernel::Aff_transformation_3						Aff3;

#include "../CGsp/Zre.h"

//CGAL
#define NOMINMAX // for conflicts with NxMath
#include <CGAL/gl.h>
#include <CGAL/compute_normal.h>
#include <CGAL/bounding_box.h>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Min_sphere_annulus_d_traits_d.h>
#include <CGAL/Min_sphere_d.h>

//OSG
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Config>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ImagePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/Material>
#include <osg/Texture3D>

//GLUT
#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

//PhysX
#include "NxPhysics.h"
#include "Physics/Helpers/Cooking.h"
#include "Physics/Helpers/UserAllocator.h"
#include "Physics/Helpers/Stream.h"
#include "Physics/Helpers/ErrorStream.h"
#include "Physics/Helpers/Utilities.h"
#include "Physics/Helpers/SamplesVRDSettings.h"

// TODO: reference additional headers your program requires here
