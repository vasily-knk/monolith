#include "../common/middefs.h"
#include "main.h"
#include "my_wnd.h"

#define RUBIK_WIDTH 512

CMainWindow::CMainWindow() {
	CWindow::CWindow();
	solvestate=false;
}
void CMainWindow::Close() {
}
void CMainWindow::Init() {
	rubik_ctrl=new CRubikCtrl;
	rubik_ctrl->Resize((MODE_WIDTH-RUBIK_WIDTH)/2, 24, RUBIK_WIDTH, RUBIK_WIDTH);
	AddControl(rubik_ctrl);
	
	rot_left=new CBtn ("cntr-CW");
	rot_left->Resize((MODE_WIDTH-RUBIK_WIDTH)/2, RUBIK_WIDTH+32, 60, 24);
	AddControl(rot_left);
	
	rot_right=new CBtn ("CW");
	rot_right->Resize(MODE_WIDTH/2+4, RUBIK_WIDTH+32, 60, 24);
	AddControl(rot_right);
	
	mix_btn=new CBtn ("Mix!");
	mix_btn->Resize (MODE_WIDTH/2-24, RUBIK_WIDTH+128, 48, 24);
	AddControl(mix_btn);

	save_btn=new CBtn ("Save");
	save_btn->Resize(0, 0, 0, 0);
	AddControl(save_btn);

	load_btn=new CBtn ("Load");
	load_btn->Resize(0, 0, 0, 0);
	AddControl(load_btn);

	filename_edit=new CMyEdit ("");
	filename_edit->Resize ((MODE_WIDTH-RUBIK_WIDTH)/2, RUBIK_WIDTH+160, 256, 24);
	AddControl(filename_edit);
	filename_edit->SetVisible(false);

	solve_btn=new CBtn ("Solve!");
	solve_btn->Resize((MODE_WIDTH+RUBIK_WIDTH)/2+8, 24, 40, 24);
	AddControl(solve_btn);

	quit_btn=new CBtn ("Quit");
	quit_btn->Resize(MODE_WIDTH-72, MODE_HEIGHT-32, 64, 24);
	AddControl(quit_btn);

	plane_group=new CRadioGroup ();
	AddControl(plane_group);
	for (int i=0; i<6; i++) {
		plane_radios[i] = new CRadio ("", plane_group, i);
		plane_radios[i]->Resize(8, 8+ 32*i, 40, 24);
		plane_radios[i]->SetColor (planecolors[i*3+0]*255, planecolors[i*3+1]*255, planecolors[i*3+2]*255);
		AddControl(plane_radios[i]);
	}
	plane_radios[0]->Check();
}

void CMainWindow::HideMenu() {
	/*rot_left->SetVisible(false);
	rot_right->SetVisible(false);
	mix_btn->SetVisible(false);
	save_btn->SetVisible(false);
	load_btn->SetVisible(false);

	for (int i=0; i<6; i++)
		plane_radios[i]->SetVisible(false);*/
}
void CMainWindow::ShowMenu() {
	rot_left->SetVisible(true);
	rot_right->SetVisible(true);
	mix_btn->SetVisible(true);
	save_btn->SetVisible(true);
	load_btn->SetVisible(true);
	for (int i=0; i<6; i++)
		plane_radios[i]->SetVisible(true);
}

void CMainWindow::Draw() {
	CWindow::Draw();
}

void CMainWindow::Evt(CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam) {
	char str[100];

			
	CWindow::Evt(p_ctrl, evt, wParam, lParam);
	if (p_ctrl==plane_group) {
		rubik_ctrl->SetPlaneToRot(lParam);
	} else if (evt==WM_LBUTTONDBLCLK) {
		if (p_ctrl==quit_btn) {
			time_to_die=true;
		} else if (p_ctrl==rot_left) {
			rubik_ctrl->ResetSolution();
			rubik_ctrl->Rotate(plane_group->GetValue(), false);
		} else if (p_ctrl==rot_right) {
			rubik_ctrl->ResetSolution();
			rubik_ctrl->Rotate(plane_group->GetValue(), true);
		} else if (p_ctrl==mix_btn) {
			rubik_ctrl->Mix();
		} else if (p_ctrl==save_btn) {
			savestate=true;
			filename_edit->SetVisible(true);
			Evt (filename_edit, WM_SETFOCUS,0,0);
		} else if (p_ctrl==load_btn) {
			savestate=false;
			filename_edit->SetVisible(true);
			Evt (filename_edit, WM_SETFOCUS,0,0);
		} else if (p_ctrl==solve_btn) {
			if (!solvestate) {
				rubik_ctrl->Solve();
				HideMenu ();
				solvestate=true;
			} else {
				rubik_ctrl->Solve();
				solvestate=false;
			}
		}
	} else if (evt==WM_SETFOCUS) {
		if (p_ctrl==filename_edit) 
			filename_edit->SetText ("");
	} else if (evt==WM_KILLFOCUS) {
		if (p_ctrl==filename_edit) {
			if (wParam) {
				v_result res;
				filename_edit->GetText(str);
				if (savestate) {
					res=rubik_ctrl->Save(str);
					if (res!=V_OK)
						MessageBox(hMainWnd, "Can't save!", "Monolith", MB_OK | MB_ICONSTOP);
				} else {
					res=rubik_ctrl->Load(str);
					if (res!=V_OK)
						MessageBox(hMainWnd, "Can't load!", "Monolith", MB_OK | MB_ICONSTOP);
				}
			}
			filename_edit->SetVisible(false);
		}

	} else if (evt==WM_NOTIFY) {
		if (p_ctrl==rubik_ctrl)
			ShowMenu();
	}
}





















