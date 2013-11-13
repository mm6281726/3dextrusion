/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: Michael Madden
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "drawing.h"
#include "data.h"
#include "vector.h"

/* Globals */
int subdiv_v; // The user-specified subdivision level, vertical
int subdiv_h; // The user-specified subdivision level, horizontal

/* The parameter list may need to be changed for the functions in this file */

void glVertex(vector *vec){
    glVertex3f(vec->x, vec->y, vec->z);
}

void drawSurface(void) {
    for(unsigned int j = 0; j < obj.size() - 1; j++){
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_POLYGON);
            glColor3f( 0.0f, 0.0f, 1.0f - (j+1.0)/num_draw_pts);
            glVertex(obj[j][i]);
            glVertex(obj[j][i+1]);
            glVertex(obj[j+1][i]);
            glEnd();
        }
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_POLYGON);
            glColor3f( 0.0f, 0.0f, 1.0f - (j+1.0)/num_draw_pts);
            glVertex(obj[j+1][i]);
            glVertex(obj[j+1][i+1]);
            glVertex(obj[j][i+1]);
            glEnd();
        }
    }
    for(int i = 0; i < num_draw_pts-1; i++){
        glBegin(GL_POLYGON);
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex(obj[obj.size()-1][i]);
        glVertex(obj[obj.size()-1][i+1]);
        glVertex(obj[0][i]);
        glEnd();
    }
    for(int i = 0; i < num_draw_pts-1; i++){
        glBegin(GL_POLYGON);
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex(obj[0][i]);
        glVertex(obj[0][i+1]);
        glVertex(obj[obj.size()-1][i+1]);
        glEnd();
    }
}

void draw2DPoints(void){
    glPointSize( 6.0 );
    glBegin( GL_POINTS );
    glColor3f( 0.95f, 0.207, 0.031f );
    for(int i = 0; i < num_i0_pts; i++){
        glVertex2f( i0_x[i], i0_y[i] );
    }
    glEnd();
}

void draw3DPoints(void){
    glPointSize( 6.0 );
    glBegin( GL_POINTS );
    glColor3f( 0.95f, 0.207, 0.031f );
    for(unsigned int j = 0; j < obj.size(); j++){
        for(int i = 0; i < num_draw_pts; i++){
            glVertex3f( obj[j][i]->x, obj[j][i]->y, obj[j][i]->z );
        }
    }
    glEnd();
}

void draw2DLines(void){
    glLineWidth(2.5);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    for(int i = 1; i < num_i0_pts; i++){
        glVertex3f(i0_x[i-1], i0_y[i-1], 0.0);
        glVertex3f(i0_x[i], i0_y[i], 0.0);
    }
    glEnd();
}

void draw3DLines(void){
    glLineWidth(2.5);
    for(unsigned int j = 0; j < obj.size() - 1; j++){
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_LINES);
            glColor3f( 1.0f, 0.0f, 0.0f);
            glVertex(obj[j][i]);
            glVertex(obj[j][i+1]);
            glEnd();
        }
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_LINES);
            glColor3f( 0.0f, 1.0f, .0f);
            glVertex(obj[j][i]);
            glVertex(obj[j+1][i]);
            glEnd();
        }
    }
    for(int i = 0; i < num_draw_pts-1; i++){
        glBegin(GL_LINES);
        glColor3f( 1.0f, 0.0f, 1.0f );
        glVertex(obj[obj.size()-1][i]);
        glVertex(obj[obj.size()-1][i+1]);
        glEnd();
    }
    for(int i = 0; i < num_draw_pts-1; i++){
        glBegin(GL_LINES);
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex(obj[0][i]);
        glVertex(obj[obj.size()-1][i]);
        glEnd();
    }
}

void generate3D(void){
    printf("Results of rotation: \n");
    obj.resize(3);
    for(int i = 0; i < num_i0_pts; i++){
        vector *vec = new vector(i0_x[i], i0_y[i], 0.0);
        obj[0].push_back(vec);
        printf("obj[%i][%i]: x: %f, y: %f, z: %f\n", 0, i, obj[0][i]->x, obj[0][i]->y, obj[0][i]->z);
    }
    for(int i = 0; i < num_i0_pts; i++){
        vector *vec = new vector();
        obj[0][i]->rotateVector(120.0, *vec);
        obj[1].push_back(vec);
        printf("obj[%i][%i]: x: %f, y: %f, z: %f\n", 1, i, obj[1][i]->x, obj[1][i]->y, obj[1][i]->z);
    }
	for(int i = 0; i < num_i0_pts; i++){
        vector *vec = new vector();
        obj[0][i]->rotateVector(-120.0, *vec);
        obj[2].push_back(vec);
        printf("obj[%i][%i]: x: %f, y: %f, z: %f\n", 2, i, obj[2][i]->x, obj[2][i]->y, obj[2][i]->z);
    }
}

void setupAxis(){
  // Set up drawing screen
  glLineWidth(2.5);
  glBegin(GL_LINES);
  glColor3f(1.0, 1.0, 1.0);
  glVertex2f(0.0, 200);
  glVertex2f(0.0, -200);
  glEnd();
}

/* end of drawing.cpp */
