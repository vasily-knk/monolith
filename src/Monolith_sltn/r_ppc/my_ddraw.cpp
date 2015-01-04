#include "stdafx.h"
/*#include "../common/middefs.h"
#include <ddraw.h>
#include "my_ddraw.h"

#define WND_CLASSNAME "MyDDraw"

HINSTANCE hInstance;

LPDIRECTDRAW			lpDD=NULL;
LPDIRECTDRAWSURFACE		lpDDSPrimary=NULL, lpDDSBack=NULL;
LPDIRECTDRAWCLIPPER		lpClip=NULL;
DDSURFACEDESC ddsd;
HWND hWnd;

int dd_width, dd_height, dd_bpp;
bool dd_fs;

v_uint32 dd_rbits, dd_gbits, dd_bbits;
v_uint32 dd_roffs, dd_goffs, dd_boffs;

v_uint32 dd_lpitch;
lame_color *dd_ptr;


// --------------------------------------------------------------------------------
// Error
// --------------------------------------------------------------------------------
void MyDDrawError (const char* str) {
	MessageBox (0, str, "DDraw Error", MB_OK | MB_ICONEXCLAMATION);
}

// --------------------------------------------------------------------------------
// Lock
// --------------------------------------------------------------------------------

v_result MyDDrawBegin () {
	ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (lpDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) {
		MyDDrawError ("Can't lock!!!");
		return V_FAIL;
	}
	dd_ptr=(lame_color*)ddsd.lpSurface;
	dd_lpitch=ddsd.lPitch / LAME_SIZE;

	return V_OK;
}

// --------------------------------------------------------------------------------
// Unlock
// --------------------------------------------------------------------------------
v_result MyDDrawEnd () {
	if (lpDDSBack->Unlock(NULL) != DD_OK) {
		MyDDrawError ("Can't unlock!!!");
		return V_FAIL;
	}

	if (!dd_fs) {
		RECT rcRectDest;
		POINT p;
		rcRectDest.left=0;
		rcRectDest.top=0;
		rcRectDest.right=ddsd.dwWidth;
		rcRectDest.bottom=ddsd.dwHeight;
		
		p.x = 0; p.y = 0;
		ClientToScreen(hWnd, &p);
		OffsetRect(&rcRectDest, p.x, p.y);
		
		if (lpDDSPrimary->Blt(&rcRectDest, lpDDSBack, NULL, DDBLT_WAIT, NULL) != DD_OK) {
			MyDDrawError ("Can't blit!!!");
			return V_FAIL;
		} 
	} else {
		if (lpDDSPrimary->Flip( NULL, DDFLIP_WAIT) != DD_OK) {
			MyDDrawError ("Can't flip!!!");
			return V_FAIL;
		}
	}
	return V_OK;
}

// --------------------------------------------------------------------------------
// Init
// --------------------------------------------------------------------------------
v_result MyDDrawGetPF () {
	DDPIXELFORMAT pf;
	ZeroMemory (&pf, sizeof(pf));
	pf.dwSize=sizeof(pf);
	pf.dwFlags=DDPF_RGB;
	if (lpDDSBack->GetPixelFormat(&pf) != DD_OK) {
		MyDDrawError ("Can't read pixel format!");
		return V_FAIL;
	}

	v_uint32 l;

	dd_rbits=0; dd_roffs=0;
	dd_gbits=0; dd_goffs=0;
	dd_bbits=0; dd_boffs=0;
	for (l=pf.dwRBitMask; !(l&1); l>>=1) dd_roffs++;
	for (; l; l>>=1) dd_rbits++;
	for (l=pf.dwGBitMask; !(l&1); l>>=1) dd_goffs++;
	for (; l; l>>=1) dd_gbits++;
	for (l=pf.dwBBitMask; !(l&1); l>>=1) dd_boffs++;
	for (; l; l>>=1) dd_bbits++;


	return V_OK;
}
// --------------------------------------------------------------------------------
// Init
// --------------------------------------------------------------------------------

v_result MyDDrawCreateSurface ();
v_result MyDDrawCreateSurfaceFS ();
HWND MyDDrawCreateWindow (char* title, int w, int h, bool fullscreenflag, WNDPROC Proc);
void MyDDrawKillWindow ();

v_result MyDDrawInit (int w, int h, int bpp, bool fs, WNDPROC proc) {
	if (MyDDrawCreateWindow ("DDraw window", w, h, fs, proc) == 0) {
		MyDDrawError ("Can't create window!");
		return V_FAIL;
	}
	if (DirectDrawCreate (NULL, &lpDD,  NULL) != DD_OK) {
		MyDDrawError ("DirectDrawCreate failed!");
		return V_FAIL;
	}
	if (!fs) {
		if (lpDD->SetCooperativeLevel (hWnd, DDSCL_NORMAL | DDSCL_ALLOWREBOOT) != DD_OK) {
			MyDDrawError ("SetCooperativeLevel failed!");
			lpDD->Release();			lpDD=NULL;
			return V_FAIL;
		}	
		if (lpDD->CreateClipper(0, &lpClip, NULL) != DD_OK) {
			MyDDrawError ("CreateClipper failed!");
			lpDD->Release();			lpDD=NULL;
			return V_FAIL;
		}
		if (lpClip->SetHWnd(0, hWnd) != DD_OK) {
			MyDDrawError ("lpClip->SetHWnd failed!");
			lpDD->Release();			lpDD=NULL;
			lpClip->Release();			lpClip=NULL;
			return V_FAIL;
		}
	} else {
		if (lpDD->SetCooperativeLevel (hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK) {
			MyDDrawError ("SetCooperativeLevel failed!");
			lpDD->Release();			lpDD=NULL;
			return V_FAIL;
		}
		if (lpDD->SetDisplayMode (w, h, bpp) != DD_OK) {
			MyDDrawError ("SetDisplayMode failed!");
			lpDD->Release();			lpDD=NULL;
			return V_FAIL;
		}	
	}
	dd_width=w;
	dd_height=h;
	dd_bpp=bpp;
	dd_fs=fs;
	if (!fs) {
		if (MyDDrawCreateSurface() != V_OK) {
			return V_FAIL;
		}
	} else {
		if (MyDDrawCreateSurfaceFS() != V_OK) {
			return V_FAIL;
		}
	}
	if (MyDDrawGetPF() != V_OK) {
		return V_FAIL;
	}			

	return V_OK;
}

// --------------------------------------------------------------------------------
// Create Surface FullScreen
// --------------------------------------------------------------------------------
v_result MyDDrawCreateSurfaceFS () {
    DDSURFACEDESC ddsd;
	DDSCAPS ddscaps;

    // Create the primary surface with 1 back buffer
    memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof(ddsd );

    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;

    if (lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK) {
		MyDDrawError ("Can't create primary surface!");
        lpDD->Release();
        return V_FAIL;
    }

    // Get the pointer to the back buffer
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    if (lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack) != DD_OK) {
		MyDDrawError ("Can't get attached surface!");
        lpDDSPrimary->Release();
        lpDD->Release();
        return V_FAIL;
    }
    return V_OK;
}

// --------------------------------------------------------------------------------
// Create Surface 
// --------------------------------------------------------------------------------
v_result MyDDrawCreateSurface () {
	DDSURFACEDESC ddsd;
	ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL)) {
		MyDDrawError ("Failed to create primary surface!");
		lpClip->Release();			lpClip=NULL;
		lpDD->Release();			lpDD=NULL;
		return V_FAIL;
	}
	if (lpDDSPrimary->SetClipper(lpClip) != DD_OK) {
		MyDDrawError ("Failed to attach clipper!");
		lpDDSPrimary->Release();	lpDDSPrimary=NULL;
		lpClip->Release();			lpClip=NULL;
		lpDD->Release();			lpDD=NULL;
		return V_FAIL;
	}

	ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = dd_width;
    ddsd.dwHeight = dd_height;
	if (lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL)) {
		MyDDrawError ("Failed to create back surface!");
		lpDDSPrimary->Release();	lpDDSPrimary=NULL;
		lpClip->Release();			lpClip=NULL;
		lpDD->Release();			lpDD=NULL;
		return V_FAIL;
	}
	return V_OK;
}

// --------------------------------------------------------------------------------
// Finish
// --------------------------------------------------------------------------------
void MyDDrawClose () {
	if (dd_fs)  {lpDD->RestoreDisplayMode();}
	MyDDrawKillWindow();
	lpDDSBack->Release();		lpDDSBack=NULL;
	lpDDSPrimary->Release();	lpDDSPrimary=NULL;
	if (lpClip) {lpClip->Release();			lpClip=NULL;}
	lpDD->Release();			lpDD=NULL;
}


// --------------------------------------------------------------------------------
// Kill Window
// --------------------------------------------------------------------------------
void MyDDrawKillWindow () {
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(WND_CLASSNAME,hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

// --------------------------------------------------------------------------------
// Create Window
// --------------------------------------------------------------------------------
HWND MyDDrawCreateWindow (char* title, int w, int h, bool fullscreenflag, WNDPROC Proc) {
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)w;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)h;		// Set Bottom Value To Requested Height

//	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= Proc;							// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= WND_CLASSNAME;								// Set The Class Name

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	if (fullscreenflag)	{
		dwExStyle=WS_EX_APPWINDOW;
		dwStyle=WS_POPUP;
	} else {
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle=WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								WND_CLASSNAME,							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		MyDDrawKillWindow();								// Reset The Display
		return 0;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	dd_width=w; dd_height=h;
	return hWnd;
}*/