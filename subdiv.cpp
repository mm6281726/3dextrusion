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
GLfloat zNear   =  300.0;
GLfloat zFar    = -300.0;

/* Constants for specifying the 3 coordinate axes */
#define X_AXIS      0
#define Y_AXIS      1
#define Z_AXIS      2

/* The three different operations the mouse can control for the canvas */
#define MOUSE_ROTATE_YX   0
#define MOUSE_ROTATE_YZ   1
#define MOUSE_ZOOM      2

bool _2dmode = true;
bool displayCP = false;
bool wireframe = false;
bool phong = false;

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;

/* local function declarations */
void init(void);
void display(void);
void rotateCamera(double deg, int axis);
void resetCamera( void );
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void myMouseMotion(int x, int y);
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
  glutMotionFunc(myMouseMotion);
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
  glDisable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

	/*
	 * See drawing.c for the definition of these routines.
	 *
	 * Note: Only one should be called at a time (based on the
	 * display mode).
	 */
	  setupAxis();
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
      else{
        setupLighting();
        drawSurface();
      }
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

/*
 * Resets the viewing frustum and moves the drawing point to the center of
 * the frustum.
 */
void resetCamera( void ) {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-W/2, W/2, -H/2, H/2, -zNear, -zFar);
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
    case 'c':
      if(!_2dmode)
        resetCamera();
      display();
      printf("Camera reset.\n");
      break;

		case 'q':
			endSubdiv(0);
			break;

    case 'w':
	  if(!_2dmode)
      printf("Warning: already in 3D\n");
    else if(num_i0_pts > 4){
      printf("Rotating into 3D...\n");
      generate3D();
      _2dmode = false;
      display();
    }else
      printf("Warning: Not enough control points to perform rotation\n");
    break;

    case 'z' :
      if(!_2dmode){ 
        printf("Switching back to 2D...\n");
        resetCamera();
      }
      _2dmode = true; 
      display();
      break;
	
	  case 'r' :
	    if(!_2dmode)
	      displayCP = !displayCP;
	    else printf("Warning: cannot do this in 2D\n");
	    display();
	    break;

    case 'e' : 
      if(!_2dmode)
        wireframe = !wireframe;
      else printf("Warning: cannot do this in 2D\n");
      display();
      break;

    case 'a' :
      if(!_2dmode && subdiv_v < 6){
        subdiv_v++;
        applyVerticalSubdivision();
        printf("Applying vertical subdivision level %i\n", subdiv_v);
        display();
      }else if(subdiv_v > 5) printf("Warning: maximum vertical subdivisions reached\n");
      else if(_2dmode) printf("Warning: cannot do this in 2D\n");
      break;

    case 'b' :
      if(!_2dmode && subdiv_h < 6){
        subdiv_h++;
        applyHorizontalSubdivision();
        display();
        printf("Applying horizontal subdivision level %i\n", subdiv_h);
      }else if(subdiv_h > 5) printf("Warning: maximum horizontal subdivisions reached\n");
      else if(_2dmode) printf("Warning: cannot do this in 2D\n");
      break;

    case 'd' :
      if(!phong)
        printf("Switching to Phong shading\n");
      else
        printf("Switching to Gouraud shading\n");
      phong = !phong;
      display();
      break;

    case 's' :
      if(shininess < 128){
        shininess+=2;
        printf("Shininess is now %f\n", shininess);
        display();
      }
      else
        printf("Warning: maximum shininess reached\n");
      break;

    case 'S' :
      if(shininess > 0){
        shininess-=2;
        printf("Shininess is now %f\n", shininess);
        display();
      }
      else
        printf("Warning: minimum shininess reached\n");
      break;

    case ',':
      if(!_2dmode) rotateCamera(5, X_AXIS);
      break;

    case '<':
      if(!_2dmode) rotateCamera(-5, X_AXIS);
      break;

    case '.':
      if(!_2dmode) rotateCamera(5, Y_AXIS);
      break;

    case '>':
      if(!_2dmode) rotateCamera(-5, Y_AXIS);
      break;

    case '/':
      if(!_2dmode) rotateCamera(5, Z_AXIS);
      break;

    case '?':
      if(!_2dmode) rotateCamera(-5, Z_AXIS);
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
      mouse_mode = MOUSE_ZOOM;
    } else if (button == GLUT_RIGHT_BUTTON) {
      mouse_mode = MOUSE_ROTATE_YZ;
    }
  }

	else if (state == GLUT_DOWN && _2dmode) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
      if(num_i0_pts < 30){
        int new_x = x - 200;
				if(new_x < 2)
					new_x = 1;
        int new_y = 200 - y;
		    i0_x[num_i0_pts] = new_x;
        i0_y[num_i0_pts] = new_y;
		    if(new_x > -20){
          num_i0_pts++;
          num_draw_pts++;
          printf("x: %3d, y: %3d, i: %3d\n", new_x, new_y, num_i0_pts);
		    }
          display();
      }else
				  printf("Warning: Number of points has reached the maximum\n");
		}

  else if (button == GLUT_RIGHT_BUTTON && _2dmode) {
    if(num_i0_pts > 0){
      printf("Deleted point i: %3d\n", num_i0_pts);
      num_i0_pts--;
      num_draw_pts--;
      display();
    }else
			printf("Warning: No points to delete\n");
    }
  }
}

void myMouseMotion(int x, int y) {
  if(_2dmode) return;
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
     * glFrustum must receive positive values for the near and far
     * clip planes ( arguments 5 and 6 ).
     */
    glFrustum(fleft, fright,
      fbottom, ftop,
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
