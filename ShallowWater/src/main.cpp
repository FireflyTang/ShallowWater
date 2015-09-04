#include "Macro.h"
#include "Mesh.hpp"
#include "MeshSimplification.hpp"
#include "AABBTree.hpp"
#include "Sample.hpp"
#include "CoolCG.h"
#include "CoolCGDelegate.h"


void Draw();
Mesh_t P;
std::vector<pPoint_t> pPoints;

double deltaT = 0.1;
double a = 0.01;
int main(int argc, char ** argv) {

	LoadMesh(P, "./../src/bunny1.obj");
	//Simplification(P, 0.1);
	AABBTree_t tree;
	std::vector<AABBFacet_t> facets;
	auto aaa = LoadAABBTree(facets, "./../src/bunny1.obj");
	tree.insert(facets.begin(), facets.end());
	tree.build();
	BuildMesh(P, tree);
	Sample(pPoints, P);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	classControl.InitWindowSize(500, 500);

	glutInitWindowPosition(500, 0);
	glutCreateWindow("Scene");

	classControl.setPos(-0.0385766, 0.0454023, 0.0431139);
	classControl.setUp(0, 0, 1);
	classControl.setCenter(-0.0385766, 0.0354023, 0.0431139);
	classControl.setMoveStepCoefficient(0.1);
	classControl.setRotationStepCoefficient(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutSpecialFunc(DelagatekeyRotateByCenter);
	glutKeyboardFunc(DelegatekeyMove);
	glutDisplayFunc(Draw);
	glutMouseFunc(DelegateMousePress);
	glutMotionFunc(DelegateMouseMoveByCenter);

	glutMainLoop();

	return 0;
}

std::vector<Triangle_3> TrianglesToDraw;
std::vector<Segment_3> SegmentsToDraw;
std::vector<Segment_3> ForceToDraw;
int step = 0;
void Draw(){
	int c = 0;
	double res = DBL_MAX;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 0.00001, 10);
	//------------------------------------
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	classControl.setLookAt();

	SegmentsToDraw.clear();
	TrianglesToDraw.clear();
	ForceToDraw.clear();
	if ((step<5000) && (res>0.0001)){
		TrianglesToDraw.clear();
		SegmentsToDraw.clear();
		ForceToDraw.clear();
		res = Move(pPoints, P, a, deltaT);
		std::cout << step++ << "  " << res << std::endl;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	glBegin(GL_LINES);
	{
		glColor3d(0, 0, 1);
		for (auto p : pPoints){
			//auto p = pPoints[2430];
			ForceToDraw.push_back(Segment_3(p->Position, p->Position + 10 * deltaT*(p->Force)));
			//SegmentsToDraw.push_back(Segment_3(p->Position, p->Position + 0.3*p->Normal));
			for (int i = 0; i < p->Neighbours.size(); i++){
				Vector_3 f;
				f = 10 * deltaT * (a*p->w[i] * (p->Position - p->Neighbours[i]->Position));
				Point_3 source = p->Position;
				Point_3 target = p->Position + f;
				glVertex3d(source[0], source[1], source[2]);
				glVertex3d(target[0], target[1], target[2]);
			}
		}
	}
	glEnd();

	for (auto p : pPoints){
		for (auto pp : p->Neighbours){
			SegmentsToDraw.push_back(Segment_3(p->Position, pp->Position));
		}
		Vector_3 x = p->DeltaX;
		x = x - x*p->Facet->Normal*p->Facet->Normal;
		//ForceToDraw.push_back(Segment_3(p->Position, p->Position + x));
		//assert(p->Neighbours.size() == 6);
	}

	//auto p = pPoints[2430];
	////Vector_3 x = p->DeltaX;
	////x = x - x*p->Facet->Normal*p->Facet->Normal;
	//Vector_3 f = p->Force;
	//ForceToDraw.push_back(Segment_3(p->Position, p->Position +100*f));
	//for (auto pp : p->Neighbours){
	//	/*Vector_3 x = pp->DeltaX;
	//	x = x - x*p->Facet->Normal*p->Facet->Normal;*/
	//	Vector_3 f = pp->Force;
	//	ForceToDraw.push_back(Segment_3(pp->Position, pp->Position + 100*f));
	//}

	glColor3d(0, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!SegmentsToDraw.empty()){
		glBegin(GL_LINES);
		for (auto & s : SegmentsToDraw){
			glVertex3d(s.source().x(), s.source().y(), s.source().z());
			glVertex3d(s.target().x(), s.target().y(), s.target().z());
		}
		glEnd();
	}
	glColor3d(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!TrianglesToDraw.empty()){
		glBegin(GL_TRIANGLES);
		for (auto & t : TrianglesToDraw){
			glVertex3d(t[0].x(), t[0].y(), t[0].z());
			glVertex3d(t[1].x(), t[1].y(), t[1].z());
			glVertex3d(t[2].x(), t[2].y(), t[2].z());
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3d(1, 0, 0);
	if (!ForceToDraw.empty()){
		glBegin(GL_LINES);
		for (auto & s : ForceToDraw){
			glVertex3d(s.source().x(), s.source().y(), s.source().z());
			glVertex3d(s.target().x(), s.target().y(), s.target().z());
		}
		glEnd();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}
