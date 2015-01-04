#ifndef INC_117_RENDERER_H
#define INC_117_RENDERER_H

#include "../common/bitmap.h"

struct r_parms_t {
	int width, height, bpp;
	bool fullscreen;
	WNDPROC wndproc;
	HWND hWnd;
};

// Pic
typedef void* r_pic_t;
typedef struct {
	int width, height;
} r_pic_info_t;

typedef void* r_font_t;
// Sprite
typedef void* r_sprite_t;
typedef struct {
	int n_frames;
} r_sprite_info_t;
typedef struct {
	int width, height;
	int x, y;
} r_spr_frame_info_t;

typedef struct {
	v_result	(*R_GetParms	)	(r_parms_t*);
	v_result	(*R_Init		)	(r_parms_t*);
	v_result	(*R_Close		)	();
	v_result	(*R_Begin		)	();
	v_result	(*R_End			)	();

	void		(*R_SwitchTo3D	)	(int, int, int, int, float);
	void		(*R_Restore2D	)	();
	
	void		(*R_SetColor	)	(v_uint8, v_uint8, v_uint8);
	void		(*R_SetAlpha	)	(v_uint8);
	void		(*R_SetScale	)	(float, float);
	void		(*R_SetRot		)	(float);
	void		(*R_SetFont		)	(r_font_t);

	v_result	(*R_LoadPic			)	(const char*, r_pic_t*);
	v_result	(*R_LoadPicBMP		)	(const char*, r_pic_t*);
	v_result	(*R_LoadPicTGA		)	(const char*, r_pic_t*);
	v_result	(*R_LoadPicStraight	)	(C117Bitmap*, r_pic_t*);
	void		(*R_UnLoadPic		)	(r_pic_t);
	void		(*R_GetPicInfo		)	(r_pic_t, r_pic_info_t*);

	v_result	(*R_LoadFont		)	(const char*, r_font_t*);
	void		(*R_UnLoadFont		)	(r_font_t);

	v_result	(*R_LoadSprite		)	(const char*, r_sprite_t*);
	void		(*R_UnLoadSprite	)	(r_sprite_t);
	void		(*R_GetSpriteInfo	)	(r_sprite_t, r_sprite_info_t*);
	void		(*R_GetSprFrameInfo	)	(r_sprite_t, int, r_spr_frame_info_t*);

	void		(*R_DrawSprite	)	(r_sprite_t, int, int, int);
	void		(*R_DrawPicRect	)	(r_pic_t, int, int, int, int);
	void		(*R_DrawPicRectPlus	)	(r_pic_t, int, int, int, int, int, int, int, int);
	void		(*R_DrawPic		)	(r_pic_t, int, int);
	//void		(*R_DrawBG		)	(r_pic_t, int, int, int, int, int, int);
	//void		(*R_DrawPicPlus	)
	void		(*R_Text		)	(const char*, int, int);

	int			(*R_GetTextWidth)	(const char*);
	int			(*R_GetTextHeight)	(const char*);

	void		(*R_Clear		)	();
	void		(*R_Line		)	(int, int, int, int);
	void		(*R_Rect		)	(int, int, int, int);
	void		(*R_Box			)	(int, int, int, int);
	void		(*R_Tri			)	(int, int, int, int, int, int);

	void		(*R_Face3D		)	(float, float, float, float, float, float, float, float, float);
	void		(*R_Line3D		)	(float, float, float, float, float, float);

	void		(*R_MatrixReset )	();
	void		(*R_MatrixMove	)	(float x, float y, float z);
	void		(*R_MatrixScale	)	(float x, float y, float z);
	void		(*R_MatrixRotateX)	(float a);
	void		(*R_MatrixRotateY)	(float a);
	void		(*R_MatrixRotateZ)	(float a);


	void		(*R_Clip		)	(int, int, int, int);
	void		(*R_GetClip		)	(int*, int*, int*, int*);

	v_result	(*R_ScreenShot	)	(const char*);
} API_renderer_t;

#endif