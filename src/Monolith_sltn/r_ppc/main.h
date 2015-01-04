#ifndef INC_117_MAIN_H
#define INC_117_MAIN_H

v_result MainInit ();
v_result MainClose ();
v_result MainCycle ();

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif