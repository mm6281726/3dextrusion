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
GLfloat zNear   =  120.0;
GLfloat zFar    = -120.0;

/* Constants for specifying the 3 coordinate axes */
#define X_AXIS      0
#define Y_AXIS      1
#define Z_AXIS      2

/* The three different operations the mouse can control for the canvas */
#define MOUSE_ROTATE_YX   0
#define MOUSE_ROTATE_YZ   1
#define MOUSE_ZOOM      2

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;

GLfloat zoomFactor = 1.0;

bool _2dmode = true;
bool displayCP = false;
bool wireframe = false;

/* local function declarations */
void init(void);
void setUp2DMode();
void display(void);
void rotateCamera(double deg, int axis);
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

	/*
	 * See drawing.c for the definition of these routines.
	 *
	 * Note: Only one should be called at a time (based on the
	 * display mode).
	 */
	  setup2DMode();
    if(_2dmode){
      draw2DPoints();
      if(num_i0_pts > 1){
        draw2DLines();
      }
    }else{
	  if(displayCP)
      draw3DPoints();
	  else if(wireframe)
      draw3DLines();
    else
      drawSurface();
    }

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void rotateCamera(double deg, int axis) {
  double x, y, z;

  x = 0;
  y = 0;
  z = 0;

  if (axis == X_AXIS) {
    x = 1.0f;
  } else if (axis == Y_AXIS) {
    y = 1.0f;
  } else if (axis == Z_AXIS) {
    z = 1.0f;
  }
 
  glRotatef(deg, x, y, z);
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'q':
			endSubdiv(0);
			break;

    case 'w':
	  if(!_2dmode){
      printf("Warning: already in 3D");
		  break;
	  }else if(num_i0_pts > 4){
		  _2dmode = false;
      generate3D();
      printf("Rotating into 3D...\n");
      display();
    }else
      printf("Warning: Not enough control points to perform rotation\n");
      break;

    case 'z' :
      if(!_2dmode) 
        printf("Switching back to 2D...\n");
      _2dmode = true;
      display();
      break;
	
	  case 'r' :
	    if(!_2dmode)
	      displayCP = !displayCP;
	    else printf("Warning: cannot do this in 2D");
	    display();
	    break;

    case 'e' : 
      if(!_2dmode)
        wireframe = !wireframe;
      else printf("Warning: cannot do this in 2D");
      display();
      break;

    // case 'a' :
    //   if(!_2dmode)
    //     applyVerticalSubdivision();
    //   display();
    //   break;

    // case 's' :
    //   if(!_2dmode)
    //     applyHorizontalSubdivision();
    //   display();
    //   break;

    case ',':
      rotateCamera(5, X_AXIS);
      break;

    case '<':
      rotateCamera(-5, X_AXIS);
      break;

    case '.':
      rotateCamera(5, Y_AXIS);
      break;

    case '>':
      rotateCamera(-5, Y_AXIS);
      break;

    case '/':
      rotateCamera(5, Z_AXIS);
      break;

    case '?':
      rotateCamera(-5, Z_AXIS);
      break;

		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();

	return;
}

void myMouseButton(int button, int state, int x, int y) {
  if(!_2dmode) return;
  if (state == GLUT_DOWN && !_2dmode) {
    m_last_x = x;
    m_last_y = y;

    if (button == GLUT_LEFT_BUTTON) {
      mouse_mode = MOUSE_ROTATE_YX;
    } else if (button == GLUT_MIDDLE_BUTTON) {
      mouse_mode = MOUSE_ROTATE_YZ;
    } else if (button == GLUT_RIGHT_BUTTON) {
      mouse_mode = MOUSE_ZOOM;
    }
  }

	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
      if(num_i0_pts < 30){
        int new_x = x - 200;
				if(new_x < 2)
					new_x = 1;
        int new_y = 200 - y;
		    i0_x[num_i0_pts] = new_x;
        i0_y[num_i0_pts] = new_y;
        i0[num_i0_pts].x = new_x;
        i0[num_i0_pts].y = new_y;
        i0[num_i0_pts].z = 0.0;
        i0[num_i0_pts].w = 0.0;
		    if(new_x > -20){
          num_i0_pts++;
          printf("x: %3d, y: %3d, i: %3d\n", new_x, new_y, num_i0_pts);
		    }
          display();
      }else
				  printf("Warning: Number of points has reached the maximum\n");
		}
  if (button == GLUT_RIGHT_BUTTON) {
    if(num_i0_pts > 0){
      num_i0_pts--;
			printf("Deleted point i: %3d\n", num_i0_pts);
      display();
    }else
			printf("Warning: No points to delete\n");
    }
  }
}

void myMouseMotion(int x, int y) {
  double d_x, d_y;  /* The change in x and y since the last callback */

  d_x = x - m_last_x;
  d_y = y - m_last_y;

  m_last_x = x;
  m_last_y = y;

  if (mouse_mode == MOUSE_ROTATE_YX) {
    /* scaling factors */
    d_x /= 2.0;
    d_y /= 2.0;

    glRotatef(d_x, 0.0, 1.0, 0.0);  /* y-axis rotation */
    glRotatef(-d_y, 1.0, 0.0, 0.0); /* x-axis rotation */

  } else if (mouse_mode == MOUSE_ROTATE_YZ) {
    /* scaling factors */
    d_x /= 2.0;
    d_y /= 2.0;

    glRotatef(d_x, 0.0, 1.0, 0.0);  /* y-axis rotation */
    glRotatef(-d_y, 0.0, 0.0, 1.0); /* z-axis rotation */

  } else if (mouse_mode == MOUSE_ZOOM) {
    d_y /= 100.0;

    zoomFactor += d_y;

    if (zoomFactor <= 0.0) {
      /* The zoom factor should be positive */
      zoomFactor = 0.001;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
     * glFrustum must receive positive values for the near and far
     * clip planes ( arguments 5 and 6 ).
     */
    glFrustum(fleft*zoomFactor, fright*zoomFactor,
      fbottom*zoomFactor, ftop*zoomFactor,
      -zNear, -zFar);
  }

  /* Redraw the screen */
  glutPostRedisplay();
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}

/* end of subdiv.cpp */
