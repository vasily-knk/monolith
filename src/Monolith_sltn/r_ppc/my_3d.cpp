#include "stdafx.h"
#include "../common/middefs.h"
#include "../common/renderer.h"
#include "my_ddraw.h"
#include "my_3d.h"
#include "117_matrix.h"
#include "vector.h"

float* z_buffer=0;

C117Matrix ms_matrix;
int ms_rel_x, ms_rel_y;
float ms_zfactor=1;
void Project (Vector3D vec, int *p_x, int *p_y) {
	*p_x=ms_rel_x-(int)(vec.x/vec.z*ms_zfactor);
	*p_y=ms_rel_y+(int)(vec.y/vec.z*ms_zfactor);
}

void MS_SwitchTo3D		(int x, int y, int width, int height, float fov) {
	ms_rel_x=x+width/2;
	ms_rel_y=y+height/2;
	ms_zfactor=(float(height)/2.0f)/tan(fov/2.0f);
	if (!z_buffer) {
		z_buffer=new float [dd_width*dd_height];
	}
}
void MS_Restore2D		() {}



void MS_Line (int,int,int,int);
void MS_Line3D			(float f_x1, float f_y1, float f_z1, float f_x2, float f_y2, float f_z2) {
	int x1,y1,x2,y2;
	Vector3D v1=Vector3D(f_x1,f_y1,f_z1), v2=Vector3D(f_x2,f_y2,f_z2);
	v1=VecMatrixProduct(v1,ms_matrix);
	v2=VecMatrixProduct(v2,ms_matrix);
	
	Project (v1, &x1, &y1);
	Project (v2, &x2, &y2);
	MS_Line (x1, y1, x2, y2);
}

void MS_MatrixReset 	() {
	float arr[4][4]={
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
	ms_matrix.Copy(arr);
}
void MS_MatrixMove		(float x, float y, float z) {
	float arr[4][4]={
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{x,y,z,1},
	};
	C117Matrix temp_mat (arr);
	ms_matrix=temp_mat*ms_matrix;
}
void MS_MatrixScale		(float x, float y, float z) {
	float arr[4][4]={
		x,0,0,0,
		0,y,0,0,
		0,0,z,0,
		0,0,0,1,
	};
	C117Matrix temp_mat (arr);
	ms_matrix=temp_mat*ms_matrix;
}
void MS_MatrixRotateX	(float angle) {
	float arr[4][4]={
		{1.0, 0.0,			0.0,		0.0}, 
		{0.0, cos(-angle),	-sin(-angle), 0.0}, 
		{0.0, sin(-angle),	cos(-angle), 0.0}, 
		{0.0, 0.0,			0.0,		1.0}, 
	};
	C117Matrix temp_mat (arr);
	ms_matrix=temp_mat*ms_matrix;
}
void MS_MatrixRotateY	(float angle) {
	float arr[4][4]={
		{cos(-angle), 0.0,	sin(-angle), 0.0}, 
		{0.0,		 1.0,	0.0,		 0.0}, 
		{-sin(-angle), 0.0,	cos (-angle), 0.0}, 
		{0.0,		 0.0,	0.0,		 1.0}, 
	};
	C117Matrix temp_mat (arr);
	ms_matrix=temp_mat*ms_matrix;
}
void MS_MatrixRotateZ	(float angle) {
	float arr[4][4]={
		{cos(-angle),	 -sin(-angle), 0.0, 0.0}, 
		{sin(-angle),	 cos(-angle), 0.0, 0.0}, 
		{0.0,			 0.0,		 1.0, 0.0}, 
		{0.0,			 0.0,		 0.0, 1.0}, 
	};
	C117Matrix temp_mat (arr);
	ms_matrix=temp_mat*ms_matrix;
}


void oldtriangle (
	int x1, int y1,
	int x2, int y2,
	int x3, int y3, float z1, float z2, float z3);

void MS_Face3D	(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	int ix1,iy1,ix2,iy2, ix3,iy3;
	Vector3D v1=Vector3D(x1,y1,z1), v2=Vector3D(x2,y2,z2), v3=Vector3D(x3,y3,z3);
	v1=VecMatrixProduct(v1,ms_matrix);
	v2=VecMatrixProduct(v2,ms_matrix);
	v3=VecMatrixProduct(v3,ms_matrix);
	
	Vector3D n=CrossProduct(v2-v1, v3-v1);

	Vector3D temp_vec=(v1+v2+v3)/3.0f;
	float ccc=DotProduct(temp_vec.Normalize(),n);
	if (ccc<=0) return;

	Project (v1, &ix1, &iy1);
	Project (v2, &ix2, &iy2);
	Project (v3, &ix3, &iy3);

	oldtriangle(ix1,iy1,ix2,iy2,ix3,iy3, v1.z, v2.z, v3.z);
}
