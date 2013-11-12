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

void printPoints(std::vector<std::vector<vector*> > obj){
	for(unsigned int j = 0; j < obj.size(); j++)
		for(unsigned int i = 0; i < obj[0].size(); i++)
			std::cout << "obj[" << j << "][" << i << "]: x: " << obj[j][i]->x << ", y: " << obj[j][i]->y << ", z: " << obj[j][i]->z << std::endl;
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

void applyVerticalSubdivision(){
	std::vector<std::vector<vector*> > new_obj = obj;
	std::vector<vector*> stack (num_draw_pts);
	obj.clear();
	for(unsigned int j = 0; j < new_obj.size(); j++){
		stack.clear();
		stack.push_back(new_obj[j][0]);
		for(int i = 1; i < num_draw_pts; i++){
			//Odd Rule
			vector *vec1 = new vector();
			applyOddRule(*vec1, *new_obj[j][i-1], *new_obj[j][i]);
			stack.push_back(vec1);

			//Even Rule
			if(i == num_draw_pts-1) continue;
			vector *vec2 = new vector();
		    applyEvenRule(*vec2, *new_obj[j][i-1], *new_obj[j][i], *new_obj[j][i+1]);
		    stack.push_back(vec2);
		}
		stack.push_back(new_obj[j][num_draw_pts-1]);
		obj.push_back(stack);
	}
	printPoints(obj);
	num_draw_pts = 2*num_draw_pts-1;
}

void applyHorizontalSubdivision(){
	std::vector<std::vector<vector*> > new_obj = obj;
	obj.clear();
	obj.resize(obj.size()*2);
	printPoints(obj);
	for(int i = 0; i < num_draw_pts; i++){
		for(unsigned int j = 0; j < new_obj.size(); j++){
			vector *vec1 = new vector();
			if(j == 0)
				applyEvenRule(*vec1, *new_obj[new_obj.size()-1][i], *new_obj[j][i], *new_obj[j+1][i]);
			else 
				applyEvenRule(*vec1, *new_obj[j-1][i], *new_obj[j][i], *new_obj[j+1][i]);
			obj[j].push_back(vec1);

			vector *vec2 = new vector();
			if(j == new_obj.size()-1)
				applyOddRule(*vec2, *new_obj[j][i], *new_obj[0][i]);
			else
				applyOddRule(*vec2, *new_obj[j][i], *new_obj[j+1][i]);
			obj[j+1].push_back(vec2);
		}
	}
	printPoints(obj);
	num_draw_pts = 2*num_draw_pts-1;
}

/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
