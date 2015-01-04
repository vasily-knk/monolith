#include "../common/middefs.h"
#include "../common/renderer.h"
#include "../common/bitmap.h"
#include "r_gl.h"
#include "sprite.h"
#include "pic.h"
#include "font.h"
#include "3d.h"

HWND NeHe_CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag, WNDPROC NeHe_WndProc);
GLvoid NeHe_KillGLWindow(GLvoid);
void NeHe_GLSwap ();
void GL_Clip (int x1, int y1, int x2, int y2);

r_parms_t GL_parms={0,0,0,false};

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Main Stuff ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void GL_SetColor (v_uint8 r, v_uint8 g, v_uint8 b);
void GL_SetAlpha (v_uint8 a);

v_result GL_GetParms	(r_parms_t* p) {
	memcpy (p, &GL_parms, sizeof(r_parms_t));
	return V_OK;
}
v_result GL_Init	(r_parms_t* p) {
	_MemInit ("gl_mem.txt");
	memcpy (&GL_parms, p, sizeof (r_parms_t));

	if ((GL_parms.hWnd=NeHe_CreateGLWindow (

#ifdef _DEBUG	
		"Fucking OpenGL window"
#else
		"OpenGL window"
#endif

		
		, p->width, p->height, p->bpp, p->fullscreen, p->wndproc))==0)
		return V_FAIL;
	glViewport(0, 0, p->width, p->height);
	GL_Clip  (0,0,p->width, p->height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, p->width, p->height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	GL_SetColor(255,255,255);
	GL_SetAlpha(255);

	return V_OK;
}
v_result GL_Close	() {
	NeHe_KillGLWindow ();
	GL_parms.width=GL_parms.height=GL_parms.bpp=0;
	GL_parms.fullscreen=false;

	MemClose();
	return V_OK;
}

v_result GL_Begin	() {
	return V_OK;
}
v_result GL_End		() {
	NeHe_GLSwap();
	return V_OK;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Set- Stuff ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
v_uint8 GL_r, GL_g, GL_b, GL_a;
float GL_sx=1, GL_sy=1;
float GL_rot=0;
CGLFont* GL_font;

void GL_SetColor (v_uint8 r, v_uint8 g, v_uint8 b) {
	GL_r=r; GL_g=g; GL_b=b;
	glColor4ub(GL_r,GL_g,GL_b, GL_a);
}
void GL_SetAlpha (v_uint8 a) {
	GL_a=a;
	glColor4ub(GL_r,GL_g,GL_b, GL_a);
}
void GL_SetScale (float sx, float sy) {
	GL_sx=sx; GL_sy=sy;
}
void GL_SetRot (float rot) {
	GL_rot=rot;
}

void GL_SetFont (r_font_t fnt) {
	GL_font=(CGLFont*)fnt;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Sprite Stuff ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
v_result GL_LoadSprite (const char* filename, r_sprite_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CGLSprite *p_spr=new CGLSprite;
	if (p_spr==0)
		return V_FAIL;
	if (p_spr->Load (filename) != V_OK) {
		delete p_spr;
		return V_FAIL;
	}
	*dest=p_spr;
	return V_OK;
}

void GL_UnLoadSprite (r_sprite_t p) {
	if (p==0)
		return;
	CGLSprite* p_spr=(CGLSprite*)p;
	p_spr->Free();
	delete p_spr;
}

void GL_GetSpriteInfo (r_sprite_t p, r_sprite_info_t* dest) {
	if (p==0 || dest==0)
		return;
	CGLSprite* p_spr=(CGLSprite*)p;
	dest->n_frames=p_spr->n_frames;
}

void GL_GetSprFrameInfo (r_sprite_t p, int frame, r_spr_frame_info_t* dest) {
	if (p==0 || dest==0)
		return;
	CGLSprite* p_spr=(CGLSprite*)p;
	if (frame<0 || frame>=p_spr->n_frames)
		return;
	dest->width	=p_spr->p_frames[frame].width;
	dest->height=p_spr->p_frames[frame].height;
	dest->x		=p_spr->p_frames[frame].x;
	dest->y		=p_spr->p_frames[frame].y;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Pic Stuff ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
//	v_result	(*R_LoadPic			)	(const char*, r_pic_t*);
//	v_result	(*R_LoadPicBMP		)	(const char*, r_pic_t*);
//	void		(*R_UnLoadPic		)	(r_pic_t);
//	void		(*R_GetPicInfo		)	(r_pic_t, r_pic_info_t*);
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
v_result GL_LoadPic (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CGLPic *p_pic=new CGLPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->Load (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
v_result GL_LoadPicBMP (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CGLPic *p_pic=new CGLPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadBMP (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}
v_result GL_LoadPicTGA (const char* filename, r_pic_t* dest) {
	if (filename==0 || dest==0)
		return V_FAIL;
	CGLPic *p_pic=new CGLPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadTGA (filename) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}

v_result GL_LoadPicStraight (C117Bitmap* p_bmp, r_pic_t* dest) {
	if (p_bmp==0 || dest==0)
		return V_FAIL;
	CGLPic *p_pic=new CGLPic;
	if (p_pic==0)
		return V_FAIL;
	if (p_pic->LoadStraight (p_bmp) != V_OK) {
		delete p_pic;
		return V_FAIL;
	}
	*dest=p_pic;
	return V_OK;
}

void GL_UnLoadPic (r_pic_t p) {
	if (p==0)
		return;
	CGLPic* p_pic=(CGLPic*)p;
	p_pic->Detach();
	delete p_pic;
}

void GL_GetPicInfo (r_pic_t p, r_pic_info_t* dest) {
	if (p==0 || dest==0)
		return;
	CGLPic* p_pic=(CGLPic*)p;
	dest->width=p_pic->width;
	dest->height=p_pic->height;
}

v_result GL_LoadFont (const char* filename, r_font_t* dest) {
	if (!filename || !dest)
		return V_FAIL;
	CGLFont* p_fnt=new CGLFont;
	if (p_fnt->Load(filename) != V_OK)
		return V_FAIL;
	*dest=p_fnt;
	return V_OK;
}

void GL_UnLoadFont (r_font_t font) {
	if (!font)
		return;
	CGLFont* p_fnt=(CGLFont*)font;
	p_fnt->UnLoad();
	delete p_fnt;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Drawing primitives ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void GL_Clear () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GL_Line (int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
		glVertex2i (x1, y1);
		glVertex2i (x2, y2);
	glEnd();
}
void GL_Rect (int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
		glVertex2i (x1, y1);
		glVertex2i (x2, y1);
		glVertex2i (x2, y1);
		glVertex2i (x2, y2);
		glVertex2i (x2, y2);
		glVertex2i (x1, y2);
		glVertex2i (x1, y2);
		glVertex2i (x1, y1);
	glEnd();
}
void GL_Box (int x1, int y1, int x2, int y2) {
	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();
}
void GL_Tri (int x1, int y1, int x2, int y2, int x3, int y3) {
	glBegin(GL_TRIANGLES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();
	
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** Drawing advanced ***
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
//	void		(*R_DrawBG		)	(r_pic_t, int, int, int, int, int, int);
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------


void GL_DrawPicRect (r_pic_t p, int x1, int y1, int x2, int y2) {
	if (p==0) return;
	CGLPic* p_pic=(CGLPic*)p;
	if (!p_pic->attached) return;
	if (!glIsTexture(p_pic->id)) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, p_pic->id);

	glBegin(GL_QUADS);
		glTexCoord2f (0, 0);	glVertex2i(x1, y1);
		glTexCoord2f (1, 0);	glVertex2i(x2, y1);
		glTexCoord2f (1, 1);	glVertex2i(x2, y2);
		glTexCoord2f (0, 1);	glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void GL_DrawPicRectPlus (r_pic_t p, int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2) {
	if (p==0) return;
	CGLPic* p_pic=(CGLPic*)p;
	if (!p_pic->attached) return;
	if (!glIsTexture(p_pic->id)) return;

	float	ftx1=(float)tx1/(float)p_pic->width,
			fty1=(float)ty1/(float)p_pic->height,
			ftx2=(float)tx2/(float)p_pic->width,
			fty2=(float)ty2/(float)p_pic->height;

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, p_pic->id);

	glBegin(GL_QUADS);
		glTexCoord2f (ftx1, fty1);	
		glVertex2i(x1, y1);
		glTexCoord2f (ftx2, fty1);	
		glVertex2i(x2, y1);
		glTexCoord2f (ftx2, fty2);	
		glVertex2i(x2, y2);
		glTexCoord2f (ftx1, fty2);	
		glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void GL_DrawPic (r_pic_t p, int x, int y) {
	if (p==0) return;
	CGLPic* p_pic=(CGLPic*)p;
	if (!p_pic->attached) return;
	if (!glIsTexture(p_pic->id)) return;

	int x1=0, y1=0;
	int x2=x1+p_pic->width, y2=y1+p_pic->height;
	
	glPushMatrix();

	glLoadIdentity ();
	glTranslatef (x, y, 0);
	glRotatef (GL_rot, 0, 0, 1);
	glScalef (GL_sx, GL_sy, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, p_pic->id);

	glBegin(GL_QUADS);
		glTexCoord2f (0, 0);	glVertex2i(x1, y1);
		glTexCoord2f (1, 0);	glVertex2i(x2, y1);
		glTexCoord2f (1, 1);	glVertex2i(x2, y2);
		glTexCoord2f (0, 1);	glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void GL_DrawSprite (r_sprite_t p, int frame, int x, int y) {
	if (!p) return;
	CGLSprite* p_spr=(CGLSprite*)p;
	if (frame<0 || frame>=p_spr->n_frames)
		return;

	gl_sprframe_t* p_frame=p_spr->p_frames+frame;
	if (!glIsTexture (p_frame->id))
		return;

	int x1=-p_frame->x, y1=-p_frame->y;
	int x2=x1+p_frame->width, y2=y1+p_frame->height;
	
	glPushMatrix();

	glLoadIdentity ();
	glTranslatef ((float)x, (float)y, 0);
	glRotatef (GL_rot, 0, 0, 1);
	glScalef (GL_sx, GL_sy, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, p_frame->id);
	
	glBegin(GL_QUADS);
		glTexCoord2f (0, 0);	glVertex2i(x1, y1);
		glTexCoord2f (1, 0);	glVertex2i(x2, y1);
		glTexCoord2f (1, 1);	glVertex2i(x2, y2);
		glTexCoord2f (0, 1);	glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix ();
}

void GL_Text (const char* str, int x, int y) {
	if (!GL_font)
		return;
	
	glLoadIdentity();
	glTranslatef (x, y, 0);
	glScalef (GL_sx, GL_sy, 1);

	x=y=0;

	int x2, y2;
	for (int i=0; str[i]!=0; i++) {
		x2=x+GL_font->ptr[str[i]]->width;
		y2=y+GL_font->ptr[str[i]]->height;
		GL_DrawPicRect (GL_font->ptr[str[i]], x, y, x2, y2);
		x=x2;
	}
	glLoadIdentity();
}
int GL_GetTextWidth (const char* str) {
	int x,y;
	GL_font->GetStrDim(str, &x, &y);
	x=(float)x * GL_sx;
	return x;
}
int GL_GetTextHeight (const char* str) {
	int x,y;
	GL_font->GetStrDim(str, &x, &y);
	y=(float)y * GL_sy;
	return y;
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
v_result GL_ScreenShot (const char* filename) {
	if (!filename) return V_FAIL;
	if (!strcmp (filename, ""))
		return V_FAIL;

	C117Bitmap bmp;
	bmp.ModeRGBA();
	bmp.width=GL_parms.width;
	bmp.height=GL_parms.height;
	bmp.ptr=MemAlloc (v_uint8, GL_parms.width*GL_parms.height*4);
	
	glReadPixels (0, 0, GL_parms.width, GL_parms.height, GL_RGBA, GL_UNSIGNED_BYTE, bmp.ptr);
	
	if (bmp.Save (filename) != V_OK)
		return V_FAIL;
	bmp.Free();
	return V_OK;	
}

int GL_clip_x1, GL_clip_y1, GL_clip_x2, GL_clip_y2;
void GL_Clip (int x1, int y1, int x2, int y2) {
	GL_clip_x1=x1;
	GL_clip_y1=y1;
	GL_clip_x2=x2;
	GL_clip_y2=y2;
	glScissor(x1,GL_parms.height-y2,x2-x1,(y2-y1));
}

void GL_GetClip (int* p_x1, int* p_y1, int* p_x2, int* p_y2) {
	if (p_x1) *p_x1=GL_clip_x1;
	if (p_y1) *p_y1=GL_clip_y1;
	if (p_x2) *p_x2=GL_clip_x2;
	if (p_y2) *p_y2=GL_clip_y2;
}

void R_LoadAPI (API_renderer_t* p_ri) {
	if (!p_ri) return;
	
	p_ri->R_Init=GL_Init;
	p_ri->R_Close=GL_Close;
	p_ri->R_Begin=GL_Begin;
	p_ri->R_End=GL_End;
	p_ri->R_GetParms=GL_GetParms;

	p_ri->R_SwitchTo3D=GL_SwitchTo3D;
	p_ri->R_Restore2D=GL_Restore2D;

	p_ri->R_SetColor=GL_SetColor;
	p_ri->R_SetAlpha=GL_SetAlpha;
	p_ri->R_SetScale=GL_SetScale;
	p_ri->R_SetRot=GL_SetRot;
	p_ri->R_SetFont=GL_SetFont;

	p_ri->R_LoadPic=GL_LoadPic;
	p_ri->R_LoadPicBMP=GL_LoadPicBMP;
	p_ri->R_LoadPicTGA=GL_LoadPicTGA;
	p_ri->R_LoadPicStraight=GL_LoadPicStraight;
	p_ri->R_UnLoadPic=GL_UnLoadPic;
	p_ri->R_GetPicInfo=GL_GetPicInfo;

	p_ri->R_LoadFont=GL_LoadFont;
	p_ri->R_UnLoadFont=GL_UnLoadFont;

	p_ri->R_LoadSprite=GL_LoadSprite;
	p_ri->R_UnLoadSprite=GL_UnLoadSprite;
	p_ri->R_GetSpriteInfo=GL_GetSpriteInfo;
	p_ri->R_GetSprFrameInfo=GL_GetSprFrameInfo;

	p_ri->R_DrawSprite=GL_DrawSprite;
	p_ri->R_DrawPic=GL_DrawPic;
	p_ri->R_DrawPicRect=GL_DrawPicRect;
	p_ri->R_DrawPicRectPlus=GL_DrawPicRectPlus;
	p_ri->R_Text=GL_Text;

	p_ri->R_GetTextWidth=GL_GetTextWidth;
	p_ri->R_GetTextHeight=GL_GetTextHeight;

	p_ri->R_Clear=GL_Clear;
	p_ri->R_Line=GL_Line;
	p_ri->R_Rect=GL_Rect;
	p_ri->R_Box=GL_Box;
	p_ri->R_Tri=GL_Tri;

	p_ri->R_Face3D=GL_Face3D;
	p_ri->R_Line3D=GL_Line3D;

	p_ri->R_MatrixReset=GL_MatrixReset;
	p_ri->R_MatrixMove=GL_MatrixMove;
	p_ri->R_MatrixScale=GL_MatrixScale;
	p_ri->R_MatrixRotateX=GL_MatrixRotateX;
	p_ri->R_MatrixRotateY=GL_MatrixRotateY;
	p_ri->R_MatrixRotateZ=GL_MatrixRotateZ;

	p_ri->R_ScreenShot=GL_ScreenShot;
	p_ri->R_Clip=GL_Clip;
	p_ri->R_GetClip=GL_GetClip;
}

