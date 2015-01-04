#include "stdafx.h"
#include "../common/middefs.h"
#include "../common/bitmap.h"
#include "../common/renderer.h"
#include "my_ddraw.h"
#include "pic.h"

CSoftPic::CSoftPic () {
	width=height=0;
	ptr=0;
	a_ptr=0;
}
CSoftPic::~CSoftPic() {
	Free ();
}
v_result CSoftPic::Alloc() {
	ptr=MemAlloc(lame_color, width*height);
	if (!ptr) {
		width=height=0;
		return V_FAIL;
	}
	a_ptr=MemAlloc(v_uint8, width*height);
	if (!a_ptr) {
		MemFree (ptr);
		width=height=0;
		return V_FAIL;
	}
	return V_OK;
}
v_result CSoftPic::Free() {
	MemFree (ptr);
	MemFree (a_ptr);
	return V_OK;
}

v_result CSoftPic::LoadStraight (C117Bitmap* p_bmp) {
	if (!p_bmp) return V_FAIL;
	if (ptr) Free ();
	width=p_bmp->width;
	height=p_bmp->height;
	if (Alloc() != V_OK) {
		width=height=0;
		return V_FAIL;
	}
	int i,j;
	for (j=0; j<p_bmp->height; j++) {
		for (i=0; i<p_bmp->width; i++) {
			ptr[j*width+i]=GETCOLOR (p_bmp->GetRed(i,j), p_bmp->GetGreen(i,j), p_bmp->GetBlue(i,j));
		}
	}
	return V_OK;
}

v_result CSoftPic::Load (const char* filename) {
	if (!filename || *filename==0)
		return V_FAIL;
	FILE* fff=fopen (filename, "rb");
	if (!fff) return V_FAIL;
	if (LoadFrom (fff)!=V_OK) {
		fclose (fff);
		return V_FAIL;
	}
	fclose (fff);
	return V_OK;
}
v_result CSoftPic::LoadFrom (FILE* fff) {
	if (!fff)
		return V_FAIL;
	if (ptr)
		Free ();
	fread (&width, 4, 1, fff);
	fread (&height, 4, 1, fff);
	if (Alloc() != V_OK) {
		width=height=0;
		return V_FAIL;
	}
	int i,j;
	v_uint8 r,g,b,a;
	for (j=0; j<height; j++) {
		for (i=0; i<width; i++) {
			r=getc(fff)&0xFF;
			g=getc(fff)&0xFF;
			b=getc(fff)&0xFF;
			a=getc(fff)&0xFF;
			ptr[j*width+i]=GETCOLOR (r,g,b);
			a_ptr[j*width+i]=a;
		}
	}
	
	return V_OK;
}
v_result CSoftPic::LoadBMP (const char* filename) {
	C117Bitmap bmp;
	bmp.ModeRGBA();
	if (bmp.Load(filename) != V_OK)
		return V_FAIL;
	
	if (LoadStraight(&bmp) != V_OK) {
		bmp.Free();
		return V_FAIL;
	}
	
	bmp.Free();
	return V_OK;
	
}
v_result CSoftPic::LoadTGA (const char* filename) {
	C117Bitmap bmp;
	bmp.ModeRGBA();
	if (bmp.LoadTGA(filename) != V_OK)
		return V_FAIL;
	
	if (LoadStraight(&bmp) != V_OK) {
		bmp.Free();
		return V_FAIL;
	}
	
	bmp.Free();
	return V_OK;
}
