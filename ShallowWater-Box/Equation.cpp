#include "Macro.h"

#include  "Point.h"
#include  "Equation.h"


void CalcRightB(RightB_t RightB, Field_t H, Field_t HOld) {
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			RightB[GETID(i, j)] = H[i][j] + (1 - TAU) * (H[i][j] - HOld[i][j]);
		}
	}
}


void CalcAg(Matrix_t Ag, Grid_t Grid, pPoints_t &pPoints, Field_t D, Field_t G) {
	for (int Ai = 0; Ai < POINTNUM; Ai++) {
		auto &PointI = pPoints[Ai];
		for (int k = 0; k < PointI->Neighbours.size(); k++) {
			auto &PointJ = PointI->Neighbours[k];
			int Aj = PointJ->ID;
			Ag[Ai][Aj] = -PointI->w[k] * (D[0][Ai] + D[0][Aj]) * (G[0][Ai] + G[0][Aj]);
			Ag[Ai][Aj] /= 8 * AREA;
		}
	}
	for (int Ai = 0; Ai < POINTNUM; Ai++) {
		auto &PointI = pPoints[Ai];
		for (int k = 0; k < PointI->Neighbours.size(); k++) {
			auto &PointJ = PointI->Neighbours[k];
			int Aj = PointJ->ID;
			Ag[Ai][Ai] += PointI->w[k] * (D[0][Ai] + D[0][Aj]) * (G[0][Ai] + G[0][Aj]);
		}
		Ag[Ai][Ai] /= 8 * AREA;
	}
}

void CalcA(Matrix_t A, Matrix_t Ag) {
	for (int i = 0; i < POINTNUM; i++) {
		for (int j = 0; j < POINTNUM; j++) {
			A[i][j] = Ag[i][j];
		}
	}
	for (int k = 0; k < POINTNUM; k++) {
		A[k][k] += 1;
	}
}

