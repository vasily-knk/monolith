#ifndef INC_117_BITMAP_H
#define INC_117_BITMAP_H

#define BITMAP_FLAG_R	(1<<0)
#define BITMAP_FLAG_G	(1<<1)
#define BITMAP_FLAG_B	(1<<2)
#define BITMAP_FLAG_A	(1<<3)

class C117Bitmap {
public:
	C117Bitmap ();
	
	inline void ModeRGBA ();
	inline void ModeRGB ();
	inline void ModeBGR ();
	inline void ModeRedonly ();

	v_result Load (const char*, bool flip=true);
	v_result Save (const char*, bool flip=true);
	void Free ();
	bool CheckBPP (int n) {return (n==1)||(n==2)||(n==4)||(n==8) || (n==24);}
public:
	inline v_uint8	GetRed		(int, int);
	inline v_uint8	GetGreen	(int, int);
	inline v_uint8	GetBlue		(int, int);
	inline v_uint8	GetAlpha	(int, int);
	inline void		SetRed		(int, int, v_uint8);
	inline void		SetGreen	(int, int, v_uint8);
	inline void		SetBlue		(int, int, v_uint8);
	inline void		SetAlpha	(int, int, v_uint8);
public:
	int width, height;
	v_uint8 bytes;
	v_uint8 r_index, g_index, b_index, a_index;
	v_uint8 r,g,b,a;
	v_uint32 flags;
	v_uint8* ptr;
};

void C117Bitmap::ModeRGBA () {
	bytes=4;
	r_index=0;	g_index=1;	b_index=2;	a_index=3;
	flags = BITMAP_FLAG_R | BITMAP_FLAG_G | BITMAP_FLAG_B | BITMAP_FLAG_A;
}
void C117Bitmap::ModeRGB () {
	bytes=3;
	r_index=0;	g_index=1;	b_index=2; a_index=0;
	flags = BITMAP_FLAG_R | BITMAP_FLAG_G | BITMAP_FLAG_B;
}
void C117Bitmap::ModeBGR () {
	bytes=3;
	b_index=0;	g_index=1;	r_index=2; a_index=0;
	flags = BITMAP_FLAG_R | BITMAP_FLAG_G | BITMAP_FLAG_B;
}
void C117Bitmap::ModeRedonly () {
	bytes=1;
	r_index=0;	g_index=0;	b_index=0; a_index=0;
	flags = BITMAP_FLAG_R;
}

v_uint8 C117Bitmap::GetRed (int x, int y) {
	if (!ptr || !(flags & BITMAP_FLAG_R)) return r;
	return ptr[(y*width+x)*bytes+r_index];
}
v_uint8 C117Bitmap::GetGreen (int x, int y) {
	if (!ptr || !(flags & BITMAP_FLAG_G)) return g;
	return ptr[(y*width+x)*bytes+g_index];
}
v_uint8 C117Bitmap::GetBlue (int x, int y) {
	if (!ptr || !(flags & BITMAP_FLAG_B)) return b;
	return ptr[(y*width+x)*bytes+b_index];
}
v_uint8 C117Bitmap::GetAlpha (int x, int y) {
	if (!ptr || !(flags & BITMAP_FLAG_A)) return a;
	return ptr[(y*width+x)*bytes+a_index];
}

void C117Bitmap::SetRed (int x, int y, v_uint8 val) {
	if (!ptr || !(flags & BITMAP_FLAG_R)) return;
	ptr[(y*width+x)*bytes+r_index]=val;
}
void C117Bitmap::SetGreen (int x, int y, v_uint8 val) {
	if (!ptr || !(flags & BITMAP_FLAG_G)) return;
	ptr[(y*width+x)*bytes+g_index]=val;
}
void C117Bitmap::SetBlue (int x, int y, v_uint8 val) {
	if (!ptr || !(flags & BITMAP_FLAG_B)) return;
	ptr[(y*width+x)*bytes+b_index]=val;
}
void C117Bitmap::SetAlpha (int x, int y, v_uint8 val) {
	if (!ptr || !(flags & BITMAP_FLAG_A)) return;
	ptr[(y*width+x)*bytes+a_index]=val;
}

#pragma pack (1)
	struct bmp_fh_t {
		unsigned short signature;
		unsigned long filesize;
		unsigned long reserved;
		unsigned long dataoffset;
	};
	struct bmp_ih_t {
		unsigned long size;
		long width;
		long height;
		unsigned short planes;
		unsigned short bitcount;
		unsigned long compression;
		unsigned long imagesize;
		unsigned long xppm;
		unsigned long yppm;
		unsigned long colorsused;
		unsigned long colorsimportant;
	};
	typedef struct {
		unsigned char b, g, r, a;
	} bmp_pal_t;
#pragma pack (4)


#endif