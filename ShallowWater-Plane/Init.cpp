#include "Point.h"
#include "Macro.h"

extern Grid_t Grid;
extern Field_t H;
extern Field_t HOld;
extern Field_t B;
extern Field_t D;
extern Field_t G;
extern Matrix_t A;
extern Matrix_t Ag;
extern RightB_t RightB;
extern pPoints_t & pPoints;

void Init() {
	pPoints.clear();
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			Grid[i][j] = new Point_t;
			auto &pPoint = Grid[i][j];
			pPoints.push_back(pPoint);
			pPoint->I = i;
			pPoint->J = j;
			Point_2 temp(i * DELTAX, j * DELTAY);
			pPoint->Position = temp;
			pPoint->ID = pPoints.size() - 1;
			pPoint->Neighbours.clear();
			pPoint->w.clear();
			pPoint->alpha.clear();
			pPoint->beta.clear();
			pPoint->phi.clear();
		}
	}

//初始化
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			B[i][j] = 0;
		}
	}
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			H[i][j] = 0;
		}
	}
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			HOld[i][j] = 0;
		}
	}
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			G[i][j] = 0;
		}
	}
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			D[i][j] = 0;
		}
	}

	for (int Ai = 0; Ai < POINTNUM; Ai++) {
		for (int Aj = 0; Aj < POINTNUM; Aj++) {
			Ag[Ai][Aj] = 0;
		}
	}
	for (int Ai = 0; Ai < POINTNUM; Ai++) {
		for (int Aj = 0; Aj < POINTNUM; Aj++) {
			A[Ai][Aj] = 0;
		}
	}
	for (int k = 0; k < POINTNUM; k++) {
		RightB[k] = 0;
	}
}