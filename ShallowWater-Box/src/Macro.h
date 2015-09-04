
const int INUM = 2,JNUM = 1;
const double DELTAX = 1,DELTAY =1;
const double XLENGTH=DELTAX*(INUM-1), YLENGTH=DELTAX*(JNUM-1);
const int POINTNUM = (INUM+2)*(JNUM+2);

#define GETID(i,j) ((i)*(JNUM+2)+(j))
#define GETI(id) (id/(JNUM+2))
#define GETJ(id) (id%(JNUM+2))