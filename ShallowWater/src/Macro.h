//
//  Macro.h
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef Macro_h
#define Macro_h

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Line_2.h>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <CGAL/Polyhedron_3.h>


#define __DRAW__

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Ray_3 Ray_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Vector_2 Vector_2;
typedef Kernel::Segment_3 Segment_3;
//typedef Kernel::Direction_3 Direction_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Triangle_2 Triangle_2;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Line_3 Line_3;
typedef Kernel::Line_2 Line_2;


#define EPSILON 2e-10
#define DELTA 2e-5
#define NORMALIZE(x) ((x)/(sqrt((x).squared_length())))
#define M_PI 3.1415926535897932384626

#endif /* Macro_h */
