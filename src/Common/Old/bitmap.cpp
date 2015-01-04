#include "../common/middefs.h"
#include "bitmap.h"

C117Bitmap::C117Bitmap () {
	width=height=0;
	ptr=0;
	r=g=b=0;
	a=255;
	ModeRGB ();
}

v_result C117Bitmap::Load (const char* filename, bool flip) {
	if (!filename) return V_FAIL;
	if (!strcmp(filename, "")) return V_FAIL;
	FILE* file=fopen (filename, "rb");
	if (!file)	return V_FAIL;

	bmp_fh_t fh;
	bmp_ih_t ih;

	fread (&fh, sizeof (fh), 1, file);
	fread (&ih, sizeof (ih), 1, file);

	if (!CheckBPP(ih.bitcount)) {
		fclose (file);
		return V_FAIL;
	}

	if (ptr==0) {
		if ((ptr=new v_uint8[ih.width*ih.height*bytes])==0) {
			fclose (file);
			return V_FAIL;
		}
	} else {
		if (width!=ih.width || height!=ih.height) {
			delete[] ptr;
			if ((ptr=new v_uint8[ih.width*ih.height*bytes])==0) {
				fclose (file);
				return V_FAIL;
			}
		}
	}
	width=ih.width;
	height=ih.height;
	
	int x, y, z, y1, y2, ys;
	if (flip) {
		y1=height-1; y2=-1; ys=-1;
	} else {
		y1=0; y2=height; ys=1;
	}
	if (ih.bitcount==24) {
		fseek (file, fh.dataoffset, SEEK_SET);
	

		for (y=y1; y!=y2; y+=ys) {
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
			delete[] ptr;
			fclose (file);
			return V_FAIL;
		}
		v_uint32 mask=(1<<ih.bitcount)-1, byte;

		for (x=0; x<(1<<ih.bitcount); x++) {
			fread (p_pal+x, sizeof(bmp_pal_t), 1, file);
		}

		fseek (file, fh.dataoffset, SEEK_SET);
		for (y=y1; y!=y2; y+=ys) {
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
			for (x=x&0x03;x!=0 && x<4;x++)
				fgetc(file);
		}
		delete[] p_pal;

	}

	fclose (file);
	return V_OK;
}

v_result C117Bitmap::Save (const char* filename, bool flip) {
	if (ptr==0)
		return V_FAIL;
	if (filename==0) return V_FAIL;
	if (!strcmp(filename, "")) return V_FAIL;
	
	FILE* file=fopen (filename, "wb");
	if (file==0)
		return V_FAIL;
	
	bmp_fh_t fh;
	bmp_ih_t ih;

	int temp;
	if (width&0x03==0)
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

	int x, y, y1, y2, ys;
	if (flip) {
		y1=height-1; y2=-1; ys=-1;
	} else {
		y1=0; y2=height; ys=1;
	}

	for (y=y1; y!=y2; y+=ys) {
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
		delete[] ptr;
}
