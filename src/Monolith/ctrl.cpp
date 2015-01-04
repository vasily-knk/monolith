#include "../common/middefs.h"
#include "../common/renderer.h"
#include "keys.h"
#include "ctrl.h"
#include "main.h"
#include "vector.h"

bool CControl::CheckIn	(int x, int y) {
	return(x>=left && y>= top && x<left+width && y<top+height);
}
	
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CBtn
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

CBtn::CBtn() {
	CControl();
	strcpy (caption, "");
	down=false;
	red=green=blue=255;
}
CBtn::CBtn(const char* str) {
	CControl();
	strcpy (caption, str);
	down=false;
	red=green=blue=255;
}

void CBtn::DrawCaption () {
	int caption_x=left+(width-ri.R_GetTextWidth(caption))/2,
		caption_y=top+(height-ri.R_GetTextHeight(caption))/2;
	int x1, x2, y1, y2;
	ri.R_GetClip(&x1, &y1, &x2, &y2);
	ri.R_Clip(left, top, left+width-1, top+height-1);
	ri.R_Text (caption, caption_x, caption_y);
	ri.R_Clip(x1, y1, x2, y2);
}
void CBtn::Draw() {
	ri.R_SetAlpha(255);
	ri.R_SetColor (float(red)*0.75f, float(green)*0.75f, float(blue)*0.75f);
	ri.R_Rect (left, top, left+width-1, top+height-1);
	ri.R_SetScale(1,1);
	if (down)  {
		ri.R_SetColor (red, green, blue);
		ri.R_Rect (left+1, top+1, left+width-2, top+height-2);
		ri.R_SetScale(0.95f, 0.95f);
	}
	DrawCaption();
}

void CBtn::Event (int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_LBUTTONDOWN) {
		down=true;
	} else if (evt==WM_LBUTTONUP || evt==WM_LBUTTONDBLCLK) {
		down=false;
	}
	CControl::Event(evt, wParam, lParam);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CRadio
// --------------------------------------------------------------------------------

CRadioGroup::CRadioGroup() {
	CControl();
	value=0;
}
void CRadioGroup::AddRadio(CRadio* r) {
	if (!r) return;
	r->SetOwner(this);
	list.AddLast(r);
}
void CRadioGroup::SetValue(int v) {
	for (C117List<CRadio*>::C117ListIT it=list.head; it!=0; it=it->next) {
		if (it->val->GetValue()==v)
			it->val->SetChecked (true);
		else
			it->val->SetChecked (false);

	}
	value=v;
	if (owner)
		owner->Evt (this, WM_LBUTTONDOWN, 0, value);
}

#define RADIO_CHECK_WIDTH 16
CRadio::CRadio() {
	CBtn();
	owner=0;value=0;checked=0;
}
CRadio::CRadio(const char* str, CRadioGroup* o, int v) {
	CBtn();
	strcpy (caption, str);
	checked=false;
	value=v;
	if (o)
		o->AddRadio(this);
	else
		owner=0;
}
void CRadio::DrawCaption () {
	int caption_x=left+(width-ri.R_GetTextWidth(caption)+16)/2,
		caption_y=top+(height-ri.R_GetTextHeight(caption))/2;
	int x1, x2, y1, y2;
	ri.R_GetClip(&x1, &y1, &x2, &y2);
	ri.R_Clip(left, top, left+width-1, top+height-1);
	ri.R_Text (caption, caption_x, caption_y);
	ri.R_Clip(x1, y1, x2, y2);
}
void CRadio::Draw () {
	CBtn::Draw ();
	ri.R_Rect(left+4, top+4, left+15, top+15);
	if (checked)
		ri.R_Box(left+7, top+7, left+12, top+12);
}

void CRadio::Check() {
	if (owner)
		owner->SetValue(value);
}
void CRadio::Event (int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_LBUTTONDBLCLK) {
		Check ();
	}
	CBtn::Event(evt, wParam, lParam);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CEdit
// --------------------------------------------------------------------------------

CFocusCtrl::CFocusCtrl() {
	focus=false;
	CControl::CControl();
}

void CFocusCtrl ::Event(int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_LBUTTONDOWN) {
		if (!focus)
			owner->Evt (this, WM_SETFOCUS, 0, 0);
	} else if (evt==WM_KEYDOWN) {
		if (lParam==K_ESCAPE) {
			owner->Evt (this, WM_KILLFOCUS, false ,0);
		}
	}
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CEdit
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void KeyEdit (int key, char* type_str, int max_len) {
	int len=strlen (type_str);
	if (key>=32 && key<127 && len <MAX_LOADSTRING-1) {
		type_str[len+1]=0;
		type_str[len]=key;
	} else if (key==K_BACKSPACE) {
		if (len>0)
			type_str[len-1]=0;
	}

}

CMyEdit::CMyEdit() {
	CFocusCtrl::CFocusCtrl();
	strcpy (text, "");
	strcpy (prev_text, "");
}
CMyEdit::CMyEdit(const char* str) {
	CFocusCtrl::CFocusCtrl();
	strcpy (text, str);
	strcpy (prev_text, "");
}

void CMyEdit::Draw() {
	ri.R_SetColor (192, 192, 192);
	ri.R_Rect (left, top, left+width-1, top+height-1);
	ri.R_Line(left+8, top,left+8, top+height-1);
	ri.R_SetScale(1,1);
	if (focus)  {
		ri.R_SetColor (255, 255, 255);
		ri.R_Rect (left+9, top+1, left+width-2, top+height-2);
	}
	int caption_x=left+(width-ri.R_GetTextWidth(text))/2,
		caption_y=top+(height-ri.R_GetTextHeight(text))/2;
	int x1, x2, y1, y2;
	ri.R_GetClip(&x1, &y1, &x2, &y2);
	ri.R_Clip(left+8, top, left+width-1, top+height-1);
	ri.R_Text (text, caption_x, caption_y);
	ri.R_Clip(x1, y1, x2, y2);
}

void CMyEdit::Event(int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_SETFOCUS) {
		strcpy (prev_text, text);
		focus=true;
	} else if (evt==WM_KILLFOCUS) {
		focus=false;
	} else if (evt==WM_KEYDOWN) {
		if (lParam==K_ENTER || lParam==K_KP_ENTER) {
			owner->Evt (this, WM_KILLFOCUS, true ,0);
		} else {
			KeyEdit (lParam, text, MAX_LOADSTRING-1);
		}
	}
	CFocusCtrl::Event(evt, wParam, lParam);
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CImage
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
CImage::CImage() {
	CControl();
	pic=0;
}

CImage::~CImage() {
	UnloadImage();
}
CImage::CImage (const char* str) {
	CControl();
	pic=0;
	LoadImage (str);
}
void CImage::Draw () {
	ri.R_SetColor(255,255,255);
	ri.R_SetAlpha(255);
	if (pic) {
		ri.R_DrawPicRect (pic, left, top, left+width, top+height);
	} else {
		ri.R_Rect (left, top, left+width-1, top+height-1);
	}
}
v_result CImage::LoadImage (const char* str) {
	UnloadImage();
	if (ri.R_LoadPicBMP(str, &pic)!=V_OK) {
		pic=0;
		return V_FAIL;
	}
	return V_OK;
}
void CImage::UnloadImage () {
	if (pic)
		ri.R_UnLoadPic(pic);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CWindow::
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

CWindow::CWindow() {
	p_focusctrl=p_lbtnctrl=0;
}
CWindow::~CWindow() {
	while (!list.IsEmpty()) {
		delete (list.GetTail()->val);
		list.RemoveLast ();
	}
}

void CWindow::AddControl(CControl* ctrl) {
	if (!ctrl) return;
	list.AddLast (ctrl);
	ctrl->SetOwner(this);
}

void CWindow::FocusEvt (CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_SETFOCUS) {
		if (p_ctrl!=p_focusctrl)
			Evt (p_focusctrl, WM_KILLFOCUS,0,0);
		p_focusctrl=p_ctrl;
	} else if (evt==WM_KILLFOCUS) {
		if (p_ctrl!=p_focusctrl)
			p_focusctrl->Event(WM_KILLFOCUS, 0,0);
		p_focusctrl=0;
	}
}

void CWindow::Evt (CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_KILLFOCUS || evt==WM_SETFOCUS)
		FocusEvt (p_ctrl, evt, wParam, lParam);
	if (p_ctrl)
		p_ctrl->Event(evt, wParam, lParam);
}

CControl* CWindow::GetIn (int x, int y) {
	CControl* result=0;
	for (ctrllist_it_t it=list.head; it!=0; it=it->next) {
		if (it->val->GetVisible() && it->val->CheckIn(x,y))
			result=it->val;
	}
	return result;
}


void CWindow::FirstEvt (int evt, v_uint16 wParam, v_uint32 lParam) {
	bool flag=false;
	if (evt==WM_LBUTTONDOWN || evt==WM_LBUTTONUP || evt==WM_MOUSEMOVE) {
		CControl* p_inctrl=GetIn (LOWORD(lParam), HIWORD(lParam));
		if (evt==WM_LBUTTONUP  && p_lbtnctrl!=0) {
			Evt (p_inctrl, WM_LBUTTONUP, wParam, lParam);
			Evt (p_lbtnctrl, WM_LBUTTONDBLCLK , wParam, lParam);
			p_lbtnctrl=0;
		} else if (evt==WM_LBUTTONDOWN) {
			if (p_focusctrl && p_focusctrl!=p_inctrl) {
				Evt(p_focusctrl, WM_KILLFOCUS, 0, 0);
				flag=true;
			} else {
				if (p_lbtnctrl) 
					Evt (p_lbtnctrl, WM_LBUTTONUP, wParam, lParam);
				p_lbtnctrl=p_inctrl;
			}
		} else if (evt==WM_MOUSEMOVE) {
			if (!(wParam & MK_LBUTTON) && p_lbtnctrl!=0) {
				Evt (p_lbtnctrl, WM_LBUTTONUP, wParam, lParam);
				p_lbtnctrl=0;
			}
		}
		if (!flag)
			Evt (p_inctrl, evt, wParam, lParam);
	} else if (evt==WM_TIMER) {
		for (ctrllist_it_t it=list.head; it!=0; it=it->next)
			it->val->Event (WM_TIMER,wParam,lParam);
	}
}

void CWindow::KeyEvent (int key, bool down) {
	if (down) 
		Evt (p_focusctrl, WM_KEYDOWN, 0, key);
	else
		Evt (p_focusctrl, WM_KEYUP, 0, key);
		
}


void CWindow::Draw() {
	for (ctrllist_it_t it=list.head; it!=0; it=it->next) {
		if (it->val->GetVisible()) 
			it->val->Draw ();
	}
}