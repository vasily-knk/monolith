#ifndef INC_117_RUBIK_H
#define INC_117_RUBIK_H

struct rubik_loc_t {
	int c;
	int dir;
	inline rubik_loc_t() {c=0; dir=0;}
	inline rubik_loc_t(const rubik_loc_t& a) {c=a.c; dir=a.dir;}
};

class CRubik {
public:
	CRubik();
	CRubik (CRubik&);

	void Mix ();
	void Reset ();
	void Copy (CRubik&);
	void Copy (CRubik*);

	void Rotate (int plane, bool dir);
	bool Won ();

	int GetCornerPlane(int loc, int n);
	int GetBridgePlane(int loc, int n);

	int GetBridge (int plane, int n);
	int GetCorner (int plane, int n);

	rubik_loc_t MoveCorner(int plane, rubik_loc_t victim, int n, bool dir);
	rubik_loc_t MoveBridge(int plane, rubik_loc_t victim, int n, bool dir);

	v_result Save (const char*);
	v_result Load (const char*);

	int GetColor (int plane, int x, int y);
	int GetColorCorner (int plane, int n);
	int GetColorBridge (int plane, int n);
	int ActualColorCorner (int n, int plane);
	int ActualColorBridge (int n, int plane);

	rubik_loc_t Corner (int i) {return corners[i];}
	rubik_loc_t Bridge (int i) {return bridges[i];}

	int GetPlaneOnCornerId (int c, int plane);
	int GetPlaneOnBridgeId (int b, int plane);
	int GetCornerInPlaneId (int plane, int c);
	int GetBridgeInPlaneId (int plane, int c);
	int GetOpposite (int plane) {return ((~(plane+1))&7)-1;}

	void Review ();
private:
	rubik_loc_t corners[8], bridges[12];
public:
	bool supercorners[8], superbridges[12];
};

#endif