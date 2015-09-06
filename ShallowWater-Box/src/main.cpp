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

Grid_t Grid = (Grid_t)(new pPoint_t[(INUM + 2)*(JNUM + 2)]);
Field_t H = (Field_t)(new double[INUM + 2][JNUM + 2]);
Field_t HOld = (Field_t)(new double[INUM + 2][JNUM + 2]);
Field_t D = (Field_t)(new double[INUM + 2][JNUM + 2]);
Field_t B = (Field_t)(new double[INUM + 2][JNUM + 2]);
Field_t G = (Field_t)(new double[INUM + 2][JNUM + 2]);
Matrix_t Ag = (Matrix_t)(new double[POINTNUM][POINTNUM]);
Matrix_t A = (Matrix_t)(new double[POINTNUM][POINTNUM]);
RightB_t RightB = (RightB_t)(new double[POINTNUM]);
pPoints_t & pPoints = *new std::vector<pPoint_t>;


int main(int argc, const char * argv[]) {
	
	Init();
	//计算w
	CalcW(Grid);
	//初始条件
	InitCondition(H, HOld, B, D, G);

	double time = 0;
	for (int step = 0; step < STEPS; step++, time += DELTAT) {
		std::cout << "step = " << step << " time =  " << time << std::endl;
		//输出
		OutputObj(Grid, H, step);
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
		//补全边界条件
		Boundary(H);
	}
	return 0;
}

