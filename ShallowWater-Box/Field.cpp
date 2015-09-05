#include "Macro.h"


void CalcD(Field_t D, Field_t H, Field_t B) {
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			D[i][j] = H[i][j] - B[i][j];
		}
	}
}

void RestoreH(Field_t HOld, Field_t HBackup) {
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			HOld[i][j] = HBackup[i][j];
		}
	}
}

void Boundary(Field_t H) {
	for (int j = 0; j < JNUM + 2; j++) {
		H[0][j] = H[2][j];
		H[INUM + 1][j] = H[INUM - 1][j];
	}
	for (int i = 0; i < INUM + 2; i++) {
		H[i][0] = H[i][2];
		H[i][JNUM + 1] = H[i][JNUM - 1];
	}
	H[0][0] = H[2][2];
	H[0][INUM - 1] = H[2][JNUM - 1];
	H[INUM + 1][0] = H[INUM - 1][2];
	H[INUM + 1][JNUM + 1] = H[INUM - 1][JNUM - 1];
}