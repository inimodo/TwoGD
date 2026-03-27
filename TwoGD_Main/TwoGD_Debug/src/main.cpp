#include"twogd.h"
#include <chrono>

#define MSPEED 0.1f
#define VSPEED 0.05f
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


void Demo_DrawGrid() 
{
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
		}
	}
	v_PointA = V3(0,0, -(GRID_SIZE));
	v_PointB = V3(0,0, -(GRID_SIZE));

	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[X] = i_X;
		v_PointB.f_Pos[X] = i_X+1;
		o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray, PF_OVERWRITE_ALLOWED, 100);
	}
	v_PointA = V3((GRID_SIZE), 0, 0);
	v_PointB = V3((GRID_SIZE), 0, 0);
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[Z] = i_X;
		v_PointB.f_Pos[Z] = i_X + 1;
		o_3DCodec.DrawEdge(&v_PointA, &v_PointB, (COLOR*)&co_Gray, PF_OVERWRITE_ALLOWED, 100);
	}

	V3 vec_Null = V3(0.0f, 0.0f, 0.0f);
	V3 vec_UnitvX = V3(1.0f, 0.0f, 0.0f);
	V3 vec_UnitvY = V3(0.0f, 1.0f, 0.0f);
	V3 vec_UnitvZ = V3(0.0f, 0.0f, 1.0f);

	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvX, (COLOR*)&co_Red,PF_OVERWRITE_ALLOWED,99);
	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvY, (COLOR*)&co_Green, PF_OVERWRITE_ALLOWED, 99);
	o_3DCodec.DrawEdge(&vec_Null, &vec_UnitvZ, (COLOR*)&co_Blue, PF_OVERWRITE_ALLOWED, 99);
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


unsigned char  gdMain(win::GDWIN * o_win)
{
	o_Console.Create();

	o_win->i_Width = WINDOW_WIDTH * WINDOW_SIZE;
	o_win->i_Height = WINDOW_HEIGHT * WINDOW_SIZE;
	o_win->v_pLeftMouseDown = LeftMouseDown;
	o_win->v_pLeftMouseUp = LeftMouseUp;
	o_win->v_pRightMouseDown = RightMouseDown;
	o_win->v_pRightMouseUp = RightMouseUp;
	o_win->v_pMouseScroll = MouseScroll;
	o_win->c_WinTitle = (wchar_t*)L"DEMOAPP";

	o_Cam = CAM3D(
		(FLOAT)0.1,
		(FLOAT)150.0,
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

	o_Wrld = WORLD(&o_3DCodec);
	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.obj", co_Red);
	o_Wrld.o_Layers[0].o_Obj.v_Anchor = V3(7, 1, 0);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.obj", co_Blue);
	o_Wrld.o_Layers[1].o_Obj.v_Anchor = V3(-7, 1, 0);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.obj", co_Green);
	o_Wrld.o_Layers[2].o_Obj.v_Anchor = V3(0, 1, 7);

	o_Wrld.AppendLayer((const LPSTR)"src\\obj\\obj2.obj", co_Pink);
	o_Wrld.o_Layers[3].o_Obj.v_Anchor = V3(0, 1, -7);

	v_Vmap.Read((const LPSTR)"font\\H.vmf");
	v_Vmap.f_Scale = 10;

	return TRUE;
}

DWORD*  gdUpdate(win::GDWIN * o_win)
{
	static int i_Counter = 0;
	i_Counter++;
	i_Counter %= 10;
	auto a_TimeA = std::chrono::high_resolution_clock::now();
	
	//BEGIN RENDER

	o_Img.CleanBuffer();
	o_CamCtrlr.UpdateCamCtrlr(o_win);
	o_CamCtrlr.DrawCrosshair();

	Demo_DrawGrid();

	o_Wrld.Render();
	o_3DCodec.DrawVMap(&v_Vmap);

	//END RENDER
	auto a_TimeB = std::chrono::high_resolution_clock::now();
	long long i_Time = std::chrono::duration_cast<std::chrono::milliseconds>(a_TimeB - a_TimeA).count();

	if (i_Counter == 1)
	{
		//printf("%d ms Aktive:%d\n", i_Time,o_win->b_HasFocus);
	}
	return o_Img.d_pOutputStream;
}

void  gdClose() {
	o_Img.Dispose();
}
