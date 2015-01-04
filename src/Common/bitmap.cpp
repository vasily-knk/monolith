#include "../common/middefs.h"
#include "bitmap.h"

C117Bitmap::C117Bitmap () {
	width=height=0;
	ptr=0;
	r=g=b=0;
	a=255;
	error=0;
	ModeRGB ();
}

v_result C117Bitmap::Alloc(int w, int h) {
	if (ptr==0) {
		if ((ptr=MemAlloc (v_uint8, w*h*bytes))==0) {
			return V_FAIL;
		}
	} else {
		if (width!=w || height!=h) {
			MemFree(ptr);
			if ((ptr=MemAlloc (v_uint8, w*h*bytes))==0) {
				return V_FAIL;
			}
		}
	}
	width=w;
	height=h;
	return V_OK;
}

v_result C117Bitmap::Load (const char* filename) {
	if (!filename) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	if (!strcmp(filename, "")) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	FILE* file=fopen (filename, "rb");
	if (!file) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}

	bmp_fh_t fh;
	bmp_ih_t ih;

	fread (&fh, sizeof (fh), 1, file);
	fread (&ih, sizeof (ih), 1, file);

	if (!CheckBPP(ih.bitcount)) {
		fclose (file);
		SetError (BMPERR_UNSUPPORTED);
		return V_FAIL;
	}

	if (Alloc (ih.width, ih.height)!=V_OK) {
		SetError (BMPERR_ALLOC_FAILED);
		fclose (file);
		return V_FAIL;
	}

	if (ih.bitcount==24) {
		fseek (file, fh.dataoffset, SEEK_SET);
		int x, y;

		for (y=height-1; y>=0; y--) {
			for (x=0; x<width; x++) {
				SetBlue (x, y, getc(file));
				SetGreen (x, y, getc(file));
				SetRed (x, y, getc(file));
				SetAlpha (x, y, a);
			}
			for (x=(x*3)&0x03;x!=0 && x<4;x++)
				fgetc(file);
		}
	} else {
		bmp_pal_t* p_pal=new bmp_pal_t[1<<ih.bitcount];
		if (!p_pal) {
			MemFree(ptr);
			fclose (file);
			SetError (BMPERR_ALLOC_FAILED);
			return V_FAIL;
		}
		int x, y, z;
		v_uint32 mask=(1<<ih.bitcount)-1, byte;

		for (x=0; x<(1<<ih.bitcount); x++) {
			fread (p_pal+x, sizeof(bmp_pal_t), 1, file);
		}

		fseek (file, fh.dataoffset, SEEK_SET);
		for (y=height-1; y>=0; y--) {
			for (x=0; x<width; x+=(8/ih.bitcount)) {
				byte=fgetc(file);
				for (z=0; z<8/ih.bitcount && (x+z)<width; z++) {
					SetRed		(x+z, y, p_pal[(byte>>(8-ih.bitcount))&mask].r);
					SetGreen	(x+z, y, p_pal[(byte>>(8-ih.bitcount))&mask].g);
					SetBlue		(x+z, y, p_pal[(byte>>(8-ih.bitcount))&mask].b);
					SetAlpha (x+z, y, a);
					byte<<=ih.bitcount;
					byte&=0xFF;
				}
			}
			for (x=x&0x03;x!=0 && x<4;x++) {
				fgetc(file);
			}
		}
		MemFree(p_pal);

	}

	fclose (file);
	return V_OK;
}

v_result C117Bitmap::Save (const char* filename) {
	if (ptr==0) {
		SetError (BMPERR_DATA_EMPTY);
		return V_FAIL;
	}
	if (filename==0) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	if (!strcmp(filename, "")) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	
	FILE* file=fopen (filename, "wb");
	if (file==0) {
		SetError (BMPERR_FILE_NOT_FOUND);
		return V_FAIL;
	}
	
	bmp_fh_t fh;
	bmp_ih_t ih;

	int temp;
	if ((width&0x03)==0)
		temp=0;
	else
		temp=width&0xFFFC+4;

	fh.signature='B' | ('M'<<8);
	fh.filesize=sizeof(fh)+sizeof(ih)+temp*height*3;
	fh.reserved=0;
	fh.dataoffset=sizeof(fh)+sizeof(ih);

	ih.size=sizeof(ih);
	ih.width=width;
	ih.height=height;
	ih.planes=1;
	ih.bitcount=24;
	ih.compression=0;
	ih.imagesize=width*height*3;
	ih.xppm=ih.yppm=0;
	ih.colorsused=ih.colorsimportant=0;


	fwrite (&fh, sizeof (fh), 1, file);
	fwrite (&ih, sizeof (ih), 1, file);

	int x, y;
	for (y=height-1; y>=0; y--) {
		for (x=0; x<width; x++) {
			fputc (GetBlue (x, y), file);
			fputc (GetGreen (x, y), file);
			fputc (GetRed (x, y), file);
		}
		for (x=(x*3)&0x03;x!=0 && x<4;x++)
			fputc(0, file);
	}

	fclose (file);
	return V_OK;
}

void C117Bitmap::Free () {
	if (ptr)
		MemFree(ptr);
}

