#include"twogd.h"
#include <chrono>

CONSOLE o_Console;
CODEC2D o_Codec;
CANVAS o_Img;

CODEC3D o_World;

VMAP vmf_Map;
OBJ3D obj_Monkey;
OBJ3D obj_Cube;
OBJ3D obj_Plane;
CAM3D o_camera;


#define MSPEED 0.1f
#define VSPEED 0.05f
#define WINDOW_SIZE 90
#define WINDOW_WIDTH 16
#define WINDOW_HEIGHT 9
#define GRID_SPACING 1
#define GRID_SIZE 15


void Demo_DrawGrid() 
{
	V3 vec_Null = V3(0.0f, 0.0f, 0.0f);
	V3 vec_UnitvX = V3(1.0f, 0.0f, 0.0f);
	V3 vec_UnitvY = V3(0.0f, 1.0f, 0.0f);
	V3 vec_UnitvZ = V3(0.0f, 0.0f, 1.0f);

	o_World.DrawEdge(&vec_Null, &vec_UnitvX, &co_Red);
	o_World.DrawEdge(&vec_Null, &vec_UnitvY, &co_Green);
	o_World.DrawEdge(&vec_Null, &vec_UnitvZ, &co_Blue);

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
			o_World.DrawEdge(&v_PointA, &v_PointB, &co_Gray);
			v_PointB.f_Pos[X]++;
			v_PointB.f_Pos[Z]++;
			o_World.DrawEdge(&v_PointA, &v_PointB, &co_Gray);
		}
	}
	v_PointA = V3(0,0, -(GRID_SIZE));
	v_PointB = V3(0,0, -(GRID_SIZE));

	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[X] = i_X;
		v_PointB.f_Pos[X] = i_X+1;
		o_World.DrawEdge(&v_PointA, &v_PointB, &co_Gray);
	}
	v_PointA = V3((GRID_SIZE), 0, 0);
	v_PointB = V3((GRID_SIZE), 0, 0);
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		v_PointA.f_Pos[Z] = i_X;
		v_PointB.f_Pos[Z] = i_X + 1;
		o_World.DrawEdge(&v_PointA, &v_PointB, &co_Gray);
	}
}

void Demo_DrawColoredCubes() 
{
	obj_Cube.v_Anchor.f_Pos[X] = 0;
	obj_Cube.v_Anchor.f_Pos[Z] = 7;
	o_World.DrawObject(&obj_Cube, &co_Blue);
	
	obj_Cube.v_Anchor.f_Pos[1] = 1;

	obj_Cube.v_Anchor.f_Pos[X] = 7;
	obj_Cube.v_Anchor.f_Pos[Z] = 0;
	o_World.DrawObject(&obj_Cube, &co_Red);

	obj_Cube.v_Anchor.f_Pos[X] = -7;
	obj_Cube.v_Anchor.f_Pos[Z] = 0;
	o_World.DrawObject(&obj_Cube, &co_Green);

	obj_Cube.v_Anchor.f_Pos[X] = 0;
	obj_Cube.v_Anchor.f_Pos[Z] = -7;
	o_World.DrawObject(&obj_Cube, &co_Pink);
}

void Demo_CursorLaser(win::GDWIN* o_win) 
{
	V3 v_Angle;
	V2 v_Cpos = _PTOP(o_win->v_CursorPos);
	o_camera.Relate(&v_Cpos, &v_Angle);
	V3 v_LaserStart = V3(0, 2, 0);
	V3 v_LaserStop  = V3(0, 0, 50);
	V3 v_Offset  = V3(0, 1, 0 );
	v_LaserStart = v_LaserStart + o_World.o_Camera->i_Position;
	v_LaserStop.RotateThis(v_Angle);
	v_LaserStop.RotateThis(o_World.o_Camera->i_Rotation);

	o_World.DrawEdge(&v_LaserStart,&v_LaserStop,&co_Red);
	
	system("cls");
	printf("%f %f\n",v_Angle.f_Pos[X], v_Angle.f_Pos[1]);
}



void prefabLoader() 
{
	printf("RES: %X \n", obj_Cube.Read((const LPSTR)"src\\obj\\obj2.obj"));
	printf("RES: %X \n", obj_Monkey.Read((const LPSTR)"src\\obj\\obj3.obj"));
	printf("RES: %X \n", obj_Plane.Read((const LPSTR)"src\\obj\\obj4.obj"));
	printf("RES: %X \n", vmf_Map.Read((const LPSTR)"src\\obj\\obj1.vmf"));
}
unsigned char  gdMain(win::GDWIN * o_win)
{
	o_Console.Create();

	o_win->i_Width = WINDOW_WIDTH * WINDOW_SIZE;
	o_win->i_Height = WINDOW_HEIGHT * WINDOW_SIZE;

	o_camera = CAM3D(
		(FLOAT)0.1, 
		(FLOAT)100.0 , 
		(UINT32)o_win->i_Width ,
		(UINT32)o_win->i_Height , 
		(FLOAT)2.0f, 
		V3(0, 0, 0), 
		V3(0, 0, 0)
	);

	o_Img.Prepare(o_win->i_Width, o_win->i_Height);

	o_Codec = CODEC2D(&o_Img);
	o_World = CODEC3D(&o_Img,&o_camera);

	prefabLoader();

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
	BasicCameraController(o_win, &o_camera, MSPEED, VSPEED, TRUE, 2.0);

	Demo_DrawColoredCubes();
	//Demo_CursorLaser(o_win);
	Demo_DrawGrid();

	DrawCrosshair(&o_World, co_White, CHST_AXIS,0.2f);

	//END RENDER
	auto a_TimeB = std::chrono::high_resolution_clock::now();
	long long i_Time = std::chrono::duration_cast<std::chrono::milliseconds>(a_TimeB - a_TimeA).count();

	if (i_Counter == 1)
	{
		printf("%dll ms Aktive:%d\n", i_Time,o_win->b_HasFocus);
	}
	return o_Img.d_pOutputStream;
}
void  gdClose() {
	o_Img.Dispose();
	vmf_Map.Dispose();
}

