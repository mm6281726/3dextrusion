#ifndef _DRAWING_H_
#define _DRAWING_H_

/* Globals declared in drawing.cpp */
extern int subdiv_v; // The user-specified subdivision level, vertical
extern int subdiv_h; // The user-specified subdivision level, horizontal

extern GLfloat shininess; //shininess of object
extern bool phong;

/* Functions implemented in drawing.cpp */
void drawSurface(void);
void draw2DPoints(void);
void draw3DPoints(void);
void draw2DLines(void);
void draw3DLines(void);
void generate3D(void);
void setupAxis(void);
void setupLighting(void);

#endif	/* _DRAWING_H_ */
