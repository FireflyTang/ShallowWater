//
//  Draw.cpp
//  ShallowWater
//
//  Created by 唐如麟 on 15/8/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Draw.hpp"

#include "CoolCG.h"
#include "CoolCGDelegate.h"

std::vector<Segment_3> * pSegmentsToDraw;
std::vector<Triangle_3> * pTrianglesToDraw;

void displayfun(){
    
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, 1000, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 2, 1, 1000);
    //------------------------------------
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    classControl.setLookAt();
    glColor3d(1, 1, 1);
    if(pSegmentsToDraw){
        glBegin(GL_LINES);
        for(auto & s : *pSegmentsToDraw){
            glVertex3d(s.source().x(), s.source().y(), s.source().z());
            glVertex3d(s.target().x(), s.target().y(), s.target().z());
        }
        glEnd();
    }
    if(pTrianglesToDraw){
        glBegin(GL_TRIANGLES);
        for(auto & t : *pTrianglesToDraw){
            glVertex3d(t[0].x(), t[0].y(), t[0].z());
            glVertex3d(t[1].x(), t[1].y(), t[1].z());
            glVertex3d(t[2].x(), t[2].y(), t[2].z());
        }
        glEnd();
    }
    glutSwapBuffers();
}

void InitScene(int argc, char ** argv){
    glutInit(&argc,argv);
};


void DrawSence
(std::vector<Triangle_3> * _pTrianglesToDraw,std::vector<Segment_3> * _pSegmentsToDraw){
    pTrianglesToDraw=_pTrianglesToDraw;
    pSegmentsToDraw=_pSegmentsToDraw;
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    classControl.InitWindowSize(1000, 500);
    
    glutInitWindowPosition(0,0);
    glutCreateWindow("Scene");
    
    classControl.setPos(0,20,0);
    classControl.setUp(-1,0,0);
    classControl.setCenter(0,0,0);
    classControl.setMoveStepCoefficient(0.2);
    classControl.setRotationStepCoefficient(1.5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glutSpecialFunc(DelagatekeyRotateByCenter);
    glutKeyboardFunc(DelegatekeyMove);
    glutDisplayFunc(displayfun);
    glutMouseFunc(DelegateMousePress);
    glutMotionFunc(DelegateMouseMoveByCenter);
    
    glutMainLoop();
}

