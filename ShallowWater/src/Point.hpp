//
//  Point.hpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp
#include "Macro.h"
#include "Mesh.hpp"

enum Type_t{
    WET,
    DRT,
    BOUND
};

template <class Refs>
struct Facet_t;

class Point_t;
typedef Point_t * pPoint;
class Point_t{
private:
    double h;
    double b;
    Point_3 Position;
    std::vector<pPoint> Neighbours;
    std::vector<double> w;
    Vector_3 Force;
    Type_t Type;
    Facet_t * Facet;
};

#endif /* Point_hpp */
