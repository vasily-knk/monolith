#include "stdafx.h"
#include "../common/middefs.h"
#include "../common/renderer.h"
#include "../common/brez.h"

#include "my_ddraw.h"
#include "my_3d.h"
#include "pic.h"
#include "font.h"

r_parms_t r_parms;

// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------
v_result	MS_GetParms		(r_parms_t* p) {
	if (p==0) return V_FAIL;
	memcpy (p, &r_parms, sizeof(r_parms_t));
	return V_OK;
}

void MS_SetColor (v_uint8 r, v_uint8 g, v_uint8 b);
void MS_SetScale (float,float);
void MS_Clip (int x1, int y1, int x2, int y2);

v_result	MS_Init			(r_parms_t* p) {
	_MemInit ("ms_mem.txt");

	memcpy (&r_parms, p, sizeof(r_parms_t));
	if (MyDDrawInit (p->width, p->height, p->bpp, p->fullscreen, p->wndproc)!=V_OK)
		return V_FAIL;

	MS_SetColor (255,255,255);
	MS_SetScale(1,1);
	MS_Clip (0,0, p->width, p->height);
	z_buffer=0;

	return V_OK;
}

v_result	MS_Close			() {
	if (z_buffer)
		delete[] z_buffer;
	MyDDrawClose();
	r_parms.width=r_parms.height=r_parms.bpp=0;
	r_parms.fullscreen=false;
	MemClose();
	return V_OK;
}

v_result MS_Begin () {
	if (z_buffer) {
		for (int y=0; y<dd_height; y++) 
			for (int x=0; x<dd_width; x++)
				SETZBUFFER(x,y, -10000);
	}
	return MyDDrawBegin ();
}
v_result MS_End () {
	return MyDDrawEnd ();
}

// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------
lame_color r_color=(lame_color)0xFFFFFFFF;
float r_sx=1.0f, r_sy=1.0f;
int r_x1, r_y1, r_x2, r_y2;
CSoftFont* r_font=0;

void MS_SetColor (v_uint8 r, v_uint8 g, v_uint8 b) {
	r_color=GETCOLOR(r,g,b);
}
void MS_SetAlpha (v_uint8) {
}
void MS_SetScale (float sx, float sy) {
	r_sx=sx; r_sy=sy;
}
void MS_SetRot (float) {
}
void MS_SetFont (r_font_t new_font) {
	r_font=(CSoftFont*)new_font;
}
void MS_Clip (int x1, int y1, int x2, int y2) {
	r_x1=x1; r_y1=y1; r_x2=x2; r_y2=y2;
}
void MS_GetClip (int* p_x1, int* p_y1, int* p_x2, int* p_y2) {
	if (p_x1) *p_x1=r_x1;
	if (p_y1) *p_y1=r_y1;
	if (p_x2) *p_x2=r_x2;
	if (p_y2) *p_y2=r_y2;
}

// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------
void MS_Clear () {
	memset (dd_ptr, 0, dd_height*dd_lpitch*LAME_SIZE);
}

void MS_Line (int x1, int y1, int x2, int y2) {
	brez_t brez;
	BrezInit (&brez, x1, y1, x2, y2);
	while (!BREZ_FINISHED(brez)){
		STOREPIXELSAFE (brez.x, brez.y, r_color);
		BREZ_INCREASE(brez);
	}
}
void MS_Rect (int x1, int y1, int x2, int y2) {
	int temp;
	if (x1>x2) {temp=x1; x1=x2; x2=temp;}
	if (y1>y2) {temp=y1; x1=y2; y2=temp;}
	for (int i=x1; i<=x2; i++) {
		STOREPIXELSAFE (i, y1, r_color);
		STOREPIXELSAFE (i, y2, r_color);
	}
	for (i=y1; i<=y2; i++) {
		STOREPIXELSAFE (x1, i, r_color);
		STOREPIXELSAFE (x2, i, r_color);
	}
}
void MS_Box (int x1, int y1, int x2, int y2) {
	int temp;
	if (x1>x2) {temp=x1; x1=x2; x2=temp;}
	if (y1>y2) {temp=y1; x1=y2; y2=temp;}
	for (int j=y1; j<=y2; j++) {
		for (int i=x1; i<=x2; i++) {
			STOREPIXELSAFE (i, j, r_color);
		}
	}
}
// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------
v_result MS_LoadPic (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CSoftPic *p_pic=new CSoftPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->Load (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
v_result MS_LoadPicBMP (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CSoftPic *p_pic=new CSoftPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadBMP (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
v_result MS_LoadPicTGA (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CSoftPic *p_pic=new CSoftPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadTGA (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
v_result MS_LoadPicStraight (C117Bitmap* p_bmp, r_pic_t* dest) {
	if (p_bmp==0 || dest==0)
		return V_FAIL;
	CSoftPic *p_pic=new CSoftPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadStraight (p_bmp) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
void MS_UnLoadPic (r_pic_t p) {
	if (!p) return;
	CSoftPic *p_pic=(CSoftPic*)p;
	p_pic->Free();
	delete p_pic;
}
void MS_GetPicInfo (r_pic_t p, r_pic_info_t* dest) {
	if (p==0 || dest==0)
		return;
	CSoftPic* p_pic=(CSoftPic*)p;
	dest->width=p_pic->width;
	dest->height=p_pic->height;
}
#define PIC_MINALPHA 32
void MS_DrawPic (r_pic_t p, int x, int y) {
	if (p==0) return;
	CSoftPic* p_pic=(CSoftPic*)p;
	int scr_width=(int)((float)p_pic->width*r_sx),
		scr_height=(int)((float)p_pic->height*r_sy);

	brez_t x_brez, y_brez;
	lame_color c;
	BrezInit (&y_brez, 0, 0, scr_height-1, p_pic->height-1);
	while (!BREZ_FINISHED(y_brez)) {
		BrezInit (&x_brez, 0, 0, scr_width-1, p_pic->width-1);
		while (!BREZ_FINISHED(x_brez)) {
			if (PIC_PTR_GETALPHA(p_pic, x_brez.y, y_brez.y) >= PIC_MINALPHA) {
				c=PIC_PTR_GETPIXEL(p_pic, x_brez.y, y_brez.y);
				STOREPIXELSAFE (x+x_brez.x, y+y_brez.x, c);
			}

			BREZ_INCREASE(x_brez);
		}
		BREZ_INCREASE(y_brez);
	}
	
}


// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------

v_result MS_LoadFont (const char* filename, r_font_t* dest) {
	if (!filename || !dest)
		return V_FAIL;
	CSoftFont* p_fnt=new CSoftFont;
	if (p_fnt->Load(filename) != V_OK)
		return V_FAIL;
	*dest=p_fnt;
	return V_OK;
}

void MS_UnLoadFont (r_font_t font) {
	if (!font)
		return;
	CSoftFont* p_fnt=(CSoftFont*)font;
	p_fnt->UnLoad();
	delete p_fnt;
}
void MS_Text (const char* str, int x, int y) {
	if (!r_font) return;
	CSoftPic* letter;
	
	for (int i=0; str[i]!=0; i++) {
		letter=r_font->ptr[str[i]];
		MS_DrawPic (letter, x, y);
		x+=(int)((float)(letter->width)*r_sx);
	}
};
int MS_GetTextWidth (const char* str) {
	if (!r_font) return 0;
	int x,y;
	r_font->GetStrDim(str, &x, &y);
	x=(float)x * r_sx;
	return x;
}
int MS_GetTextHeight (const char* str) {
	if (!r_font) return 0;
	int x,y;
	r_font->GetStrDim(str, &x, &y);
	y=(float)y * r_sy;
	return y;
}

void MS_Hide () {
	MyDDrawHide ();
}

void MS_Restore () {
	MyDDrawRestore ();
}

// --------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------
void R_LoadAPI (API_renderer_t* p_ri) {
	if (!p_ri) return;
	p_ri->R_GetParms=MS_GetParms;
	p_ri->R_Init=MS_Init;
	p_ri->R_Close=MS_Close;
	p_ri->R_Begin=MS_Begin;
	p_ri->R_End=MS_End;

	p_ri->R_SetColor=MS_SetColor;
	p_ri->R_SetAlpha=MS_SetAlpha;
	p_ri->R_SetScale=MS_SetScale;
	p_ri->R_SetRot=MS_SetRot;

	p_ri->R_LoadPic=MS_LoadPic;
	p_ri->R_LoadPicBMP=MS_LoadPicBMP;
	p_ri->R_LoadPicTGA=MS_LoadPicTGA;
	p_ri->R_LoadPicStraight=MS_LoadPicStraight;
	p_ri->R_UnLoadPic=MS_UnLoadPic;
	p_ri->R_GetPicInfo=MS_GetPicInfo;
	p_ri->R_DrawPic=MS_DrawPic;

	p_ri->R_LoadFont=MS_LoadFont;
	p_ri->R_UnLoadFont=MS_UnLoadFont;
	p_ri->R_SetFont=MS_SetFont;
	p_ri->R_Text=MS_Text;
	p_ri->R_GetTextWidth=MS_GetTextWidth;
	p_ri->R_GetTextHeight=MS_GetTextHeight;

	p_ri->R_Clip=MS_Clip;
	p_ri->R_GetClip=MS_GetClip;

	p_ri->R_SwitchTo3D=MS_SwitchTo3D;
	p_ri->R_Restore2D=MS_Restore2D;
	p_ri->R_Face3D=MS_Face3D;
	p_ri->R_Line3D=MS_Line3D;

	p_ri->R_MatrixReset=MS_MatrixReset;
	p_ri->R_MatrixMove=MS_MatrixMove;
	p_ri->R_MatrixScale=MS_MatrixScale;
	p_ri->R_MatrixRotateX=MS_MatrixRotateX;
	p_ri->R_MatrixRotateY=MS_MatrixRotateY;
	p_ri->R_MatrixRotateZ=MS_MatrixRotateZ;

	p_ri->R_Clear=MS_Clear;
	p_ri->R_Line=MS_Line;
	p_ri->R_Rect=MS_Rect;
	p_ri->R_Box=MS_Box;

	p_ri->R_Hide=MS_Hide;
	p_ri->R_Restore=MS_Restore;
}


