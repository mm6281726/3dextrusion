/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
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
#include "drawing.h"
#include "data.h"
#include "vector.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;

bool _2dmode = 1;

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-W/2, W/2, -H/2, H/2, -zNear, -zFar);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up drawing screen
	glLineWidth(2.5);
  glBegin(GL_LINES);
  glColor3f(1.0, 1.0, 1.0);
  glVertex2f(0.0, 200);
  glVertex2f(0.0, -200);
  glEnd();

	/*
	 * See drawing.c for the definition of these routines.
	 *
	 * Note: Only one should be called at a time (based on the
	 * display mode).
	 */
    if(_2dmode)
      drawPoints();
    else
      drawSurface();
    if(num_i0_pts > 1)
        drawLines();

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'q':
			endSubdiv(0);
			break;

    case 'w':
      if(num_i0_pts > 4){
		    _2dmode = false;
        generate3D();
        printf("Rotating...\n");
      }
      else
        printf("Warning: Not enough control points to perform rotation\n");
      break;

		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();

	return;
}

void myMouseButton(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
      if(num_i0_pts < 30){
        int new_x = x - 200;
				if(new_x < 2)
					new_x = 0;
        int new_y = 200 - y;
        if(new_x > 0){
		      i0_x[num_i0_pts] = new_x;
          i0_y[num_i0_pts] = new_y;
          i0[num_i0_pts].x = new_x;
          i0[num_i0_pts].y = new_y;
          i0[num_i0_pts].z = 0.0;
          i0[num_i0_pts].w = 0.0;
          num_i0_pts++;
          printf("x: %3d, y: %3d, i: %3d\n", x, y, num_i0_pts);
          display();
        }
      }else
				printf("Warning: Number of points has reached the maximum\n");
		}
  if (button == GLUT_RIGHT_BUTTON) {
    if(num_i0_pts > 0){
      num_i0_pts--;
			printf("i: %3d\n", num_i0_pts);
      display();
    }else
			printf("Warning: No points to delete\n");
    }
	}
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}


/* end of subdiv.cpp */
