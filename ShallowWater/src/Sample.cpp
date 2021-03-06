//
//  Sample.cpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#include "Sample.hpp"
#include <algorithm>
#include <math.h>
class compdistance_t{
public:
	compdistance_t(pPoint_t _Point) :Point(_Point){};
	bool operator() (const pPoint_t & a, const pPoint_t & b){
		//auto v1 = a->Position - Point->Position;
		//auto v2 = b->Position - Point->Position;
		//auto v12 = Vector_2(v1.x(), v1.z());
		//v12 = NORMALIZE(v12);
		//auto v22 = Vector_2(v2.x(), v2.z());
		//v22 = NORMALIZE(v22);
		//double c;
		//c = v12.x(); if (c>1) c = 1; if (c<-1) c = -1;
		//double angle1 = acos(c);
		//c = v22.x(); if (c>1) c = 1; if (c<-1) c = -1;
		//double angle2 = acos(c);
		//if (v12.y()<0)
		//	angle1 = M_PI * 2 - angle1;
		//if (v22.y()<0)
		//	angle2 = M_PI * 2 - angle2;
		//if (std::abs(angle1 - angle2) < DELTA) {
		if (CGAL::squared_distance(a->Position, Point->Position) < CGAL::squared_distance(b->Position, Point->Position))
			return true;
		else
			return false;
		//}
		//else{
		//	return angle1 < angle2;
		//}
	}
	pPoint_t Point;
};

class compangle_t{
public:
	compangle_t(pPoint_t _Point, pPoint_t _ref) :Point(_Point), ref(_ref){
		refv = ref->Position - Point->Position;
		refv = refv - refv*Point->Normal*Point->Normal;
		refv = NORMALIZE(refv);
	};
	bool operator() (const pPoint_t & a, const pPoint_t & b){
		auto v1 = a->Position - Point->Position;
		auto v2 = b->Position - Point->Position;
		v1 = v1 - v1*Point->Normal*Point->Normal;
		v2 = v2 - v2*Point->Normal*Point->Normal;
		v1 = NORMALIZE(v1);
		v2 = NORMALIZE(v2);
		double c;
		c = v1*refv; if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
		double angle1 = acos(c);
		c = v2*refv; if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
		double angle2 = acos(c);
		if (CGAL::cross_product(refv, v1)*Point->Normal < 0) angle1 = 2 * M_PI - angle1;
		if (CGAL::cross_product(refv, v2)*Point->Normal < 0) angle2 = 2 * M_PI - angle2;
		return angle1 <= angle2;
	}
	pPoint_t Point;
	pPoint_t ref;
	Vector_3 refv;
};

Point_2 Project2(const Point_3 & Point, int ProjDirec){
	if (ProjDirec == 0){
		Point_2 temp(Point.y(), Point.z());
		return temp;
	}
	if (ProjDirec == 1){
		Point_2 temp(Point.x(), Point.z());
		return temp;
	}
	if (ProjDirec == 2){
		Point_2 temp(Point.x(), Point.y());
		return temp;
	}
}

const int ProjX = 0;
const int ProjY = 1;
const int ProjZ = 2;

bool IsIn2D(const Point_3 & Point, const Mesh_t::Facet_handle & ProjFace, const Mesh_t::Facet_handle & TriangleFace, Point_3 & ProjPoint){
	Plane_3 plane = ProjFace->Triangle.supporting_plane();
	ProjPoint = plane.projection(Point);
	assert(!isnan(ProjPoint.x()));
	Point_2 ProjPoint2 = Project2(ProjPoint, ProjFace->ProjDirec);
	Point_3 ProjPoints[3];
	for (int i = 0; i < 3; i++){
		ProjPoints[i] = plane.projection(TriangleFace->Triangle[i]);
	}
	Point_2 Projpoints2[3];
	for (int i = 0; i < 3; i++){
		Projpoints2[i] = Project2(ProjPoints[i], ProjFace->ProjDirec);
	}
	Triangle_2 ProjTriangle2(Projpoints2[0], Projpoints2[1], Projpoints2[2]);
	assert(!ProjTriangle2.is_degenerate());
	return ProjTriangle2.has_on_bounded_side(ProjPoint2) || ProjTriangle2.has_on_boundary(ProjPoint2);
}

bool IsIn3D(const Point_3 & Point, const Mesh_t::Facet_handle & Face, Point_3 & ProjPoint){
	assert(!isnan(Point.x()));
	assert(!Face->Triangle2[Face->ProjDirec].is_degenerate());
	Plane_3 plane = Face->Triangle.supporting_plane();
	ProjPoint = plane.projection(Point);
	if (isnan(ProjPoint.x())){
		std::cout << ProjPoint << std::endl;
		std::cout << Point << std::endl;
		std::cout << plane << std::endl;
		std::cout << 1;
	}
	assert(Point.x()>-100);
	assert(!isnan(ProjPoint.x()));
	return Face->Triangle2[Face->ProjDirec].has_on_bounded_side(Project2(ProjPoint, Face->ProjDirec)) || Face->Triangle2[Face->ProjDirec].has_on_boundary(Project2(ProjPoint, Face->ProjDirec));
}

Point_3 CalcPosition(const Triangle_3 & Triangle, double first, double second, int unknown){
	Plane_3 plane = Triangle.supporting_plane();
	double a = plane.a();
	double b = plane.b();
	double c = plane.c();
	double d = plane.d();
	double third;
	switch (unknown)
	{
	case(ProjX) : {
		third = (-d - first*b - second*c) / a;
		Point_3 temp(third, first, second);
		return temp;
		break;
	}
	case(ProjY) : {
		third = (-d - first*a - second*c) / b;
		Point_3 temp(first, third, second);
		return temp;
		break;
	}
	case(ProjZ) : {
		third = (-d - first*a - second*b) / c;
		Point_3 temp(first, second, third);
		return temp;
		break;
	}
	default:
		assert(false);
		break;
	}
}

//class remove_t{
//public:
//    remove_t(pPoint_t _Point):Point(_Point){};
//	bool operator()(const pPoint_t & a, const pPoint_t & b){
//		auto v1 = a->Position - Point->Position;
//		auto v2 = b->Position - Point->Position;
//		auto v12 = Vector_2(v1.x(), v1.z());
//		v12 = NORMALIZE(v12);
//		auto v22 = Vector_2(v2.x(), v2.z());
//		v22 = NORMALIZE(v22);
//		double c;
//		c = v12.x(); if (c>1) c = 1; if (c<-1) c = -1;
//		double angle1 = acos(c);
//		c = v22.x(); if (c>1) c = 1; if (c<-1) c = -1;
//		double angle2 = acos(c);
//		if (v12.y()<0)
//			angle1 = M_PI * 2 - angle1;
//		if (v22.y()<0)
//			angle2 = M_PI * 2 - angle2;
//		return std::abs(angle1-angle2)<0.1;
//    }
//    pPoint_t Point;
//};

void Sample(std::vector<pPoint_t> & pPoints, Mesh_t & Mesh){
	srand((unsigned)time(NULL));
	for (auto v = Mesh.vertices_begin(); v != Mesh.vertices_end(); v++)
	{
		pPoints.push_back(new Point_t);
		auto iPoint = pPoints.back();
		iPoint->Position = v->point();
		Mesh_t::Facet_handle f = nullptr;
		auto h = v->vertex_begin();
		do{
			if (!h->is_border())
				f = h->facet();
		} while (++h != v->vertex_begin());
		iPoint->Facet = &(*f);
		iPoint->Normal = f->Normal;
		iPoint->ID = pPoints.size() - 1;
		f->PointsOnFacet.push_back(&(*iPoint));
	}
	//auto f = Mesh.facets_begin();
	//for (; f != Mesh.facets_end(); f++){
	//	int p = rand() % 100;
	//	//if (p > 30){
	//	pPoints.push_back(new Point_t);
	//	auto iPoint = pPoints.back();
	//	iPoint->Position = f->Center;
	//	iPoint->Facet = &(*f);
	//	iPoint->Normal = f->Normal;
	//	f->PointsOnFacet.push_back(&(*iPoint));
	//	iPoint->ID = pPoints.size() - 1;
	//	//}
	//}
	//f--;
	//Point_3 O(0, -1, 0);
	//pPoints.push_back(new Point_t);
	//auto iPoint = pPoints.back();
	//iPoint->Position = O;
	//iPoint->Facet = &(*f);
	//iPoint->Normal = f->Normal;
	//f->PointsOnFacet.push_back(&(*iPoint));
	//Point_3 O2(1, -1, 1.2);
	//pPoints.push_back(new Point_t);
	//iPoint = pPoints.back();
	//iPoint->Position = O2;
	//iPoint->Facet = &(*f);
	//iPoint->Normal = f->Normal;
	//f->PointsOnFacet.push_back(&(*iPoint));
}
double Move(std::vector<pPoint_t> & pPoints, Mesh_t & Mesh, double a, double DeltaT){
	double res = 0;
	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++){//每个面
		if (f->PointsOnFacet.empty()) continue;
		for (auto iPoint : f->PointsOnFacet){//每个面上的点
			iPoint->w.clear();
			iPoint->alpha.clear();
			iPoint->beta.clear();
			iPoint->phi.clear();
			iPoint->Neighbours.clear();

			compdistance_t compdistance(&(*iPoint));
			//remove_t remove(&(*iPoint));
			//reEnd_t reEnd(&(*iPoint));
			auto h = f->facet_begin();//每条边
			do{
				auto v = h->vertex();//每个顶点
				auto hh = v->vertex_begin();//每个边
				do{
					assert(!hh->is_border_edge());
					if (!hh->is_border()){
						auto ff = hh->facet();//每个边对应的面
						iPoint->Neighbours.insert(iPoint->Neighbours.end(), ff->PointsOnFacet.begin(), ff->PointsOnFacet.end());
					}
					auto vv = hh->opposite()->vertex();//第二圈顶点
					auto hhh = vv->vertex_begin();//第二圈边
					do{
						assert(!hhh->is_border_edge());
						if (!hhh->is_border()){
							auto fff = hhh->facet();
							iPoint->Neighbours.insert(iPoint->Neighbours.end(), fff->PointsOnFacet.begin(), fff->PointsOnFacet.end());
						}
					} while (++hhh != vv->vertex_begin());
				} while (++hh != v->vertex_begin());
			} while (++h != f->facet_begin());

			//删除重复元素
			std::sort(iPoint->Neighbours.begin(), iPoint->Neighbours.end());
			auto last = std::unique(iPoint->Neighbours.begin(), iPoint->Neighbours.end());
			iPoint->Neighbours.erase(last, iPoint->Neighbours.end());
			//删除自己
			iPoint->Neighbours.erase(std::find(iPoint->Neighbours.begin(), iPoint->Neighbours.end(), iPoint));
			//按照距离排序
			std::sort(iPoint->Neighbours.begin(), iPoint->Neighbours.end(), compdistance);

			/*auto last2 = std::unique(iPoint->Neighbours.begin(), iPoint->Neighbours.end(), remove);
			iPoint->Neighbours.erase(last2, iPoint->Neighbours.end());*/
			//把多余6个的删除
			assert(iPoint->Neighbours.size() >= 6);
			iPoint->Neighbours.erase(iPoint->Neighbours.begin() + 6, iPoint->Neighbours.end());
		}
	}

	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++){//每个面
		if (f->PointsOnFacet.empty()) continue;
		for (auto iPoint : f->PointsOnFacet){//每个面上的点
			for (auto neighbour : iPoint->Neighbours){
				if (neighbour->Neighbours.end() == std::find(neighbour->Neighbours.begin(), neighbour->Neighbours.end(), iPoint)){
					neighbour->Neighbours.push_back(iPoint);
				}
			}
			assert(iPoint->Neighbours.size() >= 6);
		}
	}

	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++){//每个面
		for (auto iPoint : f->PointsOnFacet){//每个面上的点
			compangle_t compangle(&(*iPoint), iPoint->Neighbours[0]);
			std::sort(iPoint->Neighbours.begin() + 1, iPoint->Neighbours.end(), compangle);
			int i = 0;
			while (i<iPoint->Neighbours.size()){
				int j = (i + 1) % iPoint->Neighbours.size();
				auto vi = iPoint->Neighbours[i]->Position - iPoint->Position;
				auto vj = iPoint->Neighbours[j]->Position - iPoint->Position;
				double c = NORMALIZE(vi)*NORMALIZE(vj); if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
				double angle = acos(c);
				if (angle < ((double)10 / 180)*M_PI){
					int n;
					if (vi.squared_length() >= vj.squared_length()) n = i;
					if (vi.squared_length() < vj.squared_length()) n = j;
					auto last = std::find(iPoint->Neighbours[n]->Neighbours.begin(), iPoint->Neighbours[n]->Neighbours.end(), iPoint);
					if (last != iPoint->Neighbours[n]->Neighbours.end())
						iPoint->Neighbours[n]->Neighbours.erase(last);
					iPoint->Neighbours.erase(iPoint->Neighbours.begin() + n);
				}
				else{
					i++;
				}
			}
		}
	}
	
	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++){//每个面
		if (f->PointsOnFacet.empty()) continue;
		for (auto iPoint : f->PointsOnFacet){//每个面上的点
			compangle_t compangle(&(*iPoint), iPoint->Neighbours[0]);
			std::sort(iPoint->Neighbours.begin() + 1, iPoint->Neighbours.end(), compangle);
			auto & Neighbours = iPoint->Neighbours;
			for (int i = 0; i < Neighbours.size(); i++){
				int next = (i + 1) % Neighbours.size();
				int prev = (i - 1 + Neighbours.size()) % Neighbours.size();
				double c;
				assert(Neighbours[i]->Position != iPoint->Position);
				assert(Neighbours[prev]->Position != iPoint->Position);
				assert(Neighbours[next]->Position != iPoint->Position);
				c = NORMALIZE(Neighbours[i]->Position - iPoint->Position)*NORMALIZE(Neighbours[next]->Position - iPoint->Position); if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
				double phi = acos(c);
				if (isnan(phi)){
					std::cout << 1;
				}
				assert(!isnan(phi));
				c = NORMALIZE(Neighbours[i]->Position - Neighbours[next]->Position)*NORMALIZE(iPoint->Position - Neighbours[next]->Position); if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
				double beta = acos(c);
				//if (isnan(beta)){
				//	std::cout << 1;
				//}
				assert(!isnan(beta));
				c = NORMALIZE(Neighbours[i]->Position - Neighbours[prev]->Position)*NORMALIZE(iPoint->Position - Neighbours[prev]->Position); if (c > 1) c = 1 - EPSILON; if (c < -1) c = -1 + EPSILON;
				double alpha = acos(c);
				//if (isnan(alpha)){
				//	std::cout << 1;
				//}
				assert(!isnan(alpha));
				iPoint->phi.push_back(phi);
				iPoint->beta.push_back(beta);
				iPoint->alpha.push_back(alpha);
			}
			for (int i = 0; i < Neighbours.size(); i++){
				double w;
				double alpha = iPoint->alpha[i];
				double beta = iPoint->beta[i];
				double t1, t2;
				t1 = cos(alpha) / sin(alpha);
				t2 = cos(beta) / sin(beta);
				w = t1 + t2;
				if (w>100000){
					//int next = (i + 1) % Neighbours.size();
					//int prev = (i - 1 + Neighbours.size()) % Neighbours.size();
					//std::cout << t1<<"     "<<t2<<std::endl;
					std::cout << iPoint->Position << std::endl;
					//std::cout << alpha <<"   "<<beta<< std::endl;
					//std::cout << i << std::endl;
					//std::cout << Neighbours[i]->Position << std::endl;
					//std::cout << Neighbours[prev]->Position << std::endl;
					//std::cout << Neighbours[next]->Position << std::endl;
					std::cout << iPoint->ID << std::endl;
				}


				assert(!isnan(w));
				//assert(w < 300);
				//if (w > 100)
				//	w = 100;
				//if (w < -100)
				//	w = -100;
				iPoint->w.push_back(w);
			}
			Vector_3 force(0, 0, 0);
			for (int i = 0; i < iPoint->Neighbours.size(); i++){
				force = force + a*iPoint->w[i] * (iPoint->Position - iPoint->Neighbours[i]->Position);
				if (force.x() < -100){
					std::cout << iPoint->w[i] << std::endl;
					std::cout << iPoint->Position - iPoint->Neighbours[i]->Position;
				}
				assert(force.x() > -100);
			}
			iPoint->Force = force;

		}
	}
	for (auto f = Mesh.facets_begin(); f != Mesh.facets_end(); f++){//每个面
		if (f->PointsOnFacet.empty()) continue;
		auto origPointsOnfacet = f->PointsOnFacet;
		for (auto iPoint : origPointsOnfacet){//每个面上的点
			assert(iPoint->Position.x()>-100);
			Point_3 origPosition = iPoint->Position;
			auto Force = iPoint->Force;
			auto DeltaX = DeltaT*Force;
			assert(iPoint->Position.x()>-100);
			iPoint->DeltaX = DeltaX;
			auto Final = iPoint->Position + DeltaX;
			assert(Final.x()>-100);
			assert(!isnan(Final.x()));
			bool End = false;
			Point_3 ProjPoint;
			assert(Final.x()>-100);
			if (IsIn3D(Final, f, ProjPoint)){
				iPoint->Position = ProjPoint;
				End = true;
			}
			Mesh_t::Facet_handle RefFace;
			Mesh_t::Vertex_handle RefVertex;
			bool Find2D = false;
			bool Find3D = false;
			if (!End){
				auto h = f->facet_begin();
				do{
					assert(!h->is_border_edge());
					auto v = h->vertex();
					auto hh = v->vertex_begin();
					do{
						assert(!hh->is_border_edge());
						auto ff = hh->facet();
						if (IsIn2D(Final, f, ff, ProjPoint)){
							RefFace = ff;
							RefVertex = v;
							Find2D = true;
							break;
						}
					} while (++hh != v->vertex_begin());
					if (Find2D) break;
				} while (++h != f->facet_begin());
				if (Find2D){
					Find3D = false;
					assert(!isnan(Final.x()));
					if (IsIn3D(Final, RefFace, ProjPoint)){
						Find3D = true;
						End = true;
						iPoint->Position = ProjPoint;
						auto newface = RefFace;
						auto origface = iPoint->Facet;
						origface->PointsOnFacet.erase(std::find(origface->PointsOnFacet.begin(), origface->PointsOnFacet.end(), iPoint));
						iPoint->Facet = newface;
						iPoint->Normal = newface->Normal;
						newface->PointsOnFacet.push_back(iPoint);
					}
					else{
						h = RefFace->facet_begin();
						do{
							assert(!h->is_border_edge());
							if (!((h->vertex() == RefVertex) || (h->opposite()->vertex() == RefVertex))) continue;
							auto ff = h->opposite()->facet();
							if (IsIn3D(Final, ff, ProjPoint)){
								Find3D = true;
								End = true;
								iPoint->Position = ProjPoint;
								auto newface = ff;
								auto origface = iPoint->Facet;
								origface->PointsOnFacet.erase(std::find(origface->PointsOnFacet.begin(), origface->PointsOnFacet.end(), iPoint));
								iPoint->Facet = newface;
								iPoint->Normal = newface->Normal;
								newface->PointsOnFacet.push_back(iPoint);
							}
						} while (++h != RefFace->facet_begin());
					}
				}
			}
			if (!End){
				int ProjDirec = f->ProjDirec;
				Point_3 ProjFinal = f->Triangle.supporting_plane().projection(Final);
				Point_2 Final2 = Project2(ProjFinal, ProjDirec);
				Triangle_2 Triangle2 = f->Triangle2[ProjDirec];
				bool isleft[3];
				for (int i = 0; i < 3; i++){
					isleft[i] = !(CGAL::determinant(Vector_2(Triangle2[i], Final2), Vector_2(Triangle2[i], Triangle2[(i + 1) % 3])) >= 0);
				}
				int linenum = -1;
				if (isleft[0] && !isleft[1] && isleft[2]){
					linenum = 1;
				}
				if (isleft[0] && isleft[1] && !isleft[2]){
					linenum = 2;
				}
				if (!isleft[0] && isleft[1] && isleft[2]){
					linenum = 0;
				}
				if (linenum != -1){
					Line_2 line(Triangle2[linenum], Triangle2[(linenum + 1) % 3]);
					Point_2 foot = line.projection(Final2);
					double length1 = sqrt(CGAL::squared_distance(foot, Triangle2[linenum]));
					double length2 = sqrt(CGAL::squared_distance(foot, Triangle2[(linenum + 1) % 3]));
					double length0 = sqrt(CGAL::squared_distance(Triangle2[(linenum + 1) % 3], Triangle2[linenum]));
					Final2 = foot;
					if (std::abs(length2 - length1 - length0) < EPSILON) Final2 = Triangle2[linenum];
					if (std::abs(length1 - length2 - length0) < EPSILON) Final2 = Triangle2[(linenum + 1) % 3];
					End = true;
				}
				if (!End){
					int vnum = -1;
					if (!isleft[0] && !isleft[1] && isleft[2]){
						vnum = 1;
					}
					if (!isleft[0] && isleft[1] && !isleft[2]){
						vnum = 0;
					}
					if (isleft[0] && !isleft[1] && !isleft[2]){
						vnum = 2;
					}
					if (vnum != -1){
						Final2 = Triangle2[vnum];
						End = true;
					}
				}
				if (!End)
					std::cout << 1;
				assert(End);
				iPoint->Position = CalcPosition(f->Triangle, Final2.x(), Final2.y(), ProjDirec);
				assert(!isnan(iPoint->Position.x()));
			}
			res = res + sqrt((iPoint->Position - origPosition).squared_length());
		}
	}
	res = res / DeltaT / pPoints.size();
	return res;
}