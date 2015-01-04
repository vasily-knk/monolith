#include "../common/middefs.h"
#include "keys.h"
#include "main.h"

/*bool time_to_die=false;
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
}*/




// ------------------------- * key mapping * -------------------------
v_uint8 scantokey[128]=
{
	0, K_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', K_BACKSPACE, // 0..14
	K_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', K_ENTER, // 14..28
	K_CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 29..41
	K_SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', K_SHIFT, // 42..54
	K_KP_STAR, K_ALT, K_SPACE, K_CAPSLOCK, // 55..58
	K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLLLOCK, // 59..70
	K_HOME, K_UPARROW, K_PGUP, K_KP_MINUS, // 71..74
	K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, // 75..78
	K_END, K_DOWNARROW, K_PGDN, K_INS, K_DEL, // 79..83
	0, 0, 0, K_F11, K_F12, 0, 0, K_WIN, K_WIN, K_MENU, 0, 0, // 84..95
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}; 

/*
** In_MapKey
**
** maps from windows keynums
*/
bool shift_down=false;
int		keyshift[256];		// key to map to if shift held down in console

void Win32Keys_Init () {
	for(int n=0; n<256; n++)
		keyshift[n]=n;
	for(n='a'; n<='z'; n++)
		keyshift[n]=n-'a'+'A';
	keyshift['1']='!';
	keyshift['2']='@';
	keyshift['3']='#';
	keyshift['4']='$';
	keyshift['5']='%';
	keyshift['6']='^';
	keyshift['7']='&';
	keyshift['8']='*';
	keyshift['9']='(';
	keyshift['0']=')';
	keyshift['-']='_';
	keyshift['=']='+';
	keyshift[',']='<';
	keyshift['.']='>';
	keyshift['/']='?';
	keyshift[';']=':';
	keyshift['\'']='"';
	keyshift['[']='{';
	keyshift[']']='}';
	keyshift['`']='~';
	keyshift['\\']='|';
}
void Win32Keys_Update (int key, bool down) {
	if (key==K_SHIFT) {
		shift_down=down;
	}
}
int In_MapKey(int key)
{
	int result;
	bool is_extended;

	result=(key>>16)&255;
	if(result>127) return 0;
	is_extended=key&(1<<24)?true:false;

	result=scantokey[result];

	if(!is_extended) switch(result)
	{
	case K_HOME:
		return K_KP_HOME;
	case K_UPARROW:
		return K_KP_UPARROW;
	case K_PGUP:
		return K_KP_PGUP;
	case K_LEFTARROW:
		return K_KP_LEFTARROW;
	case K_RIGHTARROW:
		return K_KP_RIGHTARROW;
	case K_END:
		return K_KP_END;
	case K_DOWNARROW:
		return K_KP_DOWNARROW;
	case K_PGDN:
		return K_KP_PGDN;
	case K_INS:
		return K_KP_INS;
	case K_DEL:
		return K_KP_DEL;
	} else switch(result)
	{
	case K_ENTER:
		return K_KP_ENTER;
	case '/':
		return K_KP_SLASH;
	case K_PAUSE:
		return K_NUMLOCK;
	}

	if (shift_down)
		result=keyshift[result];

	return result;
}

/*gametime_t GetTime () {
	return gametime_t(GetTickCount())/1000.0;
}*/


bool time_to_die=false;
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	FirstEvt(uMsg, wParam, lParam);
	
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
	case WM_KEYDOWN:
		Win32Keys_Update (In_MapKey(lParam), true);
		KeyEvent (In_MapKey(lParam), true);
		break;
	case WM_KEYUP: 
		Win32Keys_Update (In_MapKey(lParam), false);
		KeyEvent (In_MapKey(lParam), false);
		break;
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

	Win32Keys_Init();
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

