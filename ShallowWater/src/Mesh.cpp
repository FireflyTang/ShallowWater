//
//  Mesh.cpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//


#include<CGAL/Polyhedron_incremental_builder_3.h>
#include<CGAL/IO/Polyhedron_iostream.h>
#include "Mesh.hpp"
typedef Mesh_t::HalfedgeDS HalfedgeDS;

// A modifier creating a triangle with the incremental builder.
template<class HDS>
class polyhedron_builder : public CGAL::Modifier_base<HDS> {
public:
    std::vector<double> &coords;
    std::vector<int>    &tris;
    polyhedron_builder( std::vector<double> &_coords, std::vector<int> &_tris ) : coords(_coords), tris(_tris) {}
    void operator()( HDS& hds) {
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        
        // create a cgal incremental builder
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( coords.size()/3, tris.size()/3 );
        
        // add the polyhedron vertices
        for( int i=0; i<(int)coords.size(); i+=3 ){
            B.add_vertex( Point( coords[i+0], coords[i+1], coords[i+2] ) );
        }
        
        // add the polyhedron triangles
        for( int i=0; i<(int)tris.size(); i+=3 ){
            B.begin_facet();
            B.add_vertex_to_facet( tris[i+0] );
            B.add_vertex_to_facet( tris[i+1] );
            B.add_vertex_to_facet( tris[i+2] );
            B.end_facet();
        }
        
        // finish up the surface
        B.end_surface();
    }
};

// reads the first integer from a string in the form
// "334/455/234" by stripping forward slashes and
// scanning the result
int get_first_integer( const char *v ){
    int ival;
    std::string s( v );
    std::replace( s.begin(), s.end(), '/', ' ' );
    sscanf( s.c_str(), "%d", &ival );
    return ival;
}

// barebones .OFF file reader, throws away texture coordinates, normals, etc.
// stores results in input coords array, packed [x0,y0,z0,x1,y1,z1,...] and
// tris array packed [T0a,T0b,T0c,T1a,T1b,T1c,...]
void load_obj( const char *filename, std::vector<double> &coords, std::vector<int> &tris ){
    double x, y, z;
    char line[1024], v0[1024], v1[1024], v2[1024];
    
    // open the file, return if open fails
    FILE *fp = fopen(filename, "r" );
    if( !fp ) return;
    
    // read lines from the file, if the first character of the
    // line is 'v', we are reading a vertex, otherwise, if the
    // first character is a 'f' we are reading a facet
    while( fgets( line, 1024, fp ) ){
        if( line[0] == 'v' && line[1]==' '){
            sscanf( line, "%*s%lf%lf%lf", &x, &y, &z );
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        } else if( line[0] == 'f' ){
            sscanf( line, "%*s%s%s%s", v0, v1, v2 );
            tris.push_back( get_first_integer( v0 )-1 );
            tris.push_back( get_first_integer( v1 )-1 );
            tris.push_back( get_first_integer( v2 )-1 );
        }
    }
    fclose(fp);
}

int LoadMesh(Mesh_t & P,char * fn){
    // two vectors to hold point coordinates and
    // triangle vertex indices
    std::vector<double> coords;
    std::vector<int>    tris;
    
    // load the input file
    load_obj(fn, coords, tris );
    if( coords.size() == 0 ){
        perror("no vertexes");
        exit(-1);
    }
    
    // build a polyhedron from the loaded arrays
    
    polyhedron_builder<HalfedgeDS> builder( coords, tris );
    P.delegate( builder );
    return P.size_of_vertices();
}

int BuildMesh(Mesh_t & Mesh, AABBTree_t & AABBTree){
	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++)
	{
		Point_3 points[3];
		Point_2 points2[3];
		double x = 0, y = 0, z = 0;
		auto h = f->facet_begin();
		for (int i = 0; i<3; i++){
			x += h->vertex()->point().x();
			y += h->vertex()->point().y();
			z += h->vertex()->point().z();
			points[i] = h->vertex()->point();
			h++;
		}
		Point_3 center(x / 3, y / 3, z / 3);
		f->Center = center;
		Vector_3 normal = Plane_3(points[0], points[1], points[2]).orthogonal_vector();

		normal = NORMALIZE(normal);
		Line_3 line(center, center + normal);

		typedef boost::optional<AABBTree_t::Intersection_and_primitive_id<Line_3>::Type> intersection_t;
		std::vector<intersection_t> intersections;
		AABBTree.all_intersections(line, std::back_inserter(intersections));
		double min = DBL_MAX;
		AABBFacet_t * pAABBFacet = nullptr;
		Point_3 * p;
		for (auto i : intersections){
			if ((p = boost::get<Point_3>(&(i->first)))){
				if (sqrt(CGAL::squared_distance(center, *p))<min){
					min = sqrt(CGAL::squared_distance(center, *p));
					pAABBFacet = i->second;
				}
			}
		}
		assert(min<0.1);
		//Vector_3 ref(0, 1, 0);
		if (normal*pAABBFacet->Normal<0) normal = -normal;
		f->Normal = normal;

		Triangle_3 Triangle(points[0], points[1], points[2]);
		f->Triangle = Triangle;

		double maxlength[3];
		for (int m = 0; m < 3; m++){
			maxlength[m] = 0;
			for (int i = 0; i < 3; i++){
				int j = (i + 1) % 3;
				maxlength[m] = std::max(maxlength[m], std::abs(Triangle[i][m] - Triangle[j][m]));
			}
		}
		f->ProjDirec = -1;


		if ((std::abs(normal[0])) >= (std::abs(normal[1])) && (std::abs(normal[0])) >= (std::abs(normal[2]))) f->ProjDirec = 0;
		if ((std::abs(normal[1])) >= (std::abs(normal[0])) && (std::abs(normal[1])) >= (std::abs(normal[2]))) f->ProjDirec = 1;
		if ((std::abs(normal[2])) >= (std::abs(normal[1])) && (std::abs(normal[2])) >= (std::abs(normal[0]))) f->ProjDirec = 2;

		//if ((maxlength[0] <= maxlength[1]) && (maxlength[0] <= maxlength[2])) f->ProjDirec = 0;
		//if ((maxlength[1] <= maxlength[2]) && (maxlength[1] <= maxlength[0])) f->ProjDirec = 1;
		//if ((maxlength[2] <= maxlength[1]) && (maxlength[2] <= maxlength[0])) f->ProjDirec = 2;

		assert(f->ProjDirec != -1);
		for (int m = 0; m < 3; m++){
			Point_2 points2[3];
			for (int i = 0; i < 3; i++){
				if (m == 0){
					Point_2 temp(Triangle[i].y(), Triangle[i].z());
					points2[i] = temp;
				}
				if (m == 1){
					Point_2 temp(Triangle[i].x(), Triangle[i].z());
					points2[i] = temp;
				}
				if (m == 2){
					Point_2 temp(Triangle[i].x(), Triangle[i].y());
					points2[i] = temp;
				}
			}
			auto v1 = points2[1] - points2[0];
			auto v2 = points2[2] - points2[0];
			if (CGAL::determinant(v1, v2) == 0){
				Triangle_2 temp(points2[0], points2[2], points2[1]);
				f->Triangle2[m] = temp;
				assert(temp.is_degenerate());
			}
			if (CGAL::determinant(v1, v2) < 0){
				Triangle_2 temp(points2[0], points2[2], points2[1]);
				f->Triangle2[m] = temp;
				assert(!temp.is_degenerate());
				assert(CGAL::determinant(temp[1] - temp[0], temp[2] - temp[0])>0);
			}
			if (CGAL::determinant(v1, v2) > 0){
				Triangle_2 temp(points2[0], points2[1], points2[2]);
				f->Triangle2[m] = temp;
				assert(CGAL::determinant(temp[1] - temp[0], temp[2] - temp[0])>0);
				assert(!temp.is_degenerate());
			}
		}
	}
	return 1;
}
