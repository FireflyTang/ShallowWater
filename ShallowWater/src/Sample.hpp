//
//  Sample.hpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef Sample_hpp
#define Sample_hpp

#include "Macro.h"
#include "Mesh.hpp"

void Sample(std::vector<pPoint_t> & Points,Mesh_t & Mesh);
double Move(std::vector<pPoint_t> & pPoints, Mesh_t & Mesh, double a, double DeltaT);
#endif /* Sample_hpp */
