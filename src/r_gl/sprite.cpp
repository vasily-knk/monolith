#include "../common/middefs.h"
#include "r_gl.h"
#include "sprite.h"

CGLSprite::CGLSprite () {
	n_frames=0;
	p_frames=0;
}
CGLSprite::~CGLSprite () {
	Free ();
}

void CGLSprite::Free () {
	if (n_frames) {
		for (int i=0; i<n_frames; i++) {
			if (glIsTexture(p_frames[i].id))
				glDeleteTextures(1, &(p_frames[i].id));
		}
		n_frames=0;
		MemFree (p_frames);
		p_frames=0;
	}
}
v_result CGLSprite::Alloc () {
	if (p_frames)
		MemFree(p_frames);
	if (!(p_frames=MemAlloc (gl_sprframe_t, n_frames)))
		return V_FAIL;
	return V_OK;
}
v_result CGLSprite::Load (const char* filename) {
	if (!filename)
		return V_FAIL;
	FILE* src_file=fopen (filename, "rb");
	if (!src_file)
		return V_FAIL;
	
	info_preheader_t info_preheader;
	fread (&info_preheader, sizeof (info_preheader_t), 1, src_file);
	if (info_preheader.magic!=SPRITE_MAGIC)
		return V_FAIL;

	if (info_preheader.version==0) {
		info_sprite_t info_sprite;
		fread (&info_sprite, sizeof(info_sprite_t), 1, src_file);

		n_frames=info_sprite.frames;
		if (Alloc()!=V_OK) {
			fclose (src_file);
			return V_FAIL;
		}

		fseek (src_file, info_sprite.dataoffset, SEEK_SET);
		for (int i=0; i<n_frames; i++) {
			if (ReadFrame (src_file, p_frames+i)!=V_OK) {
				Free();
				return V_FAIL;
			}
		}

	} else {
		Free();
		fclose (src_file);
		return V_FAIL;
	}
	fclose (src_file);
	return V_OK;
}

v_result CGLSprite::ReadFrame (FILE* src_file, gl_sprframe_t* ptr) {
	info_sprframe_t info_sprframe;
	fread (&info_sprframe, sizeof(info_sprframe_t), 1, src_file);

	ptr->width=		info_sprframe.width;
	ptr->height=	info_sprframe.height;
	ptr->x=			info_sprframe.x;
	ptr->y=			info_sprframe.y;

	v_uint32* data=new v_uint32[ptr->width*ptr->height];
	if (!data) return V_FAIL;

	for (int i=0; i<ptr->width*ptr->height; i++) {
		fread (data+i, 4, 1, src_file);
	}

	int w, h;
	for (w=1; w<ptr->width; w<<=1);
	for (h=1; h<ptr->height; h<<=1);
	
	v_uint32* data1=new v_uint32[w*h];
	gluScaleImage (GL_RGBA, ptr->width, ptr->height, GL_UNSIGNED_BYTE, data, w, h, GL_UNSIGNED_BYTE, data1);
	
	glGenTextures(1, &(ptr->id));
	
	glBindTexture(GL_TEXTURE_2D, ptr->id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_filter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);

	MemFree(data1);
	MemFree(data);
	return V_OK;
}