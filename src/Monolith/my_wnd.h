#ifndef INC_117_MY_WND_H
#define INC_117_MY_WND_H

#include "ctrl.h"
#include "ctrl_rubik.h"

class CMainWindow : public CWindow {
public:
	CMainWindow ();
	void Init ();
	void Close ();
	void Draw ();
	void Evt (CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam);
	void HideMenu ();
	void ShowMenu ();
private:
	CRubikCtrl* rubik_ctrl;
	CBtn* rot_left, *rot_right, *mix_btn, *save_btn, *load_btn, *solve_btn, *quit_btn;
	CRadioGroup* plane_group;
	CRadio* plane_radios[6];
	bool savestate;
	CMyEdit *filename_edit;
	r_pic_t pic;
	bool solvestate;
};


#endif