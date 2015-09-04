#import <vector>

class Point_t;
typedef Point_t * pPoint_t;
class Point_t{
public:
    int I,J;
    double X,Y;
    double h;
    int ID;
    std::vector<pPoint_t> Neighbours;
    std::vector<double> w,alpha,beta;
};