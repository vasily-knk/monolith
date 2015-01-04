#include "../common/middefs.h"
#include "ctrl_rubik.h"
#include "main.h"

void Rearrange (Vector3D* src, Vector3D* dst) {
	Vector3D temp;
	dst[0]=Vector3D (1,0,0);
	dst[1]=Vector3D (0,1,0);
	dst[2]=Vector3D (0,0,1);
	int i;
	for (i=0; i<3; i++) {
		if (src[i].x!=0) {
			temp=src[0]; src[0]=src[i]; src[i]=temp;
			temp=dst[0]; dst[0]=dst[i]; dst[i]=temp;
			dst[0]=dst[0]/src[0].x;
		}
	}
	if (src[1].y==0) {
		temp=src[1]; src[1]=src[2]; src[2]=temp;
		temp=dst[1]; dst[1]=dst[2]; dst[2]=temp;
	}
	dst[1]=dst[1]/src[1].y;
	dst[2]=dst[2]/src[2].z;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// CRubikCtrl
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

char colors_names[6][MAX_LOADSTRING] = {
	"Blue", "Green", "Cyan", "Red", "Magenta", "Yellow"
};

CRubikCtrl::CRubikCtrl() {
	CControl::CControl();
	ang_x=ang_y=ang_z=0;
	lastx=lasty=0;
	down=0;
	rotating=false;
	plane_to_rot=0;
	rot_plane=0;

	solving=false;
}

#define OD6 0.1666f
#define OD3 0.3333f

void CRubikCtrl::Rotate (int side, bool dir) {
	if (rotating)
		return;
	
	rot_plane=side;
	rotating=true;
	rot_dir=dir;
	rot_started=GetTickCount();
	rot_finished=rot_started+500;
}

Vector3D planevecs[6]={
	Vector3D(0,0,1), Vector3D(0,1,0), Vector3D(-1,0,0),
	Vector3D(1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
};
v_uint8 planecolors[18] = {
	0,0,1,   0,1,0,   0,1,1,
	1,0,0,   1,0,1,   1,1,0
};
#define GAP 0.01f
Vector3D cornersqr[4]={
	Vector3D (OD6+GAP, OD6+GAP, 0.51f),
	Vector3D (OD6+GAP, 0.5f-GAP, 0.51f),
	Vector3D (0.5f-GAP, 0.5f-GAP, 0.51f),
	Vector3D (0.5f-GAP, OD6+GAP, 0.51f)
};
Vector3D centersqr[4]={
	Vector3D (-OD6+GAP, -OD6+GAP, 0.51f),
	Vector3D (-OD6+GAP, OD6-GAP, 0.51f),
	Vector3D (OD6-GAP, OD6-GAP, 0.51f),
	Vector3D (OD6-GAP, -OD6+GAP, 0.51f)
};
Vector3D bridgesqr[4]={
	Vector3D (-OD6+GAP, OD6+GAP, 0.51f),
	Vector3D (-OD6+GAP, 0.5f-GAP, 0.51f),
	Vector3D (OD6-GAP,	0.5f-GAP, 0.51f),
	Vector3D (OD6-GAP,	OD6+GAP, 0.51f)
};

#define CUBE_LIGHT 192
#define CUBE_DARK 96
void CRubikCtrl::DrawCorner(int n) {
	for (int i=0; i<3; i++) {
		int p1=cube.GetCornerPlane(n, i), 
			p2=cube.GetCornerPlane(n, (i+1)%3),
			p3=cube.GetCornerPlane(n, (i+2)%3);
		v_uint8 r=planecolors[cube.ActualColorCorner(n, i)*3+0],
			g=planecolors[cube.ActualColorCorner(n, i)*3+1],
			b=planecolors[cube.ActualColorCorner(n, i)*3+2];
		if (!solving &&(cube.GetCornerPlane(n,0)==plane_to_rot || cube.GetCornerPlane(n,1)==plane_to_rot || cube.GetCornerPlane(n,2)==plane_to_rot))
			ri.R_SetColor(r*CUBE_LIGHT,g*CUBE_LIGHT,b*CUBE_LIGHT);
		else
			ri.R_SetColor(r*CUBE_DARK,g*CUBE_DARK,b*CUBE_DARK);
		
		
		Vector3D src[3]={planevecs[p3], planevecs[p2], planevecs[p1]},
			dst[3];
		Rearrange (src, dst);
		
		ri.R_Face3D(
			DotProduct(cornersqr[0],dst[0]),
			DotProduct(cornersqr[0],dst[1]),
			DotProduct(cornersqr[0],dst[2]),

			DotProduct(cornersqr[1],dst[0]),
			DotProduct(cornersqr[1],dst[1]),
			DotProduct(cornersqr[1],dst[2]),

			DotProduct(cornersqr[2],dst[0]),
			DotProduct(cornersqr[2],dst[1]),
			DotProduct(cornersqr[2],dst[2])
		);
		ri.R_Face3D(
			DotProduct(cornersqr[2],dst[0]),
			DotProduct(cornersqr[2],dst[1]),
			DotProduct(cornersqr[2],dst[2]),

			DotProduct(cornersqr[3],dst[0]),
			DotProduct(cornersqr[3],dst[1]),
			DotProduct(cornersqr[3],dst[2]),

			DotProduct(cornersqr[0],dst[0]),
			DotProduct(cornersqr[0],dst[1]),
			DotProduct(cornersqr[0],dst[2])
		);
	}
}

void CRubikCtrl::DrawCenter(int n) {
	Vector3D src[3], dst[3];
	src[2]=planevecs[n];
	if (planevecs[n].z!=0)
		src[0]=Vector3D(1,0,0);
	else if (planevecs[n].x!=0)
		src[0]=Vector3D(0,1,0);
	else
		src[0]=Vector3D(0,0,1);
	
	v_uint8 r=planecolors[n*3+0],
		g=planecolors[n*3+1],
		b=planecolors[n*3+2];
	
	if (!solving && n==plane_to_rot)
		ri.R_SetColor(r*CUBE_LIGHT,g*CUBE_LIGHT,b*CUBE_LIGHT);
	else
		ri.R_SetColor(r*CUBE_DARK,g*CUBE_DARK,b*CUBE_DARK);

	src[1]=CrossProduct(src[2],src[0]);
	Rearrange (src, dst);

	ri.R_Face3D(
		DotProduct(centersqr[0],dst[0]),
		DotProduct(centersqr[0],dst[1]),
		DotProduct(centersqr[0],dst[2]),
		
		DotProduct(centersqr[1],dst[0]),
		DotProduct(centersqr[1],dst[1]),
		DotProduct(centersqr[1],dst[2]),
		
		DotProduct(centersqr[2],dst[0]),
		DotProduct(centersqr[2],dst[1]),
		DotProduct(centersqr[2],dst[2])
		);
	ri.R_Face3D(
		DotProduct(centersqr[2],dst[0]),
		DotProduct(centersqr[2],dst[1]),
		DotProduct(centersqr[2],dst[2]),
		
		DotProduct(centersqr[3],dst[0]),
		DotProduct(centersqr[3],dst[1]),
		DotProduct(centersqr[3],dst[2]),
		
		DotProduct(centersqr[0],dst[0]),
		DotProduct(centersqr[0],dst[1]),
		DotProduct(centersqr[0],dst[2])
		);
}

void CRubikCtrl::DrawBridge(int n) {
	for (int i=0; i<2; i++) {
		int p1=cube.GetBridgePlane(n, i), 
			p2=cube.GetBridgePlane(n, 1-i);
		v_uint8 r=planecolors[cube.ActualColorBridge(n, i)*3+0],
			g=planecolors[cube.ActualColorBridge(n, i)*3+1],
			b=planecolors[cube.ActualColorBridge(n, i)*3+2];

	
		Vector3D src[3],dst[3];
		src[2]=planevecs[p1];
		src[1]=planevecs[p2];
		src[0]=CrossProduct(src[1], src[2]);
		Rearrange (src, dst);
		
		if (!solving && (cube.GetBridgePlane(n,0)==plane_to_rot || cube.GetBridgePlane(n,1)==plane_to_rot))
			ri.R_SetColor(r*CUBE_LIGHT,g*CUBE_LIGHT,b*CUBE_LIGHT);
		else
			ri.R_SetColor(r*CUBE_DARK,g*CUBE_DARK,b*CUBE_DARK);
		ri.R_Face3D(
			DotProduct(bridgesqr[0],dst[0]),
			DotProduct(bridgesqr[0],dst[1]),
			DotProduct(bridgesqr[0],dst[2]),

			DotProduct(bridgesqr[1],dst[0]),
			DotProduct(bridgesqr[1],dst[1]),
			DotProduct(bridgesqr[1],dst[2]),

			DotProduct(bridgesqr[2],dst[0]),
			DotProduct(bridgesqr[2],dst[1]),
			DotProduct(bridgesqr[2],dst[2])
		);
		ri.R_Face3D(
			DotProduct(bridgesqr[2],dst[0]),
			DotProduct(bridgesqr[2],dst[1]),
			DotProduct(bridgesqr[2],dst[2]),

			DotProduct(bridgesqr[3],dst[0]),
			DotProduct(bridgesqr[3],dst[1]),
			DotProduct(bridgesqr[3],dst[2]),

			DotProduct(bridgesqr[0],dst[0]),
			DotProduct(bridgesqr[0],dst[1]),
			DotProduct(bridgesqr[0],dst[2])
		);
	}
	
}
void CRubikCtrl::DrawCube() {
	for (int i=0; i<8; i++) {
		cube.supercorners[i]=false;
	}
	for (i=0; i<12; i++) {
		cube.superbridges[i]=false;
	}
	for (i=0; i<4;i++) {
		cube.supercorners[cube.GetCorner(rot_plane, i)]=true;
		cube.superbridges[cube.GetBridge(rot_plane, i)]=true;
	}
	for (i=0; i<8; i++) {
		if (!cube.supercorners[i])
			DrawCorner(i);
	}
		
	for (i=0; i<12; i++) {
		if (!cube.superbridges[i])
			DrawBridge(i);
	}
		
	for (i=0; i<8; i++) {
		if (i!=rot_plane)
			DrawCenter(i);
	}
		
	if (rotating) {
		float f_dir=(rot_dir)?(-1.0f):(1.0f);
		ri.R_MatrixRotateX(planevecs[rot_plane].x*f_dir*rot_ang);
		ri.R_MatrixRotateY(planevecs[rot_plane].y*f_dir*rot_ang);
		ri.R_MatrixRotateZ(planevecs[rot_plane].z*f_dir*rot_ang);
	}
	for (i=0; i<4;i++) {
		DrawCorner(cube.GetCorner(rot_plane, i));
		DrawBridge(cube.GetBridge(rot_plane, i));
		DrawCenter(rot_plane);
	}
}

/*void CRubikCtrl::DrawCube () {
	v_uint8 colors[21]= {
		0,0,1,
		0,1,0,
		0,1,1,
		1,0,0,
		1,0,1,
		1,1,0,

		1,1,1
	};
	Vector3D vec[18]= {
		Vector3D (0,1,0), Vector3D(-1,0,0), Vector3D(0,0,1),
		Vector3D (1,0,0), Vector3D(0,0,1), Vector3D(0,-1,0),
		Vector3D (0,0,-1), Vector3D(0,-1,0), Vector3D(-1,0,0),
		Vector3D (0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0),
		Vector3D (0,1,0), Vector3D(0,0,-1), Vector3D(-1,0,0),
		Vector3D (1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1),
	};
	float f0=-0.48f, f1=-OD6-0.02f;
	Vector3D	vecs[4]= {
		Vector3D(f0, f0, 0.51f),
		Vector3D(f0, f1, 0.51f),
		Vector3D(f1, f1, 0.51f),
		Vector3D(f1, f0, 0.51f)
	};
	Vector3D	blackvecs[4]= {
		Vector3D(-0.5f, -0.5f, 0.5f),
		Vector3D(-0.5f, 0.5f, 0.5f),
		Vector3D(0.5f, 0.5f, 0.5f),
		Vector3D(0.5f, -0.5f, 0.5f)
	};
	int i,j, k;
	Vector3D nvecs[4];
	for (int plane=0; plane<6; plane++) {
		ri.R_SetColor(0,0,0);
				ri.R_Face3D(
					DotProduct(blackvecs[0], vec[plane*3+0]), 
					DotProduct(blackvecs[0], vec[plane*3+1]), 
					DotProduct(blackvecs[0], vec[plane*3+2]), 

					DotProduct(blackvecs[1], vec[plane*3+0]), 
					DotProduct(blackvecs[1], vec[plane*3+1]), 
					DotProduct(blackvecs[1], vec[plane*3+2]), 

					DotProduct(blackvecs[2], vec[plane*3+0]), 
					DotProduct(blackvecs[2], vec[plane*3+1]), 
					DotProduct(blackvecs[2], vec[plane*3+2])
				);
				ri.R_Face3D(
					DotProduct(blackvecs[2], vec[plane*3+0]), 
					DotProduct(blackvecs[2], vec[plane*3+1]), 
					DotProduct(blackvecs[2], vec[plane*3+2]), 

					DotProduct(blackvecs[3], vec[plane*3+0]), 
					DotProduct(blackvecs[3], vec[plane*3+1]), 
					DotProduct(blackvecs[3], vec[plane*3+2]), 

					DotProduct(blackvecs[0], vec[plane*3+0]), 
					DotProduct(blackvecs[0], vec[plane*3+1]), 
					DotProduct(blackvecs[0], vec[plane*3+2])
				);


		for (j=0; j<3; j++) {
			for (i=0; i<3; i++) {
				for (k=0; k<4; k++)
					nvecs[k]=Vector3D(vecs[k].x+OD3*float(i), vecs[k].y+OD3*float(j), vecs[k].z);

				ri.R_SetColor(
					colors[cube.GetColor(plane,i,j)*3+0]*128,
					colors[cube.GetColor(plane,i,j)*3+1]*128,
					colors[cube.GetColor(plane,i,j)*3+2]*128);
				ri.R_Face3D(
					DotProduct(nvecs[0], vec[plane*3+0]), 
					DotProduct(nvecs[0], vec[plane*3+1]), 
					DotProduct(nvecs[0], vec[plane*3+2]), 

					DotProduct(nvecs[1], vec[plane*3+0]), 
					DotProduct(nvecs[1], vec[plane*3+1]), 
					DotProduct(nvecs[1], vec[plane*3+2]), 

					DotProduct(nvecs[2], vec[plane*3+0]), 
					DotProduct(nvecs[2], vec[plane*3+1]), 
					DotProduct(nvecs[2], vec[plane*3+2])
				);
				ri.R_Face3D(
					DotProduct(nvecs[2], vec[plane*3+0]), 
					DotProduct(nvecs[2], vec[plane*3+1]), 
					DotProduct(nvecs[2], vec[plane*3+2]), 

					DotProduct(nvecs[3], vec[plane*3+0]), 
					DotProduct(nvecs[3], vec[plane*3+1]), 
					DotProduct(nvecs[3], vec[plane*3+2]), 

					DotProduct(nvecs[0], vec[plane*3+0]), 
					DotProduct(nvecs[0], vec[plane*3+1]), 
					DotProduct(nvecs[0], vec[plane*3+2])
				);

			}
		}
	}
}*/

#define NUM 3
void CRubikCtrl::Draw() {
	ri.R_SetColor(255,255,255);
	ri.R_Rect(left, top, left+width-1, top+height-1);

	ri.R_SwitchTo3D(left, top, width, height, V_PI/4);
	ri.R_MatrixReset();
	ri.R_MatrixMove(0,0, -3);
	ri.R_MatrixRotateX(ang_x);
	ri.R_MatrixRotateY(ang_y);


	/*for (int i=0; i<6; i++) {
		ri.R_SetColor(planecolors[i*3+0]*255, planecolors[i*3+1]*255, planecolors[i*3+2]*255);
		ri.R_Line3D(planevecs[i].x*0.5f, planevecs[i].y*0.5f, planevecs[i].z*0.5f, planevecs[i].x, planevecs[i].y, planevecs[i].z);
	}*/

	DrawCube();
		
	ri.R_Restore2D();
}

void CRubikCtrl::RotFinished () {
	rotating=false;
	cube.Rotate(rot_plane, rot_dir);
	if (!solving) {
		CheckWon();
	} else {
		ProcessList();
	}
}

void CRubikCtrl::Event(int evt, v_uint16 wParam, v_uint32 lParam) {
	if (evt==WM_LBUTTONDOWN) {
		lastx=LOWORD(lParam); lasty=HIWORD(lParam);
		down=true;
	} else if (down && (evt==WM_LBUTTONUP || (evt==WM_MOUSEMOVE && wParam & MK_LBUTTON))) {
		ang_x+=float(HIWORD(lParam)-lasty)/float(height)*V_PI;
		ang_y+=float(LOWORD(lParam)-lastx)/float(width)*V_PI;
		lastx=LOWORD(lParam); lasty=HIWORD(lParam);
		if (evt==WM_LBUTTONDBLCLK)
			down=false;
	} else if (evt==WM_TIMER) {
		if (rotating) {
			int cur_time=GetTickCount();
			if (cur_time>rot_finished) {
				RotFinished ();
			} else
				rot_ang= (V_PI/2) * (float (cur_time-rot_started)/float (rot_finished-rot_started));
		}
	}
}

void CRubikCtrl::Mix() {
	if (MessageBox (hMainWnd, "Are you sure you want to mix the cube?", "Monolith", MB_YESNO|MB_ICONQUESTION )==IDYES) {
		ResetSolution();
		cube.Mix();
	}
}

bool CRubikCtrl::CheckWon() {
	if (cube.Won ()) {
		MessageBox (hMainWnd, "You have won!", "Congratulations!", MB_ICONINFORMATION | MB_OK);
		return true;
	}
	return false;
}

void CRubikCtrl::ProcessList () {
	if (!solving || list.IsEmpty()) {
		CheckWon ();
		owner->Evt(this, WM_NOTIFY, 0, 0);
		solving=false;
	} else {
		if (list.GetHead()->val.achtung) {
			MessageBox (hMainWnd, "ACHTUNG!\n", "Monolith", MB_OK);
			solving=false;
			list.GetHead()->val.achtung=false;
			return;
		}

		Rotate (list.GetHead()->val.plane, list.GetHead()->val.dir);
		list.RemoveFirst();
	}
}

void CRubikCtrl::ResetSolution() {
	list.Clear();
}

void CRubikCtrl::Solve() {
	if (solving) {
		solving=false;
	} else {
		if (list.IsEmpty()) 
			BuildList ();
		if (!list.IsEmpty())
			solving=true;
	}
	

	ProcessList ();
}
