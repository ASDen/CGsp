#include "Animation.h"
#include "Modifiers.h"
#include "Primitives.h"


/* Interpolators */
#include "Interpolators\Constant.h"

/* Primitives */
#include "Primitives\3D\Box_3.h"
#include "Primitives\3D\Capsule_3.h"
#include "Primitives\3D\ChamferCyl_3.h"
#include "Primitives\3D\Cone_3.h"
#include "Primitives\3D\Cylinder_3.h"
#include "Primitives\3D\Lathe_3.h"
#include "Primitives\3D\Pyramid_3.h"
#include "Primitives\3D\Sphere_3.h"
#include "Primitives\3D\Spindle_3.h"
#include "Primitives\3D\Spring_3.h"
#include "Primitives\3D\Torus_3.h"
#include "Primitives\3D\Tube_3.h"

#include "Primitives\2D\Arc_2.h"
#include "Primitives\2D\Circle_2.h"
#include "Primitives\2D\Ellipse_2.h"
#include "Primitives\2D\Plane_3.h"
#include "Primitives\2D\Rectangle_2.h"

/* Modifiers */
#include "Modifiers\Facet\Bridge.h"
#include "Modifiers\Facet\Extrude.h"
#include "Modifiers\Facet\Outline.h"
#include "Modifiers\Facet\Bevel.h"

#include "Modifiers\Polyhedron\Bend.h"
#include "Modifiers\Polyhedron\Bulge.h"
#include "Modifiers\Polyhedron\Cylindrical_Wave.h"
#include "Modifiers\Polyhedron\Linear_Wave.h"
#include "Modifiers\Polyhedron\Morph.h"
#include "Modifiers\Polyhedron\Noise.h"
#include "Modifiers\Polyhedron\Skew.h"
#include "Modifiers\Polyhedron\Slice.h"
#include "Modifiers\Polyhedron\Smooth.h"
#include "Modifiers\Polyhedron\Spherify.h"
#include "Modifiers\Polyhedron\Squeeze.h"
#include "Modifiers\Polyhedron\Stretch.h"
#include "Modifiers\Polyhedron\Taper.h"
#include "Modifiers\Polyhedron\Twist.h"