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

void drawSurface(void) {

}

void drawPoints(void){
    glPointSize( 6.0 );
    glBegin( GL_POINTS );
    glColor3f( 0.95f, 0.207, 0.031f );
    for(int i = 0; i < num_i0_pts; i++){
        glVertex2f( i0_x[i], i0_y[i] );
    }
    glEnd();
}

void drawLines(void){
    glLineWidth(2.5);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    for(int i = 1; i < num_i0_pts; i++){
        glVertex3f(i0[i-1].x, i0[i-1].y, i0[i].z);
        glVertex3f(i0[i].x, i0[i].y, i0[i].z);
    }
    glEnd();
}

void generate3D(void){
    for(int i = 0; i < num_i0_pts; i++){
        i0[i].rotateVector(120.0, i1[i]);
    }
	for(int i = 0; i < num_i0_pts; i++){
        i0[i].rotateVector(-120.0, i2[i]);
    }
}

/* end of drawing.cpp */
