#include "Macro.h"

#include "Point.h"


class campdistance_t {
public:
    campdistance_t(const pPoint_t &_center) : center(_center) {
    };

    bool operator()(const pPoint_t &a, const pPoint_t &b) {
	    double aa = (a->Position - center->Position).squared_length();
	    double bb = (b->Position - center->Position).squared_length();
	    return aa < bb;
    };
private:
    pPoint_t center;
};

class campangle_t {
public:
    campangle_t(const pPoint_t &_center, const pPoint_t &_ref) : ref(_ref), center(_center) {
	    rv = ref->Position - center->Position;
	    rv = NORMALIZE(rv);
    };

    bool operator()(const pPoint_t &a, const pPoint_t &b) {
	    auto av = a->Position - center->Position;
	    av = NORMALIZE(av);
	    auto bv = b->Position - center->Position;
	    bv = NORMALIZE(bv);
	    double aangle = av * rv;
	    if (aangle > 1) aangle = 1 - EPSILON;
	    if (aangle < -1) aangle = -1 + EPSILON;
	    aangle = acos(aangle);
	    double bangle = bv * rv;
	    if (bangle > 1) bangle = 1 - EPSILON;
	    if (bangle < -1) bangle = -1 + EPSILON;
	    bangle = acos(bangle);
	    if (CGAL::determinant(rv, av) < 0) aangle = 2 * PI - aangle;
	    if (CGAL::determinant(rv, bv) < 0) bangle = 2 * PI - bangle;
	    return aangle <= bangle;
    }

private:
    pPoint_t center;
    pPoint_t ref;
    Vector_2 rv;
};

void CalcW(Grid_t Grid) {

//查找相邻的点
	for (int i = 0; i < INUM + 2; i++) {
		for (int j = 0; j < JNUM + 2; j++) {
			auto &pPoint = Grid[i][j];
			int imin = (i - 2 < 0 ? 0 : i - 2);
			int imax = (i + 2 > INUM + 1 ? INUM + 1 : 0);
			int jmin = (j - 2 < 0 ? 0 : j - 2);
			int jmax = (j + 2 > JNUM + 1 ? JNUM + 1 : 0);
			for (int ii = imin; ii <= imax; ii++) {
				for (int jj = jmin; jj <= jmax; jj++) {
					pPoint->Neighbours.push_back(Grid[ii][jj]);
				}
			}
			std::unique(pPoint->Neighbours.begin(), pPoint->Neighbours.end());
			assert(pPoint->Neighbours.size() >= 9);
			auto self = std::find(pPoint->Neighbours.begin(), pPoint->Neighbours.end(), pPoint);
			assert(self != pPoint->Neighbours.end());
			pPoint->Neighbours.erase(self);
			campdistance_t campdistance(pPoint);
			std::sort(pPoint->Neighbours.begin(), pPoint->Neighbours.end(), campdistance);
			pPoint->Neighbours.erase(pPoint->Neighbours.begin() + 6, pPoint->Neighbours.end());
		}
	}

//保持对称性 并 按角度排序, 只对内部的点
	for (int i = 1; i < INUM + 1; i++) {
		for (int j = 1; j < JNUM + 1; j++) {
			auto &pPoint = Grid[i][j];
			for (auto neighbour : pPoint->Neighbours) {
				if (neighbour->Neighbours.end() == std::find(neighbour->Neighbours.begin(), neighbour->Neighbours.end(), pPoint)) {
					neighbour->Neighbours.push_back(pPoint);
				}
				campangle_t campangle(pPoint, pPoint->Neighbours[0]);
				std::sort(pPoint->Neighbours.begin() + 1, pPoint->Neighbours.end(), campangle);
			}
		}
	}

///TODO: 检查是否内部有退化情况

//设定边界的w
	for (int j = 0; j < JNUM + 2; j++) {
		auto &pPoint = Grid[0][j];
		int size = pPoint->Neighbours.size();
		for (int k = 0; k < size; k++)
			pPoint->w.push_back(0);
	}
	for (int j = 0; j < JNUM + 2; j++) {
		auto &pPoint = Grid[INUM + 1][j];
		int size = pPoint->Neighbours.size();
		for (int k = 0; k < size; k++)
			pPoint->w.push_back(0);
	}
	for (int i = 0; i < INUM + 2; i++) {
		auto &pPoint = Grid[0][i];
		int size = pPoint->Neighbours.size();
		for (int k = 0; k < size; k++)
			pPoint->w.push_back(0);
	}
	for (int i = 0; i < INUM + 2; i++) {
		auto &pPoint = Grid[INUM + 1][i];
		int size = pPoint->Neighbours.size();
		for (int k = 0; k < size; k++)
			pPoint->w.push_back(0);
	}

//计算内部的w
	for (int i = 1; i < INUM + 1; i++) {
		for (int j = 1; j < JNUM + 1; j++) {
			auto &pPoint = Grid[i][j];
			auto &Neighbours = pPoint->Neighbours;
			for (int k = 0; k < Neighbours.size(); k++) {
				int next = (k + 1) % Neighbours.size();
				int prev = (k - 1 + Neighbours.size()) % Neighbours.size();
				double c;
				assert(Neighbours[k]->Position != pPoint->Position);
				assert(Neighbours[prev]->Position != pPoint->Position);
				assert(Neighbours[next]->Position != pPoint->Position);
				c = NORMALIZE(Neighbours[k]->Position - pPoint->Position) * NORMALIZE(Neighbours[next]->Position - pPoint->Position);
				if (c > 1) c = 1 - EPSILON;
				if (c < -1) c = -1 + EPSILON;
				double phi = acos(c);
				assert(!isnan(phi));
				c = NORMALIZE(Neighbours[k]->Position - Neighbours[next]->Position) * NORMALIZE(pPoint->Position - Neighbours[next]->Position);
				if (c > 1) c = 1 - EPSILON;
				if (c < -1) c = -1 + EPSILON;
				double beta = acos(c);
				assert(!isnan(beta));
				c = NORMALIZE(Neighbours[k]->Position - Neighbours[prev]->Position) * NORMALIZE(pPoint->Position - Neighbours[prev]->Position);
				if (c > 1) c = 1 - EPSILON;
				if (c < -1) c = -1 + EPSILON;
				double alpha = acos(c);
				assert(!isnan(alpha));
				pPoint->phi.push_back(phi);
				pPoint->beta.push_back(beta);
				pPoint->alpha.push_back(alpha);
			}
			for (int k = 0; k < Neighbours.size(); i++) {
				double w;
				double alpha = pPoint->alpha[k];
				double beta = pPoint->beta[k];
				double t1, t2;
				t1 = cos(alpha) / sin(alpha);
				t2 = cos(beta) / sin(beta);
				w = t1 + t2;
				assert(!isnan(w));
				pPoint->w.push_back(w);
			}
		}
	}
///TODO: 检查内部点的w是否为0
}