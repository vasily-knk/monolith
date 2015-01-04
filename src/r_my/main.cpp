#include "../common/middefs.h"
#include "../common/renderer.h"
#include "main.h"
#include "mysoft.h"
#include <math.h>

API_renderer_t ri;

v_result MainInit () {
	R_LoadAPI(&ri);
	r_parms_t r_parms={640, 480, 16, false, WndProc, 0};
	if (ri.R_Init(&r_parms) != V_OK)
		return V_FAIL;
	return V_OK;
}

v_result MainClose () {
	ri.R_Close();
	return V_OK;
}

v_result MainCycle () {
	if (ri.R_Begin()!=V_OK)
		return V_FAIL;

	ri.R_Clear();
	ri.R_SetColor (255, 0, 0);
	for (float f=0; f<=6.28f; f+=0.05f)
		ri.R_Line(100, 100, 100+100.0*cos(f), 100+100.0*sin(f));
	
	if (ri.R_End()!=V_OK)
		return V_FAIL;
	return V_OK;
}

