//
//  MeshSimplification.cpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//



#include "Macro.h"

#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>

#include "MeshSimplification.hpp"





typedef CGAL::Polyhedron_3<Kernel> Surface_mesh;
namespace SMS = CGAL::Surface_mesh_simplification ;
int Simplification(Mesh_t & P,double Ratio){
    SMS::Count_ratio_stop_predicate<Mesh_t> stop(Ratio);
    //typedef Polyhedron::Halfedge_handle        Halfedge_handle;
    //Polyhedron PP;
    auto aa=&P;
    auto a=P.is_valid();
    //SMS::Count_stop_predicate<Surface_mesh> stop(3);
    SMS::edge_collapse(P,
                       stop,
                       CGAL::vertex_index_map(get(CGAL::vertex_external_index,P)).
                       halfedge_index_map(get(CGAL::halfedge_external_index,P)).
                       get_cost(SMS::Edge_length_cost <Mesh_t>()).
                       get_placement(SMS::Midpoint_placement<Mesh_t>())
                       );
    return P.size_of_vertices();
}