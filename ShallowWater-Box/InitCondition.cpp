#include "Macro.h"

void InitCondition(Field_t H, Field_t HOld, Field_t B, Field_t D, Field_t G) {
	for (int i = 0; i < INUM; i++) {
		for (int j = 0; j < JNUM; j++) {
			H[i][j] = 2;
			HOld[i][j] = 2;
			B[i][j] = 0;
			D[i][j] = 2;
			G[i][j] = 1;
		}
	}
}