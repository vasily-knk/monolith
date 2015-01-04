#ifndef INC_117_PIC_H
#define INC_117_PIC_H

#include "../common/bitmap.h"
#include "my_ddraw.h"

class CSoftPic {
public:
	CSoftPic ();
	~CSoftPic();
public:
	v_result Alloc ();
	v_result Free ();
public:
	v_result Load (const char*);
	v_result LoadFrom (FILE*);
	v_result LoadBMP (const char*);
	v_result LoadTGA (const char*);
	v_result LoadStraight (C117Bitmap*);
public:
	int width, height;
	lame_color* ptr;
	v_uint8* a_ptr;
};

#define PIC_PTR_GETPIXEL(p,x,y) (p)->ptr[(y)*(p)->width+(x)]
#define PIC_PTR_GETALPHA(p,x,y) (p)->a_ptr[(y)*(p)->width+(x)]

#endif