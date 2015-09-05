#include "Solver.h"
#include "Macro.h"


void Solve(Field_t H, Matrix_t A, RightB_t RightB) {
	double AA[INUM * JNUM][INUM * JNUM];
	for (int j = 1; j <= JNUM; j++) {
		int AID = GETID(2, j);
		int BID = GETID(0, j);
		for (int k = 0; k < POINTNUM; k++)
			A[k][AID] += A[k][BID];
	}
	for (int j = 1; j <= JNUM; j++) {
		int AID = GETID(INUM - 1, j);
		int BID = GETID(INUM + 1, j);
		for (int k = 0; k < POINTNUM; k++)
			A[k][AID] += A[k][BID];
	}
	for (int i = 1; i <= INUM; i++) {
		int AID = GETID(i, 2);
		int BID = GETID(i, 0);
		for (int k = 0; k < POINTNUM; k++)
			A[k][AID] += A[k][BID];
	}
	for (int i = 1; i <= INUM; i++) {
		int AID = GETID(i, JNUM + 1);
		int BID = GETID(i, JNUM - 1);
		for (int k = 0; k < POINTNUM; k++)
			A[k][AID] += A[k][BID];
	}
	int AID = GETID(2, 2);
	int BID = GETID(0, 0);
	for (int k = 0; k < POINTNUM; k++)
		A[k][AID] += A[k][BID];
	AID = GETID(2, JNUM - 1);
	BID = GETID(0, JNUM + 1);
	for (int k = 0; k < POINTNUM; k++)
		A[k][AID] += A[k][BID];
	AID = GETID(INUM - 1, 2);
	BID = GETID(INUM + 1, 0);
	for (int k = 0; k < POINTNUM; k++)
		A[k][AID] += A[k][BID];
	AID = GETID(INUM - 1, JNUM - 1);
	BID = GETID(INUM + 1, JNUM + 1);
	for (int k = 0; k < POINTNUM; k++)
		A[k][AID] += A[k][BID];
	for (int i = 0; i < INUM * JNUM; i++) {
		for (int j = 0; j < INUM * JNUM; j++) {
			AA[i][j] = A[OuterID(i)][OuterID(j)];
		}
	}

	double BB[INUM * JNUM];
	for (int k = 0; k < INUM * JNUM; k++)
		BB[k] = RightB[OuterID(k)];

	double HH[INUM * JNUM];
	for (int i = 1; i <= INUM; i++) {
		for (int j = 1; j <= JNUM; j++) {
			H[i][j] = HH[GETID(i, j)];
		}
	}
}