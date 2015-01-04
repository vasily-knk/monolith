#ifndef INC_117_MY_3D_H
#define INC_117_MY_3D_H

void MS_SwitchTo3D		(int, int, int, int, float);
void MS_Restore2D		();
void MS_Face3D			(float, float, float, float, float, float, float, float, float);
void MS_Line3D			(float, float, float, float, float, float);

void MS_MatrixReset 		();
void MS_MatrixMove		(float x, float y, float z);
void MS_MatrixScale		(float x, float y, float z);
void MS_MatrixRotateX	(float a);
void MS_MatrixRotateY	(float a);
void MS_MatrixRotateZ	(float a);

#endif
