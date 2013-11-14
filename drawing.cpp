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

GLfloat shininess = 0;

/* The parameter list may need to be changed for the functions in this file */

void glNormal(vector *vec){
    std::vector<vector*> v;
    vector *normal;

    //if the vector is at the top
    if(vec->second_index == 0 ){
        normal = new vector();
        if(vec->first_index == 0){
            normal->crossProduct(*vec, *obj[obj.size()-1][vec->second_index]);
            v.push_back(normal);
        }else{
            normal->crossProduct(*vec, *obj[vec->first_index-1][vec->second_index]);
            v.push_back(normal);
        }
        normal = new vector();
        if(vec->first_index == obj.size()-1){
            normal->crossProduct(*vec, *obj[0][vec->second_index]);
            v.push_back(normal);
        }else{
            normal->crossProduct(*vec, *obj[vec->first_index+1][vec->second_index]);
            v.push_back(normal);
        }
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index][vec->second_index+1]);
        v.push_back(normal);
        //average normals
        vector *averaged_normal = new vector((v[0]->x + v[1]->x + v[2]->x)/3, (v[0]->y + v[1]->y + v[2]->y)/3, (v[0]->z + v[1]->z + v[2]->z)/3);
        // normal->x = (v[0]->x + v[1]->x + v[2]->x)/3;
        // normal->y = (v[0]->y + v[1]->y + v[2]->y)/3;
        // normal->z = (v[0]->z + v[1]->z + v[2]->z)/3;
        printf("!!!!!!!!!!!!!!!!!!!\n");
    //if the vector is at the bottom
    }else if (vec->second_index == obj.size()-1){
        normal = new vector();
        if(vec->first_index == 0){
            normal->crossProduct(*vec, *obj[obj.size()-1][vec->second_index]);
            v.push_back(normal);
        }else{
            normal->crossProduct(*vec, *obj[vec->first_index-1][vec->second_index]);
            v.push_back(normal);
        }
        normal = new vector();
        if(vec->first_index == obj.size()-1){
            normal->crossProduct(*vec, *obj[0][vec->second_index]);
            v.push_back(normal);
        }else{
            normal->crossProduct(*vec, *obj[vec->first_index+1][vec->second_index]);
            v.push_back(normal);
        }
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index][vec->second_index+1]);
        v.push_back(normal);

        //average normals
        normal = new vector();
        normal->x = (v[0]->x + v[1]->x + v[2]->x)/3;
        normal->y = (v[0]->y + v[1]->y + v[2]->y)/3;
        normal->z = (v[0]->z + v[1]->z + v[2]->z)/3;
    //else the vector is in the middle
    }else{
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index-1][vec->second_index]);
        v.push_back(normal);
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index+1][vec->second_index]);
        v.push_back(normal);
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index][vec->second_index-1]);
        v.push_back(normal);
        normal = new vector();
        normal->crossProduct(*vec, *obj[vec->first_index][vec->second_index+1]);
        v.push_back(normal);

        //average normals
        normal = new vector();
        normal->x = (v[0]->x + v[1]->x + v[2]->x + v[3]->x)/4;
        normal->y = (v[0]->y + v[1]->y + v[2]->y + v[3]->y)/4;
        normal->z = (v[0]->z + v[1]->z + v[2]->z + v[3]->z)/4;
    }

    glNormal3f(normal->x, normal->y, normal->z); 
}

void glVertex(vector *vec){
    glVertex3f(vec->x, vec->y, vec->z);
    glNormal(vec);
}

void drawSurface(void) {
    for(unsigned int j = 0; j < obj.size() - 1; j++){
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_POLYGON);
            glColor3f( 0.0f, 0.0f, 1.0f );
            glVertex(obj[j][i]);
            glVertex(obj[j][i+1]);
            glVertex(obj[j+1][i]);
            glEnd();
        }
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_POLYGON);
            glColor3f( 0.0f, 0.0f, 1.0f );
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

void generate3D(bool backTo2D){
    printf("Results of rotation: \n");
    if(!backTo2D)
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
  glVertex2f(0.0, 400);
  glVertex2f(0.0, -400);
  glEnd();
}

void setupLighting(void){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    // glColorMaterial(GL_FRONT, GL_DIFFUSE);
    // glColorMaterial(GL_FRONT, GL_SPECULAR);

    // GLfloat ambientMaterial[] = {0.0f, 0.0f, 1.0f, 1.0f};
    // GLfloat diffuseMaterial[] = {0.0f, 0.0f, 1.0f, 1.0f};
    // GLfloat specularMaterial[] = {0.0f, 0.0f, 1.0f, 1.0f};
    
    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    // Create light components.
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 100.0f, 0.0f, 0.0f, 1.0f };

    // Assign created components to GL_LIGHT0.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}
/* end of drawing.cpp */
