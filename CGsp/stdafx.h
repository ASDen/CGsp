// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "stdlib.h"
#include <stdio.h>
#include <tchar.h>
//CGAL
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/Subdivision_method_3.h>
//Eigen
#include <Eigen/Geometry>
//GSL
#define GSL_DLL
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
//STL
#include <iostream>
#include <string>
#include <vector>
#include <fstream>



// TODO: reference additional headers your program requires here
