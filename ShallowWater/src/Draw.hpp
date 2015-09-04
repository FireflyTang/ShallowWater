//
//  Draw.hpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef Draw_hpp
#define Draw_hpp
#include <gl/glut.h>
#include "Macro.h"
void DrawSence
(std::vector<Triangle_3> * _pTrianglesToDraw,std::vector<Segment_3> * _pSegmentsToDraw);

void InitScene(int argc, char ** argv);

#endif /* Draw_hpp */
