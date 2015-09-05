#import <vector>
#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Vector_2 Vector_2;

class Point_t;
typedef Point_t * pPoint_t;
class Point_t{
public:
    int I,J;
    Point_2 Position;
    unsigned long ID;
    std::vector<pPoint_t> Neighbours;
    std::vector<double> w, alpha, beta, phi;
};