#ifndef INC_117_FONT_H
#define INC_117_FONT_H

#include "pic.h"

#define VFNT_MAGIC 0x544E4656
#define VFNT_VERSION 0

class CGLFont {
public:
	CGLFont ();
	~CGLFont ();

	VOID UnLoad ();
	v_result Load (const char*);
	v_result GetStrDim (const char*, int*, int*);
public:
	CGLPic* ptr[256];
};

#endif