#include "../common/middefs.h"
#include "r_gl.h"
#include "3d.h"

extern r_parms_t GL_parms;

void GL_SwitchTo3D (int x, int y, int width, int height, float fov) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT|GL_VIEWPORT_BIT|GL_TRANSFORM_BIT|GL_COLOR_BUFFER_BIT );

	/* Enable/Disable */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);

	/* matrices */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(x, GL_parms.height-y-height, width, height);
	gluPerspective(fov/V_PI*180.0f, (float)(width)/(float)(height), 0.1f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear (GL_DEPTH_BUFFER_BIT);
}

void GL_Restore2D () {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}

void GL_Face3D (	float x1, float y1, float z1, 
					float x2, float y2, float z2,
					float x3, float y3, float z3) {

	glBegin(GL_TRIANGLES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);
	glEnd ();
	
}
void GL_Line3D (float x1, float y1, float z1, float x2, float y2, float z2) {
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd ();
}

void GL_MatrixReset () {
	glLoadIdentity();
}
void GL_MatrixMove (float x, float y, float z) {
	glTranslatef(x,y,z);
}
void GL_MatrixScale (float x, float y, float z) {
	glScalef(x,y,z);
}
void GL_MatrixRotateX (float a) {
	glRotatef(a/V_PI*180.0f,1.0f,0.0f,0.0f);
}
void GL_MatrixRotateY (float a) {
	glRotatef(a/V_PI*180.0f,0.0f,1.0f,0.0f);
}
void GL_MatrixRotateZ (float a) {
	glRotatef(a/V_PI*180.0f,0.0f,0.0f,0.1f);
}
