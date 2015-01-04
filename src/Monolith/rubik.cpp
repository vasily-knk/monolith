#include "../common/middefs.h"
#include "rubik.h"

int b_planes_arr[24] = {	0,1,  0,2,  0,3,  0,4,  
							1,2,  1,3,  1,5,  2,4,
							2,5,  3,4,  3,5,  4,5};
/*int c_planes_arr[24] = {	0,1,2,   0,1,3,   0,2,4,   0,3,4,
							1,2,5,   1,3,5,   2,4,5,   3,4,5};*/
int c_planes_arr[24] = {	0,2,1,   0,1,3,   0,4,2,   0,3,4,
							1,2,5,   1,5,3,   2,4,5,   3,5,4};

int bridges_arr[24] = {	0,2,3,1,   4,6,5,0,   1,7,8,4,
						5,10,9,2,  3,9,11,7,  8,11,10,6	};
						
int corners_arr[24] = {	0,1,3,2,   0,4,5,1,  0,2,6,4,
						1,5,7,3,   2,3,7,6,  4,6,7,5};


CRubik::CRubik () {
	Reset ();
}
CRubik::CRubik(CRubik& c) {
	Copy (c);
}
void CRubik::Reset() {
	for (int i=0; i<8; i++) {
		corners[i].c=i;
		corners[i].dir=0;
		supercorners[i]=false;
	}
	for (i=0; i<12; i++) {
		bridges[i].c=i;
		bridges[i].dir=0;
	} 
}
void CRubik::Copy(CRubik& c) {
	rubik_loc_t loc;
	for (int i=0; i<8; i++) {
		loc=c.Corner(i);
		corners[i].c=loc.c;
		corners[i].dir=loc.dir;
		supercorners[i]=false;
	}
	for (i=0; i<12; i++) {
		loc=c.Bridge(i);
		bridges[i].c=loc.c;
		bridges[i].dir=loc.dir;
	} 
}
void CRubik::Copy(CRubik * p) {
	rubik_loc_t loc;
	for (int i=0; i<8; i++) {
		loc=p->Corner(i);
		corners[i].c=loc.c;
		corners[i].dir=loc.dir;
		supercorners[i]=false;
	}
	for (i=0; i<12; i++) {
		loc=p->Bridge(i);
		bridges[i].c=loc.c;
		bridges[i].dir=loc.dir;
	} 
}
void CRubik::Mix() {
	/*int array[12], left=8, n, i, j;
	for (i=0; i<8; i++) 
		array[i]=i;
	for (i=0; i<8; i++) {
		n=rand()%left;
		corners[i].c=array[n];
		corners[i].dir=rand()%3;
		left--;
		for (j=n; j<left; j++)
			array[j]=array[j+1];
	}

	left=12;
	for (i=0; i<12; i++) 
		array[i]=i;
	for (i=0; i<12; i++) {
		n=rand()%left;
		bridges[i].c=array[n];
		bridges[i].dir=rand()%2;
		left--;
		for (j=n; j<left; j++)
			array[j]=array[j+1];
	}*/
	Reset ();
	for (int i=0; i<30000; i++) {
		Rotate (rand()%6, (rand()%2)==1);
	}
}

bool CRubik::Won() {
	for (int i=0; i<8; i++) {
		if (corners[i].c!=i || corners[i].dir!=0)
			return false;
	}
	for (i=0; i<12; i++) {
		if (bridges[i].c!=i || bridges[i].dir!=0)
			return false;
	}
	return true;
}

int CRubik::GetBridgePlane (int loc, int n) {
	return b_planes_arr[loc*2+n];
}
int CRubik::GetCornerPlane (int loc, int n) {
	return c_planes_arr[loc*3+n];
}
int CRubik::GetBridge (int plane, int n) {
	return bridges_arr[plane*4+n];
}
int CRubik::GetCorner (int plane, int n) {
	return corners_arr[plane*4+n];
}

rubik_loc_t CRubik::MoveCorner(int plane, rubik_loc_t victim, int n, bool dir) {
	int n1;
	if (dir) {
		n1=n+1;
		if (n1>=4) n1-=4;
	} else {
		n1=n-1;
		if (n1<0) n1+=4;
	}
	rubik_loc_t ret=corners[GetCorner(plane, n1)];

	int pos0=GetCorner(plane,n), pos1=GetCorner(plane,n1);
	corners[pos1]=victim;

	for (int i=0; i<4; i++) {
		if (GetCornerPlane(pos0,i)==plane) break;
	}
	if (i>=3) 
		MessageBox (0, "AAA", "AAA", MB_OK);
	n=(i+victim.dir)%3;

	for (i=0; i<4; i++) {
		if (GetCornerPlane(pos1,i)==plane) break;
	}
	if (i>=3) MessageBox (0, "BBB", "BBB", MB_OK);
	int new_dir=n-i;
	if (new_dir>=3)
		new_dir-=3;
	if (new_dir<0)
		new_dir+=3;
	
	corners[pos1].dir=new_dir;

	return ret;
}

rubik_loc_t CRubik::MoveBridge(int plane, rubik_loc_t victim, int n, bool dir) {
	int n1;
	if (dir) {
		n1=n+1;
		if (n1>=4) n1-=4;
	} else {
		n1=n-1;
		if (n1<0) n1+=4;
	}
	rubik_loc_t ret=bridges[GetBridge(plane, n1)];

	int pos0=GetBridge(plane,n), pos1=GetBridge(plane,n1);
	bridges[pos1]=victim;

	for (int i=0; i<3; i++) {
		if (GetBridgePlane(pos0,i)==plane) break;
	}
	if (i>=2) 
		MessageBox (0, "CCC", "CCC", MB_OK);
	n=(i+victim.dir)%2;

	for (i=0; i<3; i++) {
		if (GetBridgePlane(pos1,i)==plane) break;
	}
	if (i>=2) MessageBox (0, "DDD", "DDD", MB_OK);
	int new_dir=n-i;
	if (new_dir>=2)
		new_dir-=2;
	if (new_dir<0)
		new_dir+=2;
	
	bridges[pos1].dir=new_dir;

	return ret;
}

void CRubik::Rotate(int plane, bool dir) {
	rubik_loc_t temp;
	if (dir) {
		temp=MoveCorner (plane, corners[GetCorner(plane,0)],0, dir);
		temp=MoveCorner (plane, temp, 1, dir);
		temp=MoveCorner (plane, temp, 2, dir);
		MoveCorner(plane, temp, 3, dir);
		temp=MoveBridge (plane, bridges[GetBridge(plane,0)],0, dir);
		temp=MoveBridge (plane, temp, 1, dir);
		temp=MoveBridge (plane, temp, 2, dir);
		MoveBridge(plane, temp, 3, dir);
	} else {
		temp=MoveCorner (plane, corners[GetCorner(plane,3)],3, dir);
		temp=MoveCorner (plane, temp, 2, dir);
		temp=MoveCorner (plane, temp, 1, dir);
		MoveCorner(plane, temp, 0, dir);
		temp=MoveBridge (plane, bridges[GetBridge(plane,3)],3, dir);
		temp=MoveBridge (plane, temp, 2, dir);
		temp=MoveBridge (plane, temp, 1, dir);
		MoveBridge(plane, temp, 0, dir);
	}
}

int CRubik::GetColor(int plane, int x, int y) {
	plane%=6;
	x%=3; y%=3;
	if (x==1 && y==1) return plane;
	if (x==0 && y==0)
		return GetColorCorner(plane,0);
	if (x==0 && y==2)
		return GetColorCorner(plane,1);
	if (x==2 && y==2)
		return GetColorCorner(plane,2);
	if (x==2 && y==0)
		return GetColorCorner(plane,3);

	if (x==0 && y==1)
		return GetColorBridge(plane,0);
	if (x==1 && y==2)
		return GetColorBridge(plane,1);
	if (x==2 && y==1)
		return GetColorBridge(plane,2);
	if (x==1 && y==0)
		return GetColorBridge(plane,3);
	return -1;
}

int CRubik::GetColorCorner(int plane, int n) {
	int c_place=GetCorner(plane,n),
		c_corner=corners[c_place].c,
		c_dir=corners[c_place].dir;

	if (supercorners[c_place]) return 6;
	
	for (int i=0; i<3; i++) {
		if (GetCornerPlane(c_place,i)==plane) break;
	}
	return GetCornerPlane (c_corner, ( i+c_dir)%3);
}

/*void CRubik::Review() {
	for (int i=0; i<8; i++) {
		corners[i].dir=0;
	}
}*/

int CRubik::GetColorBridge(int plane, int n) {
	int b_place=GetBridge (plane,n),
		b_bridge=bridges[b_place].c,
		b_dir=bridges[b_place].dir;
	
	int i=(GetBridgePlane(b_place,0)==plane)?0:1;
	return GetBridgePlane (b_bridge, (i+b_dir)%2);//   b_planes_arr[b_bridge*2+((i+b_dir)%2)];
}

int CRubik::ActualColorCorner(int n, int plane) {
	return GetCornerPlane(corners[n].c, (plane+corners[n].dir)%3);
}

int CRubik::ActualColorBridge(int n, int plane) {
	return GetBridgePlane(bridges[n].c, (plane+bridges[n].dir)%2);
}

int CRubik::GetCornerInPlaneId (int plane, int c) {
	for (int i=0; i<4; i++) {
		if (GetCorner (plane,i)==c)
			return i;
	}
	return -1;
}
int CRubik::GetBridgeInPlaneId (int plane, int b) {
	for (int i=0; i<4; i++) {
		if (GetBridge (plane,i)==b)
			return i;
	}
	return -1;
}
int CRubik::GetPlaneOnCornerId (int c, int plane) {
	for (int i=0; i<3; i++) {
		if (GetCornerPlane (c,i)==plane)
			return i;
	}
	return -1;
}
int CRubik::GetPlaneOnBridgeId (int c, int plane) {
	for (int i=0; i<2; i++) {
		if (GetBridgePlane (c,i)==plane)
			return i;
	}
	return -1;
}


v_result CRubik::Save(const char* filename) {
	if (!filename || *filename==0)
		return V_FAIL;
	FILE* fff=fopen (filename, "wb");
	if (!fff) return V_FAIL;

	for (int i=0; i<8; i++)
		fwrite (corners+i, sizeof(rubik_loc_t), 1, fff);
	for (i=0; i<12; i++)
		fwrite (bridges+i, sizeof(rubik_loc_t), 1, fff);

	fclose (fff);
	return V_OK;
}

v_result CRubik::Load(const char* filename) {
	if (!filename || *filename==0)
		return V_FAIL;
	FILE* fff=fopen (filename, "rb");
	if (!fff) return V_FAIL;

	for (int i=0; i<8; i++)
		fread (corners+i, sizeof(rubik_loc_t), 1, fff);
	for (i=0; i<12; i++)
		fread (bridges+i, sizeof(rubik_loc_t), 1, fff);

	fclose (fff);
	return V_OK;
}
