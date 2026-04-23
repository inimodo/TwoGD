#include"twogd.h"

#define WINDOW_SIZE 100
#define WINDOW_WIDTH 16
#define WINDOW_HEIGHT 9
#define GRID_SIZE 50

CODEC3D o_3DCodec;
CODEC2D o_2DCodec;
CONSOLE o_Console;
CANVAS o_Img;
CAM3D o_Cam;
CAMCTRLR o_CamCtrlr;
WORLD o_Wrld;
FONTHANDLER o_fhandler;
PERLOG o_Perlog;
float f_Size = 100;
int DrawGrid() 
{
	int i_EdgeCount = 3;
	V3 v_PointA = V3(0,0,0),v_PointB= V3(0,0,0);
	float f_TempX= -GRID_SIZE, f_TempY= -GRID_SIZE;
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		for (int i_Y = -GRID_SIZE; i_Y < GRID_SIZE; i_Y++)
		{
			v_PointA.f_Pos[X] = i_X;
			v_PointA.f_Pos[Z] = i_Y+1;
			v_PointB.f_Pos[X] = i_X;
			v_PointB.f_Pos[Z] = i_Y;
			o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray,PF_OVERWRITE_ALLOWED,100);
			v_PointB.f_Pos[X]++;
			v_PointB.f_Pos[Z]++;
			o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray, PF_OVERWRITE_ALLOWED, 100);
			i_EdgeCount += 2;
		}
	}
	v_PointA = V3(0,0, -(GRID_SIZE));
	v_PointB = V3(0,0, -(GRID_SIZE));

	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[X] = i_X;
		v_PointB.f_Pos[X] = i_X+1;
		o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray, PF_OVERWRITE_ALLOWED, 100);
		i_EdgeCount += 1;
	}
	v_PointA = V3((GRID_SIZE), 0, 0);
	v_PointB = V3((GRID_SIZE), 0, 0);
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[Z] = i_X;
		v_PointB.f_Pos[Z] = i_X + 1;
		o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray, PF_OVERWRITE_ALLOWED, 100);
		i_EdgeCount += 1;
	}

	V3 vec_Null = V3(0.0f, 0.0f, 0.0f);
	V3 vec_UnitvX = V3(1.0f, 0.0f, 0.0f);
	V3 vec_UnitvY = V3(0.0f, 1.0f, 0.0f);
	V3 vec_UnitvZ = V3(0.0f, 0.0f, 1.0f);

	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvX, (COLOR*)&co_Red,PF_OVERWRITE_ALLOWED,99);
	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvY, (COLOR*)&co_Green, PF_OVERWRITE_ALLOWED, 99);
	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvZ, (COLOR*)&co_Blue, PF_OVERWRITE_ALLOWED, 99);

	return i_EdgeCount;
}

void MouseScroll(BOOL b_Up,POINT v_ScrollPoint) 
{
	if (b_Up)f_Size += 2;
	if (!b_Up)f_Size -= 2;
	printf("%d %d %d\n",v_ScrollPoint.x, v_ScrollPoint.y,b_Up);
}
void LeftMouseDown(POINT v_ClickPoint)
{
	printf("%d %d\n", v_ClickPoint.x, v_ClickPoint.y);
}
void LeftMouseUp(POINT v_ClickPoint)
{
	printf("%d %d\n", v_ClickPoint.x, v_ClickPoint.y);
}
void RightMouseDown(POINT v_ClickPoint)
{
	printf("%d %d\n", v_ClickPoint.x, v_ClickPoint.y);
}
void RightMouseUp(POINT v_ClickPoint)
{
	printf("%d %d\n", v_ClickPoint.x, v_ClickPoint.y);
}

void gdCreateWinExec(WIN* o_Win)
{
	o_Win->i_Width = WINDOW_WIDTH * WINDOW_SIZE;
	o_Win->i_Height = WINDOW_HEIGHT * WINDOW_SIZE;
	o_Win->v_pLeftMouseDown = LeftMouseDown;
	o_Win->v_pLeftMouseUp = LeftMouseUp;
	o_Win->v_pRightMouseDown = RightMouseDown;
	o_Win->v_pRightMouseUp = RightMouseUp;
	o_Win->v_pMouseScroll = MouseScroll;
	o_Win->c_WinTitle = (wchar_t*)L"Demo Window";
	o_Win->i_XPos = 0;
	o_Win->i_YPos = 0;
	o_Win->dw_Style = (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	o_Win->i_CmdShow = SW_MAXIMIZE;
}

unsigned char  gdMain(WIN* o_Win)
{
	o_Console.Create();
	
	RECT o_rect;
	GetWindowRect(o_Win->hd_WindowHandle,&o_rect);
	o_Win->i_Width = o_rect.left + o_rect.right;
	o_Win->i_Height = o_rect.top + o_rect.bottom;
	
	o_Cam = CAM3D(
		(FLOAT)0.01f,
		(FLOAT)250.0,
		(uint32_t)o_Win->i_Width,
		(uint32_t)o_Win->i_Height,
		(FLOAT)2.0f,
		V3(0, 2, 0),
		V3(0, 0, 0)
	);

	o_CamCtrlr = CAMCTRLR(&o_Cam, &o_3DCodec);
	o_CamCtrlr.f_MoveSpeed = 0.4f;
	o_CamCtrlr.f_ViewSpeed = 0.1f;
	o_Img.Prepare(o_Win->i_Width, o_Win->i_Height);
	o_2DCodec = CODEC2D(&o_Img);
	o_3DCodec = CODEC3D(&o_Img, &o_Cam);
	o_fhandler = FONTHANDLER(&o_2DCodec, (const LPSTR)"font\\font.ttf");
	o_Perlog = PERLOG(10);
	o_Wrld = WORLD(&o_3DCodec);
	o_Wrld.AppendLayer((const LPSTR)"3dobj\\obj2.obj", co_Red);
	o_Wrld.o_Layers[0].o_Obj.v_Anchor = V3(10, 1, 0);
	o_Wrld.AppendLayer((const LPSTR)"3dobj\\obj3.obj", co_Blue);
	o_Wrld.o_Layers[1].o_Obj.v_Anchor = V3(-10, 1, 0);
	o_Wrld.AppendLayer((const LPSTR)"3dobj\\obj2.obj", co_Green);
	o_Wrld.o_Layers[2].o_Obj.v_Anchor = V3(0, 1, 10);
	o_Wrld.AppendLayer((const LPSTR)"3dobj\\obj2.obj", co_Pink);
	o_Wrld.o_Layers[3].o_Obj.v_Anchor = V3(0, 1, -10);

	return TRUE;
}

DWORD*  gdUpdate(WIN * o_Win)
{
	o_Img.CleanBuffer();
	o_Perlog.Start();
	o_fhandler.Write(V2(10, 30), 25, "%.0fms\n", o_Perlog.GetDelta());
	o_CamCtrlr.UpdateCamCtrlr(o_Win);
	o_CamCtrlr.DrawCrosshair();
	DrawGrid();
	o_Wrld.Render();
	o_Perlog.Stop();
	return o_Img.d_pOutputStream;
}

void  gdClose() 
{
	o_Img.Dispose();
	o_Wrld.Dispose();
	o_Perlog.Dispose();
	o_fhandler.Dispose();
}
