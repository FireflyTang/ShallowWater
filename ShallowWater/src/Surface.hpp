//
//  Surface.hpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include "Macro.h"
#include "AABBTree.hpp"
#include "Mesh.hpp"


int BuildMesh(Mesh_t & Mesh,AABBTree_t & AABBTree){
    for(auto f = Mesh.facets_begin();f!=Mesh.facets_end();f++)
    {
        Point_3 points[3];
		Point_2 points2[3];
        double x=0,y=0,z=0;
        auto h=f->facet_begin();
        for(int i=0;i<3;i++){
            x+=h->vertex()->point().x();
            y+=h->vertex()->point().y();
            z+=h->vertex()->point().z();
            points[i]=h->vertex()->point();
            h++;
        }
        Point_3 center(x/3,y/3,z/3);
        f->Center=center;
        Vector_3 normal=Plane_3(points[0],points[1],points[2]).orthogonal_vector();
	
        normal=NORMALIZE(normal);
        Line_3 line(center,center+normal);

        typedef boost::optional<AABBTree_t::Intersection_and_primitive_id<Line_3>::Type> intersection_t;
        std::vector<intersection_t> intersections;
        AABBTree.all_intersections(line,std::back_inserter(intersections));
        double min=DBL_MAX;
        AABBFacet_t * pAABBFacet=nullptr;
        Point_3 * p;
        for(auto i:intersections){
            if((p=boost::get<Point_3>(&(i->first)))){
                if(sqrt(CGAL::squared_distance(center,*p))<min){
                    min=sqrt(CGAL::squared_distance(center,*p));
                    pAABBFacet=i->second;
                }
            }
        }
        assert(min<0.1);
		//Vector_3 ref(0, 1, 0);
        if(normal*pAABBFacet->Normal<0) normal=-normal;
        f->Normal=normal;	

		Triangle_3 Triangle(points[0], points[1], points[2]);
		f->Triangle = Triangle;

		double maxlength[3];
		for (int m = 0; m < 3; m++){
			maxlength[m] = 0;
			for (int i = 0; i < 3; i++){
				int j = (i + 1) % 3;
				maxlength[m] = std::max(maxlength[m],std::abs(Triangle[i][m]-Triangle[j][m]));
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
				assert(CGAL::determinant(temp[1]-temp[0],temp[2]-temp[0])>0);
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

#endif /* Surface_hpp */
