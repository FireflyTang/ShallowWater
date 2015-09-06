#pragma once

#include "Point.h"

const int INUM =30,JNUM = 30;
const double DELTAX = 0.1,DELTAY =0.1;
const int STEPS = 2000;
const double DELTAT =0.001;


const double XLENGTH=DELTAX*(INUM-1), YLENGTH=DELTAX*(JNUM-1);
const int POINTNUM = (INUM+2)*(JNUM+2);
const double AREA = DELTAX * DELTAY;
const double TAU = 0;

#define GETID(i,j) ((i)*(JNUM+2)+(j))
#define GETI(id) (id/(JNUM+2))
#define GETJ(id) (id%(JNUM+2))
#define EPSILON 1e-10
#define NORMALIZE(x) ((x)/(sqrt((x).squared_length())))
#define PI 3.1415926535897932384626
#define OuterID(x) (GETID(((x)/JNUM+1),(x)%JNUM+1))

typedef double (*Field_t)[JNUM + 2];
typedef std::vector<pPoint_t> pPoints_t;
typedef pPoint_t(*Grid_t)[INUM + 2];
typedef double (*Matrix_t)[POINTNUM];
typedef double * RightB_t;
