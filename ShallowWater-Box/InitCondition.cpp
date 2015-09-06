#include "Macro.h"

void InitCondition(Field_t H, Field_t HOld, Field_t B, Field_t D, Field_t G) {
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			B[i][j] = 0;
			G[i][j] = 1;
		}
	}
	
	for (int i = 0; i < INUM+2; i++) {
		for (int j = 0; j < JNUM+2; j++) {
			H[i][j] = 2;
			HOld[i][j] = 2;
			D[i][j] = 2;
		}
	}
	for (int i = 21; i <= 31; i++) {
		for (int j = 21; j <= 31; j++) {
			H[i][j] = 2.5;
			HOld[i][j] = 2.5;
			D[i][j] = 2.5;
		}
	}


}