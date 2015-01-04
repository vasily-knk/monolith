#ifndef INC_117_3D_H
#define INC_117_3D_H

void GL_SwitchTo3D (int, int, int, int, float);
void GL_Restore2D ();
void GL_Face3D (	float x1, float y1, float z1, 
					float x2, float y2, float z2,
					float x3, float y3, float z3);
void GL_Line3D (float x1, float y1, float z1, float x2, float y2, float z2);

void GL_MatrixReset ();
void GL_MatrixMove (float x, float y, float z);
void GL_MatrixScale (float x, float y, float z);
void GL_MatrixRotateX (float a);
void GL_MatrixRotateY (float a);
void GL_MatrixRotateZ (float a);


#endif