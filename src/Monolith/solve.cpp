#include "../common/middefs.h"
#include <assert.h>
#include "main.h"
#include "ctrl_rubik.h"

CRubik cube1;

int blueside;

void CRubikCtrl::BringTopCorner (int n) {
	rubik_rot_t rrr;
	bool thisside=false;
	int opside=cube1.GetOpposite(blueside);
	int c_id=cube1.GetCorner(blueside, n);
	
	int iii=cube1.GetPlaneOnCornerId(c_id, blueside);
	
	int leftside=cube1.GetCornerPlane(c_id, (iii+2)%3),
		rightside=cube1.GetCornerPlane(c_id, (iii+1)%3);
	
	
	int place_on_rs=cube1.GetCornerInPlaneId(rightside, c_id);
	place_on_rs--;
	if (place_on_rs==-1)
		place_on_rs=3;
	
	int in_column=cube1.GetCorner(rightside, place_on_rs);
	
	int place_on_ls=cube1.GetCornerInPlaneId(leftside, in_column);
	
	rubik_loc_t loc;
	for (int actualplace=0; actualplace<8; actualplace++) {
		loc=cube1.Corner(actualplace);
		if (loc.c==c_id)
			break;
	}
	if (actualplace==c_id && loc.dir==0) {
		return;
	}
	for (int i=0; i<3; i++) {
		if (cube1.GetCornerPlane(actualplace, i)==blueside)
			thisside=true;
	}
	
	int p1, p2;
	bool dir;	
	
	if (thisside) {
		p1=cube1.GetCornerInPlaneId(blueside, actualplace);
		p2=n;
		if (p2>p1)
			dir=true;
		else
			dir=false;
		for (i=0; i<abs(p2-p1); i++) {
			rrr.plane=blueside;
			rrr.dir=dir;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
		
		rrr.plane=rightside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		for (i=0; i<abs(p2-p1); i++) {
			rrr.plane=blueside;
			rrr.dir=!dir;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
	} else {
		
		
		p1=cube1.GetCornerInPlaneId(opside, actualplace);
		p2=cube1.GetCornerInPlaneId(opside, in_column);
		
		if (p2!=p1) {
			if (p2>p1)
				dir=true;
			else
				dir=false;
			for (p1=abs(p2-p1); p1>0; p1--) {
				cube1.Rotate(opside, dir);
				rrr.plane=opside;
				rrr.dir=dir;
				list.AddLast(rrr);
			}
		}
	}
	
	if (cube1.GetColorCorner(leftside, place_on_ls)==blueside) {
		// Case 1
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
	} else if (cube1.GetColorCorner(rightside, place_on_rs)==blueside) {
		// Case 2
		rrr.plane=rightside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
	} else {
		// Case 3
		rrr.plane=rightside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=rightside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
	}
}
void CRubikCtrl::BringTopBridge (int n) {
	rubik_rot_t rrr;
	rubik_loc_t loc;
	
	int b_id=cube1.GetBridge(blueside, n);
	int op_side=cube1.GetOpposite(blueside);
	
	for (int actualplace=0; actualplace<12; actualplace++) {
		loc=cube1.Bridge(actualplace);
		if (loc.c==b_id)
			break;
	}
	
	if (actualplace==b_id && loc.dir==0)
		return;
	
	// Top Side
	bool isontop=false, isonbottom=false;
	for (int i=0; i<2; i++) {
		if (cube1.GetBridgePlane(actualplace, i)==blueside) {
			isontop=true;
			break;
		}
	}
	int p1, p2;
	if (isontop) {
		int face=cube1.GetBridgePlane(actualplace, !i);
		int actualn=cube1.GetBridgeInPlaneId(blueside, actualplace);
		
		int leftcorner=actualn+1;
		if (leftcorner==4) leftcorner=0;
		leftcorner=cube1.GetCorner(blueside, leftcorner);
		int rightcorner=cube1.GetCorner(blueside, actualn);
		int lefttrack = cube1.GetPlaneOnCornerId(leftcorner, face);
		lefttrack++;
		lefttrack%=3;
		lefttrack=cube1.GetCornerPlane(leftcorner, lefttrack);
		int righttrack = cube1.GetPlaneOnCornerId(rightcorner, blueside);
		righttrack++;
		righttrack%=3;
		righttrack=cube1.GetCornerPlane(rightcorner, righttrack);
		
		rrr.plane=lefttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=righttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=face;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=face;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=righttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		int tempn=(cube1.GetBridgeInPlaneId(face, actualplace)+2)%4;
		tempn=cube1.GetBridge (face, tempn);
		p1=cube1.GetBridgeInPlaneId (op_side, tempn);
		if (p1==-1)
			MessageBox (hMainWnd, "FUUUUCK!", "Monolith", MB_OK);
		
		isonbottom=true;
	} else {
		for (i=0; i<2; i++) {
			if (cube1.GetBridgePlane(actualplace, i)==op_side) {
				isonbottom=true;
				break;
			}
		}
	}
	
	if (isonbottom) {
		
		int face=cube1.GetBridgePlane(b_id, (cube1.GetPlaneOnBridgeId (b_id, blueside)+1)&0x01);
		int temp=cube1.GetBridgeInPlaneId(face, b_id);
		int column=cube1.GetBridge(face, (temp+2)&0x03);
		
		int leftcorner=n+1;
		if (leftcorner==4) leftcorner=0;
		leftcorner=cube1.GetCorner(blueside, leftcorner);
		int rightcorner=cube1.GetCorner(blueside, n);
		int lefttrack = cube1.GetPlaneOnCornerId(leftcorner, face);
		lefttrack++;
		lefttrack%=3;
		lefttrack=cube1.GetCornerPlane(leftcorner, lefttrack);
		int righttrack = cube1.GetPlaneOnCornerId(rightcorner, blueside);
		righttrack++;
		righttrack%=3;
		righttrack=cube1.GetCornerPlane(rightcorner, righttrack);
		
		
		
		
		if (!isontop)
			p1=cube1.GetBridgeInPlaneId(op_side, actualplace);
		p2=cube1.GetBridgeInPlaneId(op_side, column);
		if (p2!=p1) {
			
			rrr.plane=op_side;
			rrr.dir=(p2>p1);
			for (p1=abs(p2-p1); p1>0; p1--) {
				cube1.Rotate(rrr.plane, rrr.dir);
				list.AddLast(rrr);
			}
		}
		
		
		if (cube1.GetColorBridge (op_side, p2)==blueside) {
			// case 1
			rrr.plane=lefttrack;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=righttrack;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			
			rrr.plane=face;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=face;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			
			rrr.plane=lefttrack;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=righttrack;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		} else {
			// case 2
			
			rrr.plane=op_side;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=lefttrack;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=righttrack;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=face;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=lefttrack;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=righttrack;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
		
	} else {
		int side0, side, side1=cube1.GetBridgePlane(actualplace,0),side2=cube1.GetBridgePlane(actualplace,1);
		int a1=-1, a2=-1, a;
		for (i=0; i<4; i++) {
			if (cube1.GetPlaneOnBridgeId(cube1.GetBridge(blueside,i), side1)!=-1) 
				a1=i;
			if (cube1.GetPlaneOnBridgeId(cube1.GetBridge(blueside,i), side2)!=-1)
				a2=i;
		}
		if (a1==(a2+1)%4) {
			side=side1;
			side0=side2;
			a=a1;
		} else {
			side=side2;
			side0=side1;
			a=a2;
		}
		
		int rot_done=0;
		if (n!=a) {
			rot_done=abs(n-a);
			rrr.dir=(n<a);
			rrr.plane=blueside;
			
			for (i=0; i<rot_done; i++) {
				cube1.Rotate(rrr.plane, rrr.dir);
				list.AddLast(rrr);
			}
		}
		
		int tempb=(cube1.GetBridgeInPlaneId(side, actualplace));
		tempb=(tempb+2)%4;
		int tempb1=cube1.GetBridge(side, tempb);
		
		int tempp=(cube1.GetPlaneOnBridgeId(tempb1, side));
		tempp=(tempp+1)%2;
		tempp=cube1.GetBridgePlane(tempb1, tempp);
		
		int side3=tempp;
		
		if (cube1.GetColorBridge (side, cube1.GetBridgeInPlaneId(side, actualplace)) == blueside ) {
			
			rrr.plane=blueside;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=op_side;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=side3;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=blueside;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			rrr.plane=blueside;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=op_side;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			rrr.plane=op_side;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=side0;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=blueside;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=op_side;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		} else {
			//case 2
			rrr.plane=blueside;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=op_side;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			
			rrr.plane=side3;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=blueside;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			rrr.plane=op_side;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
			
			rrr.plane=side;
			rrr.dir=false;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
		
		
		rrr.dir=!(n<a);
		rrr.plane=blueside;
		for (i=0; i<rot_done; i++) {
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
		
	}
	
}
void CRubikCtrl::BringBelt (int n) {
	int temp=cube1.GetCorner(blueside, n);
	int vvv=cube1.GetPlaneOnCornerId(temp, blueside);
	vvv=(vvv+1)%3;
	int prevtempplane=cube1.GetCornerPlane(temp, vvv);
	vvv=(vvv+1)%3;
	int tempplane=cube1.GetCornerPlane(temp, vvv);
	int b_n=cube1.GetCornerInPlaneId(tempplane, temp);
	
	int b_id=cube1.GetBridge(tempplane, b_n);
	int op_side=cube1.GetOpposite(blueside);
	
	rubik_loc_t loc;
	rubik_rot_t rrr;
	for (int actualplace=0; actualplace<12; actualplace++) {
		loc=cube1.Bridge(actualplace);
		if (loc.c==b_id)
			break;
	}
	
	if (actualplace==b_id && !loc.dir)
		return;
	
	bool flag=false;
	int p1, p2;
	p1=cube1.GetBridgeInPlaneId(op_side, actualplace);
	temp=cube1.GetBridge(tempplane, (b_n+1)%4);
	if (p1 != -1) {
		// Everything's OK
		p2=cube1.GetBridgeInPlaneId(op_side, temp);
		if (p2==-1) {
		}
		if (p1!=p2) {
			rrr.plane=op_side;
			rrr.dir=(p2>p1);
			for (int i=0; i<abs(p2-p1); i++) {
				cube1.Rotate(rrr.plane, rrr.dir);
				list.AddLast(rrr);
			}
		}
		
		if (cube1.GetColorBridge(tempplane, (b_n+1)%4)!=tempplane) {
			int s_t=tempplane; tempplane=prevtempplane;
			prevtempplane=s_t; 
			flag=true;
			
			rrr.plane=op_side;
			rrr.dir=true;
			cube1.Rotate(rrr.plane, rrr.dir);
			list.AddLast(rrr);
		}
		
	} else {
		tempplane=cube1.GetBridgePlane(actualplace, 0);
		prevtempplane=cube1.GetBridgePlane(actualplace, 1);
		int tempbridge=cube1.GetBridgeInPlaneId(tempplane, actualplace);
		tempbridge=(tempbridge+1)%4;
		tempbridge=cube1.GetBridge(tempplane, tempbridge);
		flag= (cube1.GetBridgeInPlaneId (blueside, tempbridge) !=-1);
		
	}
	
	rrr.plane=op_side;
	rrr.dir=flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=prevtempplane;
	rrr.dir=flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=op_side;
	rrr.dir=!flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=prevtempplane;
	rrr.dir=!flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=op_side;
	rrr.dir=!flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=tempplane;
	rrr.dir=!flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=op_side;
	rrr.dir=flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=tempplane;
	rrr.dir=flag;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	if (p1==-1)
		BringBelt(n);
	
}

int mm (int a, int b, int m) {
	int res=a-b;
	while (res<0) res+=m;
	return res;
}
void CRubikCtrl::BottomCorners () {
	int tempbridge=cube1.GetBridge(blueside,0);
	int orangeside=cube1.GetBridgePlane(tempbridge, (cube1.GetPlaneOnBridgeId(tempbridge, blueside)+1)%2),
		redside=cube1.GetOpposite(orangeside),
		opside=cube1.GetOpposite(blueside),
		lefttrack, righttrack;
	
	
	int n1=-1, n2=-1, temp, c_temp, actualn1=-1, actualn2=-1;
	int id1=-1, id2=-1;
	
	for (int i=0; i<4; i++) {
		c_temp=cube1.GetCorner(opside, i);
		temp=cube1.GetPlaneOnCornerId(c_temp, orangeside);
		if (temp != -1) {
			if (cube1.GetCornerPlane(c_temp, (temp+1)%3)==opside) {
				n1=i;
				righttrack=cube1.GetCornerPlane(c_temp, (temp+2)%3);
			} else {
				n2=i;
				lefttrack=cube1.GetCornerPlane(c_temp, (temp+1)%3);
			}
		}
	}
	id1=cube1.GetCorner(opside, n1);
	id2=cube1.GetCorner(opside, n2);
	
	rubik_loc_t loc;
	for (i=0; i<4; i++) {
		loc=cube1.Corner (cube1.GetCorner(opside, i));
		if (loc.c==id1)
			actualn1=i;
		else if (loc.c==id2)
			actualn2=i;
	}
	
	temp=mm(actualn2,actualn1,4);
	int rots;
	
	if (temp==2) {
		// Diagonal
		rots=mm(n2,actualn2,4);
	} else {
		// Adjacent
		if (temp==1) {
			// Right order
			rots=mm(n2,actualn2,4);
		} else {
			// Wrong order
			rots=mm(n1,actualn2,4);
		}
	}
	rubik_rot_t rrr;
	rrr.plane=opside;
	rrr.dir=true;
	for (i=0; i<rots; i++) {
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
	}
	
	if (temp==2) { 
		// De-diagonalize
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=orangeside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=orangeside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
	} else if (temp==3) {
		//Change order
		rrr.plane=lefttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=orangeside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=orangeside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=lefttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
	}
	c_temp=cube1.GetCorner(opside, (n2+1) % 4);
	loc=cube1.Corner(c_temp);
	if (loc.c!=c_temp) {
		//Change order
		rrr.plane=righttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=righttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=redside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=redside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=righttrack;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=righttrack;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
	}
}

bool CRubikCtrl::FC_CheckFinished() {
	int opside=cube1.GetOpposite(blueside);
	int c;
	rubik_loc_t loc;
	for (int i=0; i<4; i++) {
		c=cube1.GetCorner(opside, i);
		loc=cube1.Corner (c);
		if (loc.c!=c || loc.dir!=0)
			return false;
	}
	return true;
}

int CRubikCtrl::FC_CheckSeven(int n) {
	int opside=cube1.GetOpposite(blueside);
	int b_temp=cube1.GetBridge(opside, n);
	int face=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	b_temp=cube1.GetBridge(opside, (n+3)%4);
	int right=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	
	int thecorner=cube1.GetCorner(opside, n);
	int c, score;
	
	// Condition 1
	score=0;
	c=cube1.GetCornerInPlaneId(face, thecorner);
	if (cube1.GetColorCorner(face, c)==opside)
		score++;
	if (cube1.GetColorCorner(face, (c+3)%4)==opside)
		score++;
	if (cube1.GetColorCorner(opside, (n+2)%4)==opside)
		score++;
	if (cube1.GetColorCorner(opside, (n+3)%4)==opside)
		score++;
	
	if (score==4)
		return 1;
	
	// Condition 2
	score=0;
	c=cube1.GetCornerInPlaneId(right, thecorner);
	if (cube1.GetColorCorner(right, c)==opside)
		score++;
	if (cube1.GetColorCorner(right, (c+1)%4)==opside)
		score++;
	
	if (score==2)
		return 2;
	
		/*
		// Condition 3 -- DO NOT RESET THE SCORE!
		c=cube1.GetCornerInPlaneId(face, thecorner);
		if (cube1.GetColorCorner(face, (c+3)%4)==opside)
		score++;
		
		  if (score==3)
	return 3;*/
	
	// Condition 4
	score=0;
	if (cube1.GetColorCorner(opside, (n+1)%4)==opside)
		score++;
	if (cube1.GetColorCorner(opside, (n+2)%4)==opside)
		score++;
	c=cube1.GetCornerInPlaneId(face, thecorner);
	if (cube1.GetColorCorner(face, c)==opside)
		score++;
	
	if (score==3)
		return 4;
	
	// Condition 5
	score=0;
	if (cube1.GetColorCorner(opside, n)==opside)
		score++;
	c=cube1.GetCornerInPlaneId(right, thecorner);
	if (cube1.GetColorCorner(right, (c+1)%4)==opside)
		score++;
	
	if (score==2)
		return 5;
	
	// Condition 6
	score=0;
	if (cube1.GetColorCorner(opside, n)==opside)
		score++;
	c=cube1.GetCornerInPlaneId(face, thecorner);
	if (cube1.GetColorCorner(face, (c+3)%4)==opside)
		score++;
	
	if (score==2)
		return 6;
	
	// Condition 7
	score=0;
	if (cube1.GetColorCorner(opside, (n+1)%4)==opside)
		score++;
	if (cube1.GetColorCorner(opside, (n+3)%4)==opside)
		score++;
	c=cube1.GetCornerInPlaneId(face, thecorner);
	if (cube1.GetColorCorner(face, c)==opside)
		score++;
	
	if (score==3)
		return 7;
	
	return 0;
}

void CRubikCtrl::FinishCorners () {
	int opside=cube1.GetOpposite(blueside);
	
	while (!FC_CheckFinished()) {
		for (int n=0; (n<4 && !FC_CheckSeven(n)); n++) {};
		
		if (n==4) {
			MessageBox (hMainWnd, "AAA", "AAA", MB_OK | MB_ICONERROR);
			//cube1.Review();
			return;
		}
		
		int b_temp=cube1.GetBridge(opside, n);
		int face=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
		b_temp=cube1.GetBridge(opside, (n+1)%4);
		int left=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
		rubik_rot_t rrr;
		
		rrr.plane=left;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
	}
}

bool CRubikCtrl::FB_CheckFinished() {
	int opside=cube1.GetOpposite(blueside);
	int c;
	rubik_loc_t loc;
	for (int i=0; i<4; i++) {
		c=cube1.GetBridge(opside, i);
		loc=cube1.Bridge (c);
		if (loc.c!=c)
			return false;
	}
	return true;
}

void CRubikCtrl::BottomBridges () {
	int counter=0;
	while (!FB_CheckFinished() && counter<4) {
		
		int opside=cube1.GetOpposite(blueside);
		int c;
		rubik_loc_t loc;
		for (int i=0; i<4; i++) {
			c=cube1.GetBridge(opside, i);
			loc=cube1.Bridge (c);
			if (loc.c==c)
				break;
		}
		if (i==4) {
			time_to_die=true;
			return;
		}
		
		int b_temp=cube1.GetBridge(opside, i);
		int face=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
		b_temp=cube1.GetBridge(opside, (i+3)%4);
		int right=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
		b_temp=cube1.GetBridge(opside, (i+1)%4);
		int left=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
		
		int back=cube1.GetOpposite(face);
		rubik_rot_t rrr;
		
		rrr.plane=left;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=right;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=face;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=right;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=opside;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=right;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=face;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		
		rrr.plane=left;
		rrr.dir=false;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		rrr.plane=right;
		rrr.dir=true;
		cube1.Rotate(rrr.plane, rrr.dir);
		list.AddLast(rrr);
		counter++;
	}
	if (counter>=4) {
		time_to_die=true;
		return;
	}
}

void CRubikCtrl::Fish (int n) {
	int opside=cube1.GetOpposite(blueside);
	int b_temp=cube1.GetBridge(opside, n);
	int face=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	b_temp=cube1.GetBridge(opside, (n+3)%4);
	int right=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	int back=cube1.GetOpposite(face);
	int left=cube1.GetOpposite(right);
	rubik_rot_t rrr;
	
	rrr.plane=face;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=left;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=right;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=face;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=face;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=right;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=left;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=face;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

}

void CRubikCtrl::HHH (int n) {
	int opside=cube1.GetOpposite(blueside);
	int b_temp=cube1.GetBridge(opside, n);
	int face=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	b_temp=cube1.GetBridge(opside, (n+3)%4);
	int right=cube1.GetBridgePlane(b_temp,   (cube1.GetPlaneOnBridgeId(b_temp, opside)+1)%2);
	int back=cube1.GetOpposite(face);
	int left=cube1.GetOpposite(right);
	rubik_rot_t rrr;
	
	rrr.plane=right;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);


	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=face;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	
	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=face;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=back;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=blueside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=right;
	rrr.dir=true;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);

	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
	rrr.plane=opside;
	rrr.dir=false;
	cube1.Rotate(rrr.plane, rrr.dir);
	list.AddLast(rrr);
}

void CRubikCtrl::FinishBridges() {
	int opside=cube1.GetOpposite(blueside);
	int counter=0;
	int b;
	rubik_loc_t loc;
	int places[4];
	for (int n=0; n<4; n++) {
		b=cube1.GetBridge(opside, n);
		loc=cube1.Bridge(b);
		if (loc.dir!=0) {
			places[counter]=n;
			counter++;
		}
	}
	if (counter==0) {
		return;
	}

	if (counter=2) {
		int temp=mm(places[0], places[1], 4);
		if (temp==1) {
			
			Fish (places[0]);
		}
		else if (temp==3) {
			Fish (places[1]);
		}
		else if (temp==2)
			HHH ((places[0]+1)%4);
	}

}

void CRubikCtrl::BuildList() {
	blueside=plane_to_rot;
	rubik_rot_t r;
	cube1.Copy(cube);
	for (int i=0; i<4; i++)
		BringTopCorner (i);
	for (i=0; i<4; i++)
		BringTopBridge (i);
	for (i=0; i<4; i++)
		BringBelt (i);
	
	BottomCorners ();
	FinishCorners ();
	BottomBridges ();
	FinishBridges ();

	if (!cube1.Won())
		FinishBridges ();
	
	/*list.Clear();
	cube.Copy(cube1);*/
}

