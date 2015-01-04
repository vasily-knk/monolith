#include "../common/middefs.h"
#include "my_ddraw.h"
#include <gx.h>

HINSTANCE hInstance;

int dd_width, dd_height, dd_bpp;
v_uint32 dd_rbits, dd_gbits, dd_bbits;
v_uint32 dd_roffs, dd_goffs, dd_boffs;
v_uint32 dd_lpitch;
lame_color *dd_ptr;
WNDPROC wndproc;


ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = wndproc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = 0;
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

v_result MyDDrawInit (int w, int h, int bpp, bool fs, WNDPROC proc) {
	HWND hWnd;

	TCHAR szTitle[MAX_LOADSTRING]=L"DDD";		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING]=L"R_PPC";	// main window class name

    //g_hInst = hInstance; // Store instance handle in our global variable

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    /*LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_MOB000, szWindowClass, MAX_LOADSTRING);*/

    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return V_FAIL;
    } 
	wndproc=proc;
    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return V_FAIL;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return V_FAIL;
    }


    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

	GXDisplayProperties d_props;
	d_props=GXGetDisplayProperties ();

	SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);
	MoveWindow(hWnd, 0, 0, d_props.cxWidth, d_props.cyHeight, FALSE);

	if (!GXOpenDisplay (hWnd, GX_FULLSCREEN))
		return V_FAIL;

	dd_width=d_props.cxWidth;
	dd_height=d_props.cyHeight;
	dd_bpp=d_props.cBPP;
	dd_lpitch=d_props.cbyPitch/2;

	dd_gbits=6;
	dd_rbits=dd_bbits=5;
	dd_roffs=11;
	dd_goffs=5;
	dd_boffs=0;

	dd_ptr=new lame_color[dd_lpitch*dd_height*LAME_SIZE];
	if (!dd_ptr)
		return V_FAIL;

	return V_OK;
}
void MyDDrawClose () {
	delete[] dd_ptr;
	GXCloseDisplay();
}
void* real_ptr;
v_result MyDDrawBegin () {
	real_ptr=GXBeginDraw();
	if (!real_ptr)
		return V_FAIL;
	return V_OK;
}
v_result MyDDrawEnd () {
	memcpy (real_ptr, dd_ptr, dd_lpitch*dd_height*LAME_SIZE);
	GXEndDraw();
	return V_OK;
}

void MyDDrawHide () {
	GXSuspend ();
}

void MyDDrawRestore () {
	GXResume ();
}