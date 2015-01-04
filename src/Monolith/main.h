#ifndef INC_117_MAIN_H
#define INC_117_MAIN_H

v_result MainInit ();
v_result MainClose ();
v_result MainCycle ();

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void KeyEvent (int, bool);
void FirstEvt (int evt, v_uint16 wParam, v_uint32 lParam);
extern int MODE_WIDTH ;
extern int MODE_HEIGHT ;
extern int MODE_BPP ;
extern int MODE_FULLSCREEN ;
extern HWND hMainWnd;

extern bool time_to_die;


#endif