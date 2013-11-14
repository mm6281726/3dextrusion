#ifndef _DATA_H_
#define _DATA_H_
#include "vector.h"
#include <vector>

/* Globals declared in data.cpp */

// The maximum number of points allowed in the 4 Point Scheme
#define MAX_POINT   30

extern GLfloat i0_x[MAX_POINT];	// Iteration 0 control points, x
extern GLfloat i0_y[MAX_POINT];	// Iteration 0 control points, y

extern std::vector<std::vector<vector*> > obj;

extern int num_i0_pts;			// The number of iteration 0 control points

extern GLfloat *draw_x;		// Control points to be drawn, x
extern GLfloat *draw_y;		// Control points to be drawn, y
extern int num_draw_pts;    // The number of control points to draw


/* Functions implemented in data.cpp */
void subdividePointsArray(int subdiv_level);
void applyVerticalSubdivision();
void applyHorizontalSubdivision();
void printPoints(std::vector<std::vector<vector*> > obj);
void generateIndices();

#endif	/* _DATA_H_ */
