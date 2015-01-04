#ifndef INC_117_R_GL_H
#define INC_117_R_GL_H

#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>
#include "../common/renderer.h"

#define DEF_GL_FILTER GL_LINEAR

typedef unsigned int gl_tex_t;
extern int GL_filter;
void R_LoadAPI (API_renderer_t*);

#endif