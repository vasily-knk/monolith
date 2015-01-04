#ifndef INC_117_PIC_H
#define INC_117_PIC_H

#include "r_gl.h"
#include "../common/bitmap.h"
class CGLPic {
public:
	CGLPic ();
	~CGLPic ();
public:
	v_result Load (const char*);
	v_result LoadFrom (FILE*);
	v_result LoadBMP (const char*);
	v_result LoadTGA (const char*);
	v_result LoadStraight (C117Bitmap*);
	void Detach	();
public:
	gl_tex_t id;
	bool attached;
	int width, height;
};

#endif