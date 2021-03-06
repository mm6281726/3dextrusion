/*
 * data.cpp
 * --------
 * Contains routines to help manage the data for the Midpoint Subdivision.
 * Starter code for Project 3.
 *
 * Group Members: Michael Madden
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <assert.h> 

#include "common.h"
#include "data.h"

GLfloat i0_x[MAX_POINT];    // Iteration 0 control points, x
GLfloat i0_y[MAX_POINT];    // Iteration 0 control points, y

std::vector<std::vector<vector*> > obj;
std::vector<std::vector<vector*> > phong_obj;

int num_i0_pts;             // The number of iteration 0 control points

GLfloat *draw_x;     // Control points to be drawn, x
GLfloat *draw_y;     // Control points to be drawn, y
int num_draw_pts;    // The number of control points to draw


/**********************************************
 * Begin Routines for Midpoint Subdivision
 **********************************************/

/*
 * Copies a static array into dynamically allocated space and
 * returns the copy.  Useful for making a copy of the static
 * iteration 0 control points, since the iteration 0 control
 * points should be kept in their static array for user-interface
 * purposes.
 *
 * Note: this function needs to be called twice, once for x and
 * once for y.
 */
GLfloat *copyPointsArray(GLfloat *orig, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(orig != NULL);
	}

	GLfloat *copy;
	copy = (GLfloat *)malloc(size * sizeof(GLfloat));
	for (int i = 0; i < size; i++) {
		copy[i] = orig[i];
	}

	return copy;
}

/*
 * Allocates an 1-D GLfloat array for the given number of elements.
 */
GLfloat *makePointsArray(int size) {
	assert(size >= 0);

	return (GLfloat *)malloc(size * sizeof(GLfloat));
}

/*
 * Given 2 equally sized points arrays, arr1 and arr2 in this form:
 *
 * arr1:    a0, a1, a2, a3, ...
 * arr2:    b0, b1, b2, b3, ... 
 *
 * ...returns a newly allocated array arr3 which contains:
 * arr3:    a0, b0, a1, b1, a2, b2, a3, b3, ...
 *
 * Note:
 * This function frees up the storage for arr1 and arr2, so the caller
 * of this function should not reference these arrays again.
 *
 * The two arrays should be the same size, and the argument "size"
 * is the size of one of these two arrays.
 */

GLfloat *mergePointsArrays(GLfloat *arr1, GLfloat *arr2, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(arr1 != NULL);
		assert(arr2 != NULL);
	}

	GLfloat *arr3;

	arr3 = (GLfloat *)malloc(2 * size * sizeof(GLfloat));

	for (int i = 0; i < size; i++) {
		arr3[2 * i] = arr1[i];
		arr3[(2 * i) + 1] = arr2[i];
	}

	free(arr1);
	free(arr2);

	return arr3;
}

/*
 * Using the iteration 0 control points (which are stored in the global
 * arrays "i0_x" and "i0_y") and the argument subdivision level, sets
 * the globals "GLfloat *draw_x" and "GLfloat *draw_y" to contain the x and
 * y coordinates for all points that need to be drawn at the given
 * subdivision level.  Also sets the number of value of global
 * "num_draw_pts" to be the size of one of these two drawing arrays.
 *
 * Note: The subdivision level should be greater than or equal to 0.  This
 * routine also frees the space used by the old "draw_x" and "draw_y" arrays,
 * if they exist.
 */
void subdividePointsArray(int subdiv_level) {
	assert(subdiv_level >= 0);

	/* ADD YOUR CODE HERE */

	return;
}

void printPoints(std::vector<std::vector<vector*> > vecobj){
	printf("Current points:\n");
	for(unsigned int j = 0; j < vecobj.size(); j++)
		for(unsigned int i = 0; i < vecobj[0].size(); i++){
			std::cout << "vecobj[" << j << "][" << i << "]: x: " << vecobj[j][i]->x << ", y: " << vecobj[j][i]->y << ", z: " << vecobj[j][i]->z << std::endl;
		}
}

void printPointsDebug(std::vector<std::vector<vector*> > vecobj){
	for(unsigned int j = 0; j < vecobj.size(); j++)
		for(unsigned int i = 0; i < vecobj[0].size(); i++){
			printf("POINT #%i %i\n", j, i);
			std::cout << "vecobj["; 
			std::cout << j;
			std::cout << "][";
			std::cout << i;
			std::cout << "]: "<<std::endl;
			std::cout << "x: " << vecobj[j][i]->x << ", y: " << vecobj[j][i]->y << ", z: " << vecobj[j][i]->z << std::endl;
		}
}

//1.0/8.0(pi-1 + 6.0(pi) + pi+1)
void applyEvenRule(vector &new_vec, vector &left_vec, vector &mid_vec, vector &right_vec){
	new_vec.x = (1.0/8.0)*(left_vec.x + 6.0*(mid_vec.x) + right_vec.x);
	new_vec.y = (1.0/8.0)*(left_vec.y + 6.0*(mid_vec.y) + right_vec.y);
	new_vec.z = (1.0/8.0)*(left_vec.z + 6.0*(mid_vec.z) + right_vec.z);
}

//1.0/8.0(4.0(pi) + 4.0(pi+1))
void applyOddRule(vector &new_vec, vector &left_vec, vector &right_vec){
	new_vec.x = (1.0/8.0)*(4.0*left_vec.x + 4.0*right_vec.x);
	new_vec.y = (1.0/8.0)*(4.0*left_vec.y + 4.0*right_vec.y);
	new_vec.z = (1.0/8.0)*(4.0*left_vec.z + 4.0*right_vec.z);
}

void applyVerticalSubdivision(std::vector<std::vector<vector*> > &vecobj){
	std::vector<std::vector<vector*> > new_vecobj = vecobj;
	std::vector<vector*> stack (num_draw_pts);
	vecobj.clear();
	for(unsigned int j = 0; j < new_vecobj.size(); j++){
		stack.clear();
		stack.push_back(new_vecobj[j][0]);
		for(int i = 1; i < num_draw_pts; i++){
			//Odd Rule
			vector *vec1 = new vector();
			applyOddRule(*vec1, *new_vecobj[j][i-1], *new_vecobj[j][i]);
			stack.push_back(vec1);

			//Even Rule
			if(i == num_draw_pts-1) continue;
			vector *vec2 = new vector();
		    applyEvenRule(*vec2, *new_vecobj[j][i-1], *new_vecobj[j][i], *new_vecobj[j][i+1]);
		    stack.push_back(vec2);
		}
		stack.push_back(new_vecobj[j][num_draw_pts-1]);
		vecobj.push_back(stack);
	}
	num_draw_pts = 2*num_draw_pts-1;
	generateIndices(vecobj);
}

void applyHorizontalSubdivision(std::vector<std::vector<vector*> > &vecobj){
	std::vector<std::vector<vector*> > new_vecobj = vecobj;
	vecobj.clear();
	vecobj.resize(new_vecobj.size()*2);
	int new_point_count;
	for(int i = 0; i < num_draw_pts; i++){
		new_point_count = 0;
		for(unsigned int j = 0; j < new_vecobj.size(); j++){
			vector *vec1 = new vector();
			if(j == 0)
				applyEvenRule(*vec1, *new_vecobj[new_vecobj.size()-1][i], *new_vecobj[j][i], *new_vecobj[j+1][i]);
			else if(j == new_vecobj.size()-1)
				applyEvenRule(*vec1, *new_vecobj[j-1][i], *new_vecobj[j][i], *new_vecobj[0][i]);
			else
				applyEvenRule(*vec1, *new_vecobj[j-1][i], *new_vecobj[j][i], *new_vecobj[j+1][i]);
			vecobj[new_point_count].push_back(vec1);
			new_point_count++;

			vector *vec2 = new vector();
			if(j == new_vecobj.size()-1){
				applyOddRule(*vec2, *new_vecobj[j][i], *new_vecobj[0][i]);
			}else{
				applyOddRule(*vec2, *new_vecobj[j][i], *new_vecobj[j+1][i]);
			}
			vecobj[new_point_count].push_back(vec2);
			new_point_count++;
		}
	}
	generateIndices(vecobj);
}

void phongSubdivision(){
	applyVerticalSubdivision(phong_obj);
	applyHorizontalSubdivision(phong_obj);
}

//generate indices for calculating normals of vertices
void generateIndices(std::vector<std::vector<vector*> > &vecobj){
	for(unsigned int i = 0; i < vecobj.size(); i++)
		for(unsigned int j = 0; j < vecobj[0].size(); j++){
			vecobj[i][j]->setIndices(i, j);
		}
}

/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
