//
//  AABBTree.cpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "AABBTree.hpp"



const
Facet_primitive::Id &
Facet_primitive::
id() const {return m_pt;};

    const
Facet_primitive::Datum &
Facet_primitive::
datum() const {return m_pt->Triangle;};

    const
Facet_primitive::Point &
Facet_primitive::
 reference_point() const {return m_pt->Triangle[0];};


int LoadAABBTree(std::vector<AABBFacet_t> & facets,char * fn){
    double x, y, z;
    char line[1024], c0[1024], c1[1024], c2[1024];
    
    std::vector<Point_3> points;
    std::vector<Vector_3> normals;
    FILE *fp = fopen(fn, "r" );
    if( !fp ) {
        perror("file error");
        exit(-1);
    }
    
    while( fgets( line, 1024, fp ) ){
        if( line[0] == 'v' && line[1]==' '){
            sscanf( line, "%*s%lf%lf%lf", &x, &y, &z );
            points.push_back(Point_3(x,y,z));
            continue;
        }
        if(line[0]=='v' && line[1]=='n' ){
            sscanf( line, "%*s%lf%lf%lf", &x, &y, &z );
            normals.push_back(Vector_3(x,y,z));
            continue;
        }
        if( line[0] == 'f' ){
            sscanf( line, "%*s%s%s%s", c0, c1, c2 );
            int v0,v1,v2,n;
            Vector_3 normal(0,0,0);
            AABBFacet_t facet;
            std::string s0(c0);std::replace(s0.begin(), s0.end(), '/',' ');sscanf(s0.data(), "%d%*d%d",&v0,&n);
            normal=normal+normals[n-1];
            std::string s1(c1);std::replace(s1.begin(), s1.end(), '/',' ');sscanf(s1.data(), "%d%*d%d",&v1,&n);
            normal=normal+normals[n-1];
            std::string s2(c2);std::replace(s2.begin(), s2.end(), '/',' ');sscanf(s2.data(), "%d%*d%d",&v2,&n);
            normal=normal+normals[n-1];
			assert(v0  <= points.size());
			assert(v1  <=  points.size());
			assert(v2  <=  points.size());
            Triangle_3 triangle(points[v0-1],points[v1-1],points[v2-1]);
            facet.Triangle=triangle;
            assert(!triangle.is_degenerate());
            facet.Normal=NORMALIZE(normal/3);
            facets.push_back(facet);
            continue;
        }
    }
    fclose(fp);
	return facets.size();
}