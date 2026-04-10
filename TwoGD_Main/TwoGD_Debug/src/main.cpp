#include"twogd.h"
#include <chrono>

#define MSPEED 0.2f
#define VSPEED 0.1f
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
VMAP v_Vmap;

WORLD o_Wrld;
FONTHANDLER o_fhandler;

int Demo_DrawGrid() 
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

UCHAR DemoVignetteShader(camera *o_Cam, V3 *v_Vertex, V2 *v_Point, COLOR *c_Color)
{
	float f_Dis = v_Point->Distance(V2(o_Cam->i_Dimensions[0] / 2, o_Cam->i_Dimensions[1] / 2));
	float f_Max = sqrt(
		(o_Cam->i_Dimensions[0] / 2.0f)*(o_Cam->i_Dimensions[0] / 2.0f)+
		(o_Cam->i_Dimensions[1] / 2.0f)*(o_Cam->i_Dimensions[1] / 2.0f));
	float f_scale = ((f_Dis*0.5f) / f_Max);

	c_Color->c_Color[0] = (UCHAR)((float)c_Color->c_Color[0] * (1.0f - f_scale));
	c_Color->c_Color[1] = (UCHAR)((float)c_Color->c_Color[1] * (1.0f - f_scale));
	c_Color->c_Color[2] = (UCHAR)((float)c_Color->c_Color[2] * (1.0f - f_scale));

	if (f_scale > 1.0f)
	{
		c_Color->c_Color[0] = 0;
		c_Color->c_Color[1] = 0;
		c_Color->c_Color[2] = 0;
	}
	return GD_TASK_OKAY;
}


void MouseScroll(BOOL b_Up,POINT v_ScrollPoint) 
{
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

void gdCreateWinExec(GDWIN* o_win)
{
	o_win->i_Width = WINDOW_WIDTH * WINDOW_SIZE;
	o_win->i_Height = WINDOW_HEIGHT * WINDOW_SIZE;
	o_win->v_pLeftMouseDown = LeftMouseDown;
	o_win->v_pLeftMouseUp = LeftMouseUp;
	o_win->v_pRightMouseDown = RightMouseDown;
	o_win->v_pRightMouseUp = RightMouseUp;
	o_win->v_pMouseScroll = MouseScroll;
	o_win->c_WinTitle = (wchar_t*)L"Demo Window";
	o_win->i_XPos = 0;
	o_win->i_YPos = 0;
	o_win->dw_Style = (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	o_win->i_CmdShow = SW_MAXIMIZE;
}

unsigned char  gdMain(GDWIN* o_win)
{
	o_Console.Create();
	
	RECT o_rect;
	GetWindowRect(o_win->hd_WindowHandle,&o_rect);
	o_win->i_Width = o_rect.left + o_rect.right;
	o_win->i_Height = o_rect.top + o_rect.bottom;
	
	o_Cam = CAM3D(
		(FLOAT)0.01f,
		(FLOAT)250.0,
		(UINT32)o_win->i_Width,
		(UINT32)o_win->i_Height,
		(FLOAT)2.0f,
		V3(0, 2, 0),
		V3(0, 0, 0)
	);
	//o_Cam.s_Shader = DemoVignetteShader;

	o_CamCtrlr = CAMCTRLR(
		&o_Cam,
		&o_3DCodec
	);


	o_Img.Prepare(o_win->i_Width, o_win->i_Height);

	o_2DCodec = CODEC2D(&o_Img);
	o_3DCodec = CODEC3D(&o_Img, &o_Cam);
	o_fhandler = FONTHANDLER(&o_2DCodec, (const LPSTR)"C:\\Users\\PyroJunkie\\source\\repos\\inimodo\\TwoGD\\TwoGD_Main\\TwoGD_Main\\font\\vmf\\*");
	o_fhandler.i_Padding = 10;
	o_Wrld = WORLD(&o_3DCodec);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.objf", co_Red);
	o_Wrld.o_Layers[0].o_Obj.v_Anchor = V3(7, 1, 0);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.objf", co_Blue);
	o_Wrld.o_Layers[1].o_Obj.v_Anchor = V3(-7, 1, 0);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.objf", co_Green);
	o_Wrld.o_Layers[2].o_Obj.v_Anchor = V3(0, 1, 7);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.objf", co_Pink);
	o_Wrld.o_Layers[3].o_Obj.v_Anchor = V3(0, 1, -7);


	return TRUE;
}

DWORD*  gdUpdate(GDWIN * o_win)
{
	
	static int i_Counter = 0;
	static long long i_AvrTime = 0;
	static long long i_Time = 0;
	if (i_Counter > 5)
	{
		i_AvrTime = (int)((float)i_Time / (float)5);
		i_Time = 0;
		i_Counter = 0;
	}
	i_Counter++;
	//BEGIN RENDER
	auto a_TimeA = std::chrono::high_resolution_clock::now();
	

	o_Img.CleanBuffer();
	//o_fhandler.Write(V2(10,10),0.5f,"ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 _!$%&/()=?\n");
	//o_fhandler.Write(V2(10, 10), 0.5f, "%d ms\n", i_AvrTime);
	o_CamCtrlr.UpdateCamCtrlr(o_win);
	o_CamCtrlr.DrawCrosshair();

	int i_CountedEdges = Demo_DrawGrid();

	o_Wrld.Render();

	auto a_TimeB = std::chrono::high_resolution_clock::now();
	//END RENDER
	i_Time += std::chrono::duration_cast<std::chrono::milliseconds>(a_TimeB - a_TimeA).count();
	system("cls");
	printf("Render Took: %d ms (%.1f FPS)\nHas Focus: %d\nDrawn Edges: %d\n", i_AvrTime,(1.0f/ (i_AvrTime/1000.0f)), o_win->b_HasFocus, i_CountedEdges);
	return o_Img.d_pOutputStream;
}

void  gdClose() {
	o_Img.Dispose();
}
