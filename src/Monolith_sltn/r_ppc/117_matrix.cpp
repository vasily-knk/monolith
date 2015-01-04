#include "stdafx.h"
#include "../common/middefs.h"
#include "vector.h"
#include "117_matrix.h"

C117Matrix::C117Matrix () {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i< 4; i++)
			value[j][i]=0.0;
}
C117Matrix::C117Matrix (C117Matrix& p_mat) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i< 4; i++)
			value[j][i]=p_mat.value[j][i];

}
C117Matrix::C117Matrix (float new_value[4][4]) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i< 4; i++)
			value[j][i]=new_value[j][i];
}

/*C117Matrix C117Matrix::operator= (const C117Matrix& m) {
	Copy (m);
}*/
C117Matrix C117Matrix::operator+ (const C117Matrix& m) {
	int i, j; 
	C117Matrix res;
	for (j=0; j<4; j++)
		for (i=0; i<4; i++)
			res.value[j][i]=value[j][i]+m.value[j][i];
	return res;
}
C117Matrix C117Matrix::operator- (const C117Matrix& m) {
	int i, j; 
	C117Matrix res;
	for (j=0; j<4; j++)
		for (i=0; i<4; i++)
			res.value[j][i]=value[j][i]+m.value[j][i];
	return res;
}
C117Matrix C117Matrix::operator* (const C117Matrix& m) {
	int i, j, k; 
	C117Matrix res;
	for (j=0; j<4; j++) 
		for (i=0; i<4; i++) 
			for (k=0; k<4; k++) 
				res.value[j][i]+=value[j][k]*m.value[k][i];
	return res;
}

void C117Matrix::operator+= (const C117Matrix& m) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i<4; i++)
			value[j][i]+=m.value[j][i];
}
void C117Matrix::operator-= (const C117Matrix& m) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i<4; i++)
			value[j][i]-=m.value[j][i];
}
void C117Matrix::operator*= (const C117Matrix& m) {
	C117Matrix res;
	res = (*this) *  m;
	Copy (res);
}

void C117Matrix::Copy (C117Matrix& p_mat) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i< 4; i++)
			value[j][i]=p_mat.value[j][i];
}
void C117Matrix::Copy (float new_value[4][4]) {
	int i, j; 
	for (j=0; j<4; j++)
		for (i=0; i< 4; i++)
			value[j][i]=new_value[j][i];
}
Vector3D VecMatrixProduct (const Vector3D& vec,C117Matrix& m) {
	return Vector3D (
		vec.x*	m.GetValue(0, 0) + 
		vec.y*	m.GetValue(1, 0) + 
		vec.z*	m.GetValue(2, 0) +
				m.GetValue(3, 0), 

		vec.x*	m.GetValue(0, 1) + 
		vec.y*	m.GetValue(1, 1) + 
		vec.z*	m.GetValue(2, 1) +
				m.GetValue(3, 1), 

		vec.x*	m.GetValue(0, 2) + 
		vec.y*	m.GetValue(1, 2) + 
		vec.z*	m.GetValue(2, 2) +
				m.GetValue(3, 2)

		);
}
