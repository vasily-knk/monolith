#include "../common/middefs.h"
#include "../common/renderer.h"
#include "main.h"
#include "keys.h"
#include "ctrl.h"
#include "my_wnd.h"
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// API LOADING
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void (*R_LoadAPI)(API_renderer_t*);

HINSTANCE hR_DLL;
bool R_LoadDLL (const char* filename ) {
	char str[MAX_LOADSTRING];
	sprintf (str, "%s", filename);
	if ((hR_DLL=LoadLibrary (str)) == NULL) {
		MessageBox (NULL, str, "Error", MB_ICONERROR | MB_OK);
		return false;
	}
	if ( (R_LoadAPI= (void(__cdecl*)(API_renderer_t*) )
			GetProcAddress (hR_DLL, "R_LoadAPI")) == NULL) {
		
		MessageBox (NULL, "Renderer API","Error", MB_ICONERROR | MB_OK);
		FreeLibrary (hR_DLL);
		return false;
	}
	return true;
}
void R_UnLoadDLL () {
	FreeLibrary (hR_DLL);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// Main code
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

int MODE_WIDTH, MODE_HEIGHT, MODE_BPP, MODE_FULLSCREEN;
char dll_name[MAX_LOADSTRING];

v_result LoadVidParms () {
	FILE* fff=fopen ("vid", "rb");
	if (!fff)
		return V_FAIL;

	fread (&MODE_WIDTH, sizeof(int), 1, fff);
	fread (&MODE_HEIGHT, sizeof(int), 1, fff);
	fread (&MODE_BPP, sizeof(int), 1, fff);
	fread (&MODE_FULLSCREEN, sizeof(int), 1, fff);

	int i=0, c=0;
	for (; c!=EOF && i<MAX_LOADSTRING-1; i++ ) {
		c=getc(fff);
		if (c==EOF)
			dll_name[i]=0;
		else
			dll_name[i]=c;
	}

	fclose (fff);
	return V_OK;
}

API_renderer_t ri;
r_font_t r_font;

CMainWindow main_wnd;
CWindow* p_wnd=0;

HWND hMainWnd;

v_result MainInit () {
	MemInit();

	if (LoadVidParms()!=V_OK)
		return V_FAIL;
	if (!R_LoadDLL (dll_name))
		return V_FAIL;
	srand ((unsigned)time( NULL ) );
	r_parms_t r_parms ={MODE_WIDTH, MODE_HEIGHT, MODE_BPP, MODE_FULLSCREEN!=0, (WNDPROC)WndProc};
	R_LoadAPI (&ri);
	if (ri.R_Init (&r_parms) != V_OK)
		return V_FAIL;

	ri.R_GetParms(&r_parms);
	hMainWnd=r_parms.hWnd;


	if (ri.R_LoadFont("fonts/console.fff", &r_font)!= V_OK)
		return V_FAIL;

	ri.R_SetFont(r_font);

	main_wnd.Init();
	p_wnd=&main_wnd;

	return V_OK;
}

v_result MainClose () {
	main_wnd.Close();
	ri.R_UnLoadFont(r_font);
	ri.R_Close();
	R_UnLoadDLL ();

	MemClose();
	return V_OK;
}

v_result MainCycle () {
	if (p_wnd)
		p_wnd->FirstEvt (WM_TIMER, 0,0);

	ri.R_Begin();
	ri.R_Clip (0, 0, MODE_WIDTH, MODE_HEIGHT);
	ri.R_Clear ();

	if (p_wnd)
		p_wnd->Draw();

	ri.R_End ();
	return V_OK;
}

void KeyEvent (int k, bool down) {
	if (!p_wnd) return;
	p_wnd->KeyEvent(k, down);
}
void FirstEvt (int evt, v_uint16 wParam, v_uint32 lParam) {
	if (!p_wnd) return;
	p_wnd->FirstEvt(evt, wParam, lParam);

}
