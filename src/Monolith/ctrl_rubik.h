#ifndef INC_117_CTRL_RUBIK_H
#define INC_117_CTRL_RUBIK_H

#include "ctrl.h"
#include "rubik.h"

extern char colors_names [6][MAX_LOADSTRING];
extern v_uint8 planecolors[18];

struct rubik_rot_t {
	int plane;
	bool dir;
	bool achtung;
	rubik_rot_t () {plane=0; dir=false; achtung=false;}
};

class CRubikCtrl : public CControl {
public:
	CRubikCtrl ();
	void DrawCube ();
	void Draw ();
	void DrawCorner (int);
	void DrawBridge (int);
	void DrawCenter (int);

	void RotFinished ();
	void Event (int, v_uint16, v_uint32);
	void Rotate (int side, bool dir);
	void Mix ();

	bool CheckWon ();

	v_result Save (const char* name) {return cube.Save(name);}
	v_result Load (const char* name) {return cube.Load(name);}

	void SetPlaneToRot(int plane) {plane_to_rot=plane;}
private:
	bool down;
	int lastx, lasty;
	float ang_x, ang_y, ang_z;
	int rot_started, rot_finished;
	float rot_ang;
	bool rotating, rot_dir;
	int plane_to_rot, rot_plane;
	CRubik cube;
public:
	void Solve ();
	void ProcessList ();
	void ResetSolution();
	void BuildList ();
	void BringTopCorner (int);
	void BringTopBridge (int);
	void BringBelt (int);
	void BottomCorners ();
	void FinishCorners ();
	bool FC_CheckFinished ();
	int FC_CheckSeven (int);
	bool FB_CheckFinished ();

	void BottomBridges ();
	void FinishBridges ();

	void Fish (int);
	void HHH (int );
private:
	bool solving;
	C117List<rubik_rot_t> list;
};
#endif