#ifndef INC_117_CTRL_H
#define INC_117_CTRL_H

#include "../common/117_list.h"
#include "../common/renderer.h"
#include "vector.h"
#include "keys.h"
extern API_renderer_t ri;
extern r_font_t r_font;

class CWindow;

class CControl {
public:
	CControl () {Resize(0,0,0,0); visible=true;}
	virtual void Draw () {};
public:
	int GetLeft		() {return left;	}
	int GetTop		() {return top;		}
	int GetWidth	() {return width;	}
	int GetHeight	() {return height;	}
	bool GetVisible () {return visible;	}

	bool CheckIn	(int x, int y);

	void Resize		(int new_left, int new_top, int new_width, int new_height) {
		left=new_left; top=new_top; width=new_width; height=new_height;
	}
	void Move (int x, int y) {
		left=x; top=y;
	}
	bool SetVisible (bool new_visible) {return visible=new_visible;	}

	virtual void Event (int, v_uint16, v_uint32) {};
	void SetOwner (CWindow* new_owner) {owner=new_owner;}
	CWindow* GetOwner () {return owner;}
protected:
	int left, top, width, height;
	bool visible;
	CWindow* owner;
};

class CBtn: public CControl {
public:
	CBtn ();
	CBtn (const char*);
	virtual void DrawCaption ();
	void Draw ();
	void Event (int, v_uint16, v_uint32);
public:
	void SetCaption (const char* str) {strcpy(caption, str);}
	void GetCaption (char* dst) {strcpy(dst, caption);}
	void SetColor (v_uint8 r,v_uint8 g,v_uint8 b) {red=r; green=g; blue=b;}
protected:
	char caption[MAX_LOADSTRING];
	bool down;
	v_uint8 red,green,blue;
};

class CRadio;
class CRadioGroup : public CControl{
public:
	CRadioGroup ();
	void SetValue (int);
	int GetValue () {return value;}

	void AddRadio (CRadio*);
private:
	C117List<CRadio*> list;
	int value;
};

class CRadio : public CBtn {
public:
	CRadio ();
	CRadio (const char* , CRadioGroup* o=0, int v=0);
public:
	void Check();
	void DrawCaption ();
	void Draw();
	void SetOwner (CRadioGroup* o) {owner=o;}
	bool GetChecked () {return checked;}
	int GetValue () {return value;}
	int SetValue (int v) {return value=v;}
	bool SetChecked (bool c) {return checked=c;}
	void Event (int, v_uint16, v_uint32);
private:
	bool checked;
	int value;
	CRadioGroup* owner;
};

class CFocusCtrl:public CControl {
public:
	CFocusCtrl ();
	void Event (int, v_uint16, v_uint32);
	bool GetFocus () {return focus;}
protected:
	bool focus;
};

class CMyEdit : public CFocusCtrl {
public:
	CMyEdit ();
	CMyEdit (const char *);
	void Draw ();
	void Event (int, v_uint16, v_uint32);
public:
	void SetText (const char* str) {strcpy(text, str);}
	void GetText (char* dst) {strcpy(dst, text);}
private:
	char text[MAX_LOADSTRING], prev_text[MAX_LOADSTRING];
};

class CImage : public CControl {
public:
	CImage ();
	CImage (const char*);
	~CImage();
	void Draw ();
public:
	v_result LoadImage (const char* str);
	r_pic_t GetImage () {return pic;}
	void UnloadImage ();
private:
	r_pic_t pic;
};

typedef CControl* ctrlptr_t;
typedef C117List<ctrlptr_t> ctrllist_t;
typedef C117List<ctrlptr_t>::C117ListIT ctrllist_it_t;


class CWindow {
public:
	CWindow ();
	~CWindow ();
	virtual void Init () {};
	virtual void Close () {};
public:
	void AddControl (CControl*);
public:
	virtual void Draw ();
	CControl* GetIn (int x, int y);

	virtual void Evt (CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam);
	void FocusEvt (CControl* p_ctrl, int evt, v_uint16 wParam, v_uint32 lParam);
	void FirstEvt (int evt, v_uint16 wParam, v_uint32 lParam);
	void KeyEvent (int key, bool down);

private:
	ctrllist_t list;
	CControl *p_lbtnctrl;
	CControl *p_focusctrl;

};


#endif