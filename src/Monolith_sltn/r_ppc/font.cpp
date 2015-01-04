#include "stdafx.h"
#include "../common/middefs.h"
#include "pic.h"
#include "font.h"

CSoftFont::CSoftFont() {
	for (int i=0; i<256; i++)
		ptr[i]=0;
}
CSoftFont::~CSoftFont() {
	UnLoad();
}

v_result CSoftFont::GetStrDim(const char* str, int* p_w, int* p_h) {
	if (!p_w || !p_h || !str)
		return V_FAIL;
	*p_w=*p_h=0;
	for (int i=0; str[i]!=0; i++) {
		*p_w+=ptr[str[i]]->width;
		if (ptr[str[i]]->height > *p_h)
			*p_h=ptr[str[i]]->height;
	}
	return V_OK;
}

v_result CSoftFont::Load(const char* filename) {
	if (!filename)
		return V_FAIL;
	if (!strcmp (filename, ""))
		return V_FAIL;
	FILE* fff=fopen (filename, "rb");
	if (fff==0)
		return V_FAIL;

	info_preheader_t ph;
	fread (&ph, sizeof (info_preheader_t), 1, fff);
	if (ph.magic!=VFNT_MAGIC) {
		fclose (fff);
		return V_FAIL;
	}
	if (ph.version==0) {
		for (int i=0; i<256; i++) {
			ptr[i]=new CSoftPic;
			ptr[i]->LoadFrom(fff);
		}
	} else {
		fclose (fff);
		return V_FAIL;
	}

	fclose (fff);
	return V_OK;
}

void CSoftFont::UnLoad() {
	for (int i=0; i<256; i++) {
		if (ptr[i]) {
			delete ptr[i];
			ptr[i]=0;
		}
	}
}
