//
//  main.cpp
//  ShallowWater-Box
//
//  Created by 唐如�?on 15/9/3.
//  Copyright (c) 2015 Computer Graphics, Tsinghua University. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Point.h"
#include "Output.h"

std::vector<pPoint_t> pPoints;

int main(int argc, const char * argv[]) {
    pPoint_t Grid[INUM+2][JNUM+2];
    for(int i=0;i<INUM+2;i++) {
        for (int j = 0; j < JNUM + 2; j++) {
            Grid[i][j] = new Point_t;
            auto &pPoint = Grid[i][j];
            pPoints.push_back(pPoint);
            pPoint->I = i;
            pPoint->J = j;
            pPoint->h = 2;
            pPoint->X = i * DELTAX;
            pPoint->Y = j * DELTAY;
            pPoint->ID = pPoints.size() - 1;
        }
    }
    OutputObj(Grid,"result.obj");
    return 0;
}

