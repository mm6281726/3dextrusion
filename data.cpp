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
#include <string.h>
#include <math.h>
#include <assert.h> 

#include "common.h"
#include "data.h"

GLfloat i0_x[MAX_POINT];    // Iteration 0 control points, x
GLfloat i0_y[MAX_POINT];    // Iteration 0 control points, y

std::vector<vector*> i0;
std::vector<vector*> i1;
std::vector<vector*> i2;

std::vector<std::vector<vector*> > obj;

int num_i0_pts;             // The number of iteration 0 control points

int num_total_pts;

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

//1/8(pi-1 + 6(pi) + pi+1)
void applyEvenRule(vector &left_vec, vector &mid_vec, vector &right_vec){
	mid_vec.x = (1/8)*(left_vec.x + 6*(mid_vec.x) + right_vec.x);
	mid_vec.y = (1/8)*(left_vec.y + 6*(mid_vec.y) + right_vec.y);
	mid_vec.z = (1/8)*(left_vec.z + 6*(mid_vec.z) + right_vec.z);
}

//1/8(4(pi) + 4(pi+1))
void applyOddRule(vector &new_vec, vector &left_vec, vector &right_vec){
	new_vec.x = (1/8)*(4*left_vec.x + 4*right_vec.x);
	new_vec.y = (1/8)*(4*left_vec.y + 4*right_vec.y);
	new_vec.z = (1/8)*(4*left_vec.z + 4*right_vec.z);
}

void applyVerticalSubdivision(int level){
	num_total_pts = 2*num_total_pts+1;
	for(uint j = 0; j < obj.size()-1; j++){
		for(int i = 1; i < num_total_pts-2; i++){
			if(i%2==0) applyEvenRule(*obj[j][i-1], *obj[j][i], *obj[j][i+1]);
			else{
				vector *vec = new vector();
				applyOddRule(*vec, *obj[j][i], *obj[j][i+1]);
				obj[j].insert(i, vec);
			}
		}
	}
}

void applyHorizontalSubdivision(int level){

}

/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
