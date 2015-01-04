#include "../common/middefs.h"
#include "bitmap.h"

#pragma pack(push, 1)
typedef struct {
	v_uint8		id_length;
	v_uint8		colormap_type;
	v_uint8		image_type;
	v_uint16	colormap_start;
	v_uint16	colormap_length;
	v_uint8		colormap_depth;

	v_uint16	xpos;
	v_uint16	ypos;
	v_uint16	width;
	v_uint16	height;
	v_uint8		depth;
	v_uint8		desc;
} tga_header_t;
#pragma pack(pop)

v_result C117Bitmap::LoadTGA (const char* filename) {
	if (!filename) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	if (!strcmp(filename, "")) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	FILE* file=fopen (filename, "rb");
	if (!file){
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}

	tga_header_t fh;
	fread (&fh, sizeof(tga_header_t), 1, file);

	if (fh.colormap_type!=0 || fh.image_type!=2 || fh.depth!=32) {
		SetError(BMPERR_UNSUPPORTED_YET);
		fclose (file);
		return V_FAIL;
	}

	if (Alloc (fh.width, fh.height)!=V_OK) {
		SetError(BMPERR_ALLOC_FAILED);
		fclose (file);
		return V_FAIL;
	}

	fseek (file, sizeof(tga_header_t)+fh.id_length, SEEK_SET);
	int i, j;
	for (j=height-1; j>=0; j--) {
		for (i=0; i<width; i++) {
			SetBlue (i, j, getc(file));
			SetGreen (i, j, getc(file));
			SetRed (i, j, getc(file));
			SetAlpha (i, j, getc(file));
		}
	}

	fclose (file);
	return V_OK;
}


void main () {
	C117Bitmap bmp;
	bmp.ModeRGBA ();
	if (bmp.LoadTGA("tga.tga") != V_OK) {
		printf ("Can't load!\nError: %d\n", bmp.GetError());
	} else {
		printf ("OK!\n");
	}
	if (bmp.Save("bmp.bmp") != V_OK) {
		printf ("Can't save!\nError: %d\n", bmp.GetError());
	} else {
		printf ("OK!\n");
	}
	bmp.Free();

}