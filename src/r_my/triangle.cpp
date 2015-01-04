#include "../common/middefs.h"
#include "../common/renderer.h"
#include "my_ddraw.h"
#include "my_3d.h"

void swap_int (int* p1, int* p2) {
	int temp=*p1;
	*p1=*p2;
	*p2=temp;
}
#define INSIDE1 1
#define INSIDE2 2

#define DRAW1 4
#define DRAW2 8
#define MY_MAX(v1,v2) ((v1>v2)?v1:v2)
#define MY_MIN(v1,v2) ((v1<v2)?v1:v2)

extern lame_color r_color;

void oldtriangle (
	int x1, int y1,
	int x2, int y2,
	int x3, int y3) {

	lame_color c=r_color;

	if (y1>y2) {
		swap_int (&x1, &x2);
		swap_int (&y1, &y2);
	}
	if (y1>y3) {
		swap_int (&x1, &x3);
		swap_int (&y1, &y3);
	}
	if (y2>y3) {
		swap_int (&x2, &x3);
		swap_int (&y2, &y3);
	}
	if (y1==y3) return;

	int flag;
	int x, y;
	int y11, y12, y21, y22;
	int left, right;
	float f_x12, f_x12_step, f_x13, f_x13_step, f_x23, f_x23_step;

	flag=0;

	if (y1!=y2)
		flag|=DRAW1;
	if (y2!=y3)
		flag|=DRAW2;


	y11=y1;
	y12=y2;
	y21=y2;
	y22=y3;

	f_x12_step=0;
	f_x13_step=0;
	f_x23_step=0;

	if (flag & DRAW1)
		f_x12_step=float(x2-x1)/float(y2-y1);
	f_x13_step=float(x3-x1)/float(y3-y1);
	if (flag & DRAW2)
		f_x23_step=float(x3-x2)/float(y3-y2);

	int xm=x1+f_x13_step*float(y2-y1);

	if (flag & (DRAW1)) {
		f_x12=x1+f_x12_step*float(y11-y1);
		f_x13=x1+f_x13_step*float(y11-y1);
		for (y=y11; y<=y12; y++) {
			if (f_x12<f_x13) {
				left=(int)f_x12;
				right=(int)f_x13;
			} else {
				left=(int)f_x13;
				right=(int)f_x12;
			}
			for (x=left; x<=right; x++)
				STOREPIXELSAFE (x,y,c);
			f_x12+=f_x12_step;
			f_x13+=f_x13_step;
		}
	}
	if (flag & (DRAW2)) {
		f_x23=x2+f_x23_step*float(y21-y2);
		f_x13=xm+f_x13_step*float(y21-y2);
		for (y=y21; y<=y22; y++) {
			if (f_x23<f_x13) {
				left=(int)f_x23;
				right=(int)f_x13;
			} else {
				left=(int)f_x13;
				right=(int)f_x23;
			}
			for (x=left; x<=right; x++)
				STOREPIXELSAFE (x,y,c);
			f_x23+=f_x23_step;
			f_x13+=f_x13_step;
		}
	}
}
