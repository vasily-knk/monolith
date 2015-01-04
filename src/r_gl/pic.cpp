#include "../common/middefs.h"
#include "../common/bitmap.h"
#include "r_gl.h"
#include "pic.h"

int GL_filter=GL_LINEAR;

CGLPic::CGLPic () {
	id=0;
	attached=false;
	width=height=0;
}
CGLPic::~CGLPic () {
	if (attached)
		Detach();
}
void CGLPic::Detach () {
	if (!attached) return;
	if (!glIsTexture(id)) return;
	glDeleteTextures(1, &id);
}

v_result CGLPic::LoadFrom(FILE* fff) {
	if (!fff)
		return V_FAIL;
	fread (&width, 4, 1, fff);
	fread (&height, 4, 1, fff);

	C117Bitmap bmp, bmp1;
	bmp.ModeRGBA();
	bmp.width=width;
	bmp.height=height;
	bmp.ptr=MemAlloc (v_uint8, width*height*4); 

	v_uint32 size=width*height*4;
	for (v_uint32 i=0; i<size; i++) {
		fread (bmp.ptr+i, 1, 1, fff);
	}

	int w, h;
	for (w=1; w<width; w<<=1);
	for (h=1; h<height; h<<=1);
	
	bmp1.ModeRGBA();
	bmp1.width=w;
	bmp1.height=h;

	bmp1.ptr=MemAlloc (v_uint8, w*h*4);
	gluScaleImage (GL_RGBA, width, height, GL_UNSIGNED_BYTE, bmp.ptr, w, h, GL_UNSIGNED_BYTE, bmp1.ptr);

	glGenTextures(1, &id);
	attached=true;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_filter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp1.ptr);

	bmp.Free();
	bmp1.Free();
	return V_OK;	
}

v_result CGLPic::Load (const char* filename) {
	if (!filename)
		return V_FAIL;
	if (!strcmp (filename, ""))
		return V_FAIL;

	FILE* fff=fopen (filename, "rb");
	if (LoadFrom (fff)!=V_OK)
		return V_FAIL;
	
	return V_OK;
}

v_result CGLPic::LoadBMP (const char* filename) {
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
v_result CGLPic::LoadTGA (const char* filename) {
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

v_result CGLPic::LoadStraight (C117Bitmap* p_bmp) {
	if (p_bmp==0)
		return V_FAIL;

	width=p_bmp->width;
	height=p_bmp->height;

	int w, h;
	for (w=1; w<p_bmp->width; w<<=1);
	for (h=1; h<p_bmp->height; h<<=1);
	
	v_uint32* data1=MemAlloc (v_uint32, w*h);
	gluScaleImage (GL_RGBA, p_bmp->width, p_bmp->height, GL_UNSIGNED_BYTE, p_bmp->ptr, w, h, GL_UNSIGNED_BYTE, data1);

	glGenTextures(1, &id);
	attached=true;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_filter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);

	MemFree(data1);

	return V_OK;
}

