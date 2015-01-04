#ifndef INC_117_MATRIX_H
#define INC_117_MATRIX_H

#define MATRIX_WIDTH	4
#define MATRIX_HEIGHT	4

#include "vector.h"

class C117Matrix { 
public:
	float value [MATRIX_WIDTH][MATRIX_HEIGHT];
public:
	C117Matrix ();
	C117Matrix (C117Matrix& p_mat);
	C117Matrix (float new_value[4][4]);
public:
	//C117Matrix operator= (const C117Matrix& m);
	C117Matrix operator+ (const C117Matrix& m);
	C117Matrix operator- (const C117Matrix& m);
	C117Matrix operator* (const C117Matrix& m);

	void operator+= (const C117Matrix& m);
	void operator-= (const C117Matrix& m);
	void operator*= (const C117Matrix& m);
public:
	float GetValue (int ypos, int xpos) {return value[ypos][xpos];}
	float SetValue (int ypos, int xpos, float new_value) {return value[ypos][xpos]=new_value;}

	void Copy (C117Matrix& p_mat);
	void Copy (float new_value[4][4]);
};

Vector3D VecMatrixProduct (const Vector3D& vec,C117Matrix& m);

#endif
