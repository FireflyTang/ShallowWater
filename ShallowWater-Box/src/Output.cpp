#include <iosfwd>
#include <fstream>
#include "Macro.h"
#include "Point.h"

void OutputObj(pPoint_t Grid[INUM+2][JNUM+2],std::string fn){
    std::ofstream obj(fn);
    if(!obj.is_open()){
        perror("output error");
        exit(-1);
    }
    obj<<"usemtl"<<"  "<<"Bottom"<<std::endl;
    for(int i=0;i<INUM+2;i++) {
        for (int j = 0; j < JNUM + 2; j++) {
            obj << "v" << "  " << Grid[i][j]->X << " " << 0 << "  " << Grid[i][j]->Y << std::endl;
        }
    }
    for(int i=0;i<INUM+1;i++){
        for(int j = 0; j < JNUM + 1; j++){
            int inext=i+1;
            int jnext=j+1;
            obj<<"f"<<"  "<<GETID(i,j)+1<<" "<<GETID(inext,j)+1<<" "<<GETID(i,jnext)+1<<std::endl;
            obj<<"f"<<"  "<<GETID(inext,jnext)+1<<"  "<<GETID(i,jnext)+1<<"  "<<GETID(inext,j)+1<<std::endl;
        }
    }
    obj<<"usemtl"<<"  "<<"Water"<<std::endl;
    for(int i=0;i<INUM+2;i++) {
        for (int j = 0; j < JNUM + 2; j++) {
            obj << "v" << "  " << Grid[i][j]->X << " " << Grid[i][j]->h << "  " << Grid[i][j]->Y << std::endl;
        }
    }
    for(int i=0;i<INUM+1;i++){
        for(int j = 0; j < JNUM + 1; j++){
            int inext=i+1;
            int jnext=j+1;
            obj<<"f"<<"  "<<GETID(i,j)+POINTNUM+1<<" "<<GETID(inext,j)+POINTNUM+1<<" "<<GETID(i,jnext)+POINTNUM+1<<std::endl;
            obj<<"f"<<"  "<<GETID(inext,jnext)+POINTNUM+1<<"  "<<GETID(i,jnext)+POINTNUM+1<<"  "<<GETID(inext,j)+POINTNUM+1<<std::endl;
        }
    }
    for(int i=0;i<INUM+1;i++){
        int inext=i+1;
        obj<<"f"<<"  "<<GETID(i,0)+1<<"  "<<GETID(inext,0)+1<<"  "<<GETID(i,0)+POINTNUM+1<<std::endl;
        obj<<"f"<<"  "<<GETID(inext,0)+1<<"  "<<GETID(inext,0)+POINTNUM+1<<"  "<<GETID(i,0)+POINTNUM+1<<std::endl;
    }
    for(int i=0;i<INUM+1;i++){
        int inext=i+1;
        obj<<"f"<<"  "<<GETID(i,JNUM+1)+POINTNUM+1<<"  "<<GETID(inext,JNUM+1)+1<<"  "<<GETID(i,JNUM+1)+1<<std::endl;
        obj<<"f"<<"  "<<GETID(i,JNUM+1)+POINTNUM+1<<"  "<<GETID(inext,JNUM+1)+POINTNUM+1<<"  "<<GETID(inext,JNUM+1)+1<<std::endl;
    }
    for(int j=0;j<JNUM+1;j++){
        int jnext=j+1;
        obj<<"f"<<"  "<<GETID(0,j)+1<<"  "<<GETID(0,j)+POINTNUM+1<<"  "<<GETID(0,jnext)+POINTNUM+1<<std::endl;
        obj<<"f"<<"  "<<GETID(0,jnext)+POINTNUM+1<<"  "<<GETID(0,jnext)+1<<"  "<<GETID(0,j)+1<<std::endl;
    }
    for(int j=0;j<JNUM+1;j++) {
        int jnext = j + 1;
        obj<<"f"<<"  "<<GETID(INUM+1,j)+POINTNUM+1<<"  "<<GETID(INUM+1,j)+1<<"  "<<GETID(INUM+1,jnext)+1<<std::endl;
        obj<<"f"<<"  "<<GETID(INUM+1,jnext)+1<<"  "<<GETID(INUM+1,jnext)+POINTNUM+1<<"  "<<GETID(INUM+1,j)+POINTNUM+1<<std::endl;
    }
    obj.close();
}
