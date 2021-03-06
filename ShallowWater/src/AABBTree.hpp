//
//  AABBTree.hpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef AABBTree_hpp
#define AABBTree_hpp

#include "Macro.h"

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>

class AABBFacet_t{
public:
    Vector_3 Normal;
    Triangle_3 Triangle;
};

struct Facet_primitive {
public:
    typedef AABBFacet_t * Id;
    typedef Kernel::Point_3    Point;
    typedef Kernel::Triangle_3 Datum;
private:
    typedef std::vector<AABBFacet_t>::iterator Face_Iterator;
public:
    Facet_primitive() {}
    Facet_primitive(Face_Iterator it): m_pt(&(*it)) {}
    const Id & id() const ;
    const Datum & datum() const ;
    const Point & reference_point() const;
private:
    Id m_pt;
};

typedef CGAL::AABB_traits<Kernel, Facet_primitive> AABB_face_traits;
typedef CGAL::AABB_tree<AABB_face_traits> AABBTree_t;

int LoadAABBTree(std::vector<AABBFacet_t> & facets, char * fn);

#endif /* AABBTree_hpp */
