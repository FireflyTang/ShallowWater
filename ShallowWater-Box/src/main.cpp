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

#include "Init.h"
#include "Equation.h"
#include "Grid.h"
#include "InitCondition.h"
#include "Field.h"
#include "Solver.h"

std::vector<pPoint_t> pPoints;
pPoint_t Grid[INUM + 2][JNUM + 2];
double H[INUM + 2][JNUM + 2];
double HOld[INUM + 2][JNUM + 2];
double D[INUM + 2][JNUM + 2];
double B[INUM + 2][JNUM + 2];
double G[INUM + 2][JNUM + 2];
double Ag[POINTNUM][POINTNUM];
double A[POINTNUM][POINTNUM];
double RightB[POINTNUM];

int main(int argc, const char * argv[]) {
	Init();
	//计算w
	CalcW(Grid);
	//初始条件
	InitCondition(H, HOld, B, D, G);

	double time = 0;
	for (int step = 0; step < STEPS; step++, time += DELTAT) {
		std::cout << "step = " << step << " time =  " << time << std::endl;
		//计算深度
		CalcD(D, H, B);
		//计算Ag
		CalcAg(Ag, Grid, pPoints, D, G);
		//计算A
		CalcA(A, Ag);
		//计算右边b
		CalcRightB(RightB, H, HOld);
		//HOld = H
		RestoreH(HOld, H);
		//解方程
		Solve(H, A, RightB);
		//修改边界条件
		Boundary(H);
		//输出
		OutputObj(Grid, H, step);
	}
	return 0;
}

