#ifndef _DRAWING_H_
#define _DRAWING_H_

/* Globals declared in drawing.cpp */
extern int subdiv_v; // The user-specified subdivision level, vertical
extern int subdiv_h; // The user-specified subdivision level, horizontal

/* Functions implemented in drawing.cpp */
void drawSurface(void);
void drawPoints(void);
void drawLines(void);
void draw3DLines(void);
void generate3D(void);

#endif	/* _DRAWING_H_ */
