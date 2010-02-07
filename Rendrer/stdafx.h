// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "../CGsp/stdafx.h"

typedef CGAL::Simple_cartesian<double>                 Kernel;
typedef Kernel::Point_3                                Point_3;
typedef Kernel::Vector_3                               Vector_3;
typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
typedef CGAL::Polyhedron_3<Traits>                     Polyhedron;
typedef Polyhedron::HalfedgeDS						   HalfedgeDS;
typedef Polyhedron::Facet_iterator                     Facet_iterator;
typedef Polyhedron::Vertex_iterator                    Vertex_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator   Halfedge_facet_circulator;
typedef Polyhedron::Halfedge_handle                    Halfedge_handle;
typedef Polyhedron::Plane_const_iterator			   Plane_const_iterator;
typedef  Polyhedron::Facet							   Facet;
typedef Kernel::Aff_transformation_3                   Aff3;

#include "../CGsp/Zre.h"

#include <CGAL/gl.h>
#include <CGAL/compute_normal.h>
#include <CGAL/bounding_box.h>

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Config>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// TODO: reference additional headers your program requires here
