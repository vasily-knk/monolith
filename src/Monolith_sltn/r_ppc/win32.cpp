#include "../common/middefs.h"
#include "main.h"

bool time_to_die=false;
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_ACTIVATE: {
		if (!HIWORD(wParam))
			active=TRUE;
		else
			active=FALSE;
		break;
					  }
	case WM_SYSCOMMAND:	{
		switch (wParam)	{
		case SC_SCREENSAVE:	
		case SC_MONITORPOWER:
			return 0;
		}
		break;
						}
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	if (MainInit () != V_OK)
		return 0;
	
	while(!done && !time_to_die) {
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if (msg.message==WM_QUIT) {
				done=TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			if (active) {
				MainCycle();
			}
		}
	}
	MainClose ();

	return (msg.wParam);							// Exit The Program
}

