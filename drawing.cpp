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
int stuff = 0;
GLfloat shininess = 10;

vector* calculateNormal(vector* v1, vector* v2, vector* v3){
    vector* normal = new vector();
    vector* u1 = new vector(v2->x - v1->x, v2->y - v1->y, v2->z - v1->z);
    vector* u2 = new vector(v3->x - v1->x, v3->y - v1->y, v3->z - v1->z);
    normal->crossProduct(*u1, *u2);
    return normal;
}

void glNormal(vector* vec){
    GLfloat avg_x, avg_y, avg_z;
    int vec_first_index = (int)vec->first_index;
    int vec_second_index = (int)vec->second_index;

    int left_first_index = (int)vec->first_index-1;
    int left_second_index = (int)vec->second_index;

    int top_first_index = (int)vec->first_index;
    int top_second_index = (int)vec->second_index-1;

    int right_first_index = (int)vec->first_index+1;
    int right_second_index = (int)vec->second_index;

    int bottom_first_index = (int)vec->first_index;
    int bottom_second_index = (int)vec->second_index+1;

    //loop to end
    if(vec_first_index == 0){
        left_first_index = obj.size()-1;
    //loop to beginning
    }else if(vec_first_index == (int)obj.size()-1){
        right_first_index = 0;
    }

    //if at the top
    if(vec_second_index == 0){
        vector* normaltop = calculateNormal(vec, obj[right_first_index][right_second_index], obj[left_first_index][left_second_index]);
        vector* normalbottomright = calculateNormal(vec, obj[bottom_first_index][bottom_second_index], obj[right_first_index][right_second_index]);
        vector* normalbottomleft = calculateNormal(vec, obj[left_first_index][left_second_index], obj[bottom_first_index][bottom_second_index]);

        avg_x = (normaltop->x + normalbottomright->x + normalbottomleft->x)/3.0;
        avg_y = (normaltop->y + normalbottomright->y + normalbottomleft->y)/3.0;
        avg_z = (normaltop->z + normalbottomright->z + normalbottomleft->z)/3.0;

    //if at the bottom
    }else if(vec_second_index == (int)obj[0].size()-1){
        vector* normaltopleft = calculateNormal(vec, obj[top_first_index][top_second_index], obj[left_first_index][left_second_index]);
        vector* normaltopright = calculateNormal(vec, obj[right_first_index][right_second_index],  obj[top_first_index][top_second_index]);
        vector* normalbottom = calculateNormal(vec, obj[right_first_index][right_second_index], obj[left_first_index][left_second_index]);

        avg_x = (normaltopleft->x + normaltopright->x + normalbottom->x)/3.0;
        avg_y = (normaltopleft->y + normaltopright->y + normalbottom->y)/3.0;
        avg_z = (normaltopleft->z + normaltopright->z + normalbottom->z)/3.0;

    //if at the middle
    }else{
        vector* normaltopleft = calculateNormal(vec, obj[top_first_index][top_second_index], obj[left_first_index][left_second_index]);
        vector* normaltopright = calculateNormal(vec, obj[right_first_index][right_second_index],  obj[top_first_index][top_second_index]);
        vector* normalbottomright = calculateNormal(vec, obj[bottom_first_index][bottom_second_index], obj[right_first_index][right_second_index]);
        vector* normalbottomleft = calculateNormal(vec, obj[left_first_index][left_second_index], obj[bottom_first_index][bottom_second_index]);

        avg_x = (normaltopleft->x + normaltopright->x + normalbottomright->x + normalbottomleft->x)/4.0;
        avg_y = (normaltopleft->y + normaltopright->y + normalbottomright->y + normalbottomleft->y)/4.0;
        avg_z = (normaltopleft->z + normaltopright->z + normalbottomright->z + normalbottomleft->z)/4.0;
    }
    vector* averaged_normal = new vector(avg_x, avg_y, avg_z);
    glNormal3f(averaged_normal->x, averaged_normal->y, averaged_normal->z);
}

void defineMaterial(){
    GLfloat ambientMaterial[] = {0.3f, 0.3f, 0.8f, 1.0f};
    GLfloat diffuseMaterial[] = {0.3f, 0.3f, 0.8f, 1.0f};
    GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

}

void glVertex(vector *vec){
    defineMaterial();
    glNormal(vec);
    glVertex3f(vec->x, vec->y, vec->z);
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
    glColor3f( 0.95f, 0.207, 0.731f );
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
    glLineWidth(1.5);
    for(unsigned int j = 0; j < obj.size() - 1; j++){
        for(int i = 0; i < num_draw_pts-1; i++){
            glBegin(GL_LINES);
            glColor3f( 1.0f, 0.0f, 0.0f);
            glVertex(obj[j][i]);
            glVertex(obj[j][i+1]);
            glEnd();
        }
        for(int i = 0; i < num_draw_pts; i++){
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
    for(int i = 0; i < num_draw_pts; i++){
        glBegin(GL_LINES);
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex(obj[0][i]);
        glVertex(obj[obj.size()-1][i]);
        glEnd();
    }
}

void generate3D(){
    printf("Results of rotation: \n");
    obj.clear();
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
    generateIndices();
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

    // Create light components.
    GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat position[] = { 200.0f, 200.0f, 10.0f, 1.0f };

    // Assign created components to GL_LIGHT0.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}
/* end of drawing.cpp */
