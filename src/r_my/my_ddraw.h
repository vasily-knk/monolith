#ifndef INC_117_MY_DDRAW_H
#define INC_117_MY_DDRAW_H

#include <ddraw.h>

v_result MyDDrawInit (int w, int h, int bpp, bool fs, WNDPROC proc);
void MyDDrawClose ();
v_result MyDDrawBegin ();
v_result MyDDrawEnd ();

#define LAME_BPP 16
#define LAME_SIZE 2
typedef v_uint16 lame_color;

extern int dd_width, dd_height, dd_bpp;
extern v_uint32 dd_rbits, dd_gbits, dd_bbits;
extern v_uint32 dd_roffs, dd_goffs, dd_boffs;
extern v_uint32 dd_lpitch;
extern lame_color *dd_ptr;

#define GETRED(c)	((c>>dd_roffs<<(8-dd_rbits))&0xFF)
#define GETGREEN(c)	((c>>dd_goffs<<(8-dd_gbits))&0xFF)
#define GETBLUE(c)	((c>>dd_boffs<<(8-dd_bbits))&0xFF)
#define GETCOLOR(r,g,b) (((r)>>(8-dd_rbits)<<(dd_roffs))|((g)>>(8-dd_gbits)<<(dd_goffs))|((b)>>(8-dd_bbits)<<(dd_boffs)))

#define STOREPIXELABS(pos,c)	dd_ptr[pos]=(c)
#define STOREPIXEL(x,y,c)		dd_ptr[(y)*dd_lpitch+(x)]=(c)
extern int r_x1, r_y1, r_x2, r_y2;
#define STOREPIXELSAFE(x,y,c)		if((x)>=r_x1&&(x)<r_x2&&(y)>=r_y1&&(y)<r_y2)dd_ptr[(y)*dd_lpitch+(x)]=(c)

#endif