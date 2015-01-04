#ifndef INC_117_SPRITE_H
#define INC_117_SPRITE_H

#include "r_gl.h"

// --------------------------------------------------------------------------------
// Sprite
// --------------------------------------------------------------------------------
#define SPRITE_MAGIC 0x52505356

#pragma pack (1)
	typedef struct {
		v_uint32 frames;
		v_uint32 dataoffset;
	} info_sprite_t;
	typedef struct {
		v_uint32 width, height, x, y;
		char name[16];
	} info_sprframe_t;
#pragma pack ()

struct gl_sprframe_t {
	int width, height, x, y;
	gl_tex_t id;
};

class CGLSprite {
public:
	CGLSprite ();
	~CGLSprite ();
public:
	v_result Load (const char*);
	v_result Alloc ();
	void Free ();
	v_result ReadFrame (FILE*, gl_sprframe_t*);
public:
	int n_frames;
	gl_sprframe_t* p_frames;
};

#endif