#ifndef Mesh_hpp
#define Mesh_hpp

#include "Macro.h"
#include "AABBTree.hpp"

class Point_t;
typedef Point_t * pPoint_t;

template <class Refs>
struct Facet_t : public CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane_3> {
	Vector_3 Normal;
	std::list<pPoint_t> PointsOnFacet;
	Point_3 Center;
	Triangle_3 Triangle;
	Triangle_2 Triangle2[3];
	int ProjDirec;
};

struct Items3_t : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Face_wrapper {
        typedef Facet_t<Refs> Face;
    };
};

typedef CGAL::Polyhedron_3<Kernel, Items3_t>  Mesh_t;

enum Type_t{
    WET,
    DRT,
    BOUND
};

class Point_t{
public:
    Point_t():Force(Vector_3(0,0,0)),Normal(Vector_3(0,0,0)){};
    double h;
    double b;
    Point_3 Position;
	//Point_2 Position2;
    std::vector<pPoint_t> Neighbours;
    std::vector<double> w;
    std::vector<double> beta;
    std::vector<double> alpha;
    std::vector<double> phi;
    Vector_3 Force;
	//Vector_2 Force2;
    Type_t Type;
    Vector_3 Normal;
    Mesh_t::Facet_handle Facet;
	int ID;
	Vector_3 DeltaX;
};

int LoadMesh(Mesh_t & P,char * fn);
int BuildMesh(Mesh_t & Mesh, AABBTree_t & AABBTree);
#endif /* Mesh_hpp */