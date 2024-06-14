#include"twogd.h"
#include <chrono>

GDCONSOLE gd_Console;
GD2DCODEC gd_Codec;
GDCANVAS gd_Img;

GD3DCODEC gd_World;

GFVECTORMAP vmf_Map;
GFOBJECT obj_Monkey;
GFOBJECT obj_Cube;
GFOBJECT obj_Plane;
GDCAMERA gd_camera;


// Cam Controller Keyboard movement speed
#define MSPEED 0.1f
#define VSPEED 0.05f
// Window settings
#define WINDOW_SIZE 90
#define WINDOW_WIDTH 16
#define WINDOW_HEIGHT 9
//Grid Settings
#define GRID_SPACING 1
#define GRID_SIZE 15

// 60011ms für GRID_SIZE=150



// 60011ms für GRID_SIZE=150 
// und DrawLine funktion ohne code

// 311ms für GRID_SIZE=150 
// und DrawEdge funktion ohne code

// 011ms für GRID_SIZE=150 
// und Demo_DrawGrid funktion ohne code

//1311-1111ms für GRID_SIZE=150 
// ohne Translate fkt in drawedge aber mit GDVEC


//1311ms für GRID_SIZE=150 
// ohne Translate fkt in drawedge


void Demo_DrawGrid() 
{
	GDVEC3 vec_Null = GDVEC3(0.0f, 0.0f, 0.0f);
	GDVEC3 vec_UnitvX = GDVEC3(1.0f, 0.0f, 0.0f);
	GDVEC3 vec_UnitvY = GDVEC3(0.0f, 1.0f, 0.0f);
	GDVEC3 vec_UnitvZ = GDVEC3(0.0f, 0.0f, 1.0f);

	gd_World.DrawEdge(&vec_Null, &vec_UnitvX, &co_Red);
	gd_World.DrawEdge(&vec_Null, &vec_UnitvY, &co_Green);
	gd_World.DrawEdge(&vec_Null, &vec_UnitvZ, &co_Blue);

	GDVEC3 p_PointA = GDVEC3(0,0,0),p_PointB= GDVEC3(0,0,0);
	float f_TempX= -GRID_SIZE, f_TempY= -GRID_SIZE;
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		for (int i_Y = -GRID_SIZE; i_Y < GRID_SIZE; i_Y++)
		{
			p_PointA.f_Pos[X] = i_X;
			p_PointA.f_Pos[Z] = i_Y+1;
			p_PointB.f_Pos[X] = i_X;
			p_PointB.f_Pos[Z] = i_Y;
			gd_World.DrawEdge(&p_PointA, &p_PointB, &co_Gray);
			p_PointB.f_Pos[X]++;
			p_PointB.f_Pos[Z]++;
			gd_World.DrawEdge(&p_PointA, &p_PointB, &co_Gray);
		}
	}
	p_PointA = GDVEC3(0,0, -(GRID_SIZE));
	p_PointB = GDVEC3(0,0, -(GRID_SIZE));

	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		p_PointA.f_Pos[X] = i_X;
		p_PointB.f_Pos[X] = i_X+1;
		gd_World.DrawEdge(&p_PointA, &p_PointB, &co_Gray);
	}
	p_PointA = GDVEC3((GRID_SIZE), 0, 0);
	p_PointB = GDVEC3((GRID_SIZE), 0, 0);
	for (int i_X = -GRID_SIZE; i_X < GRID_SIZE; i_X++)
	{
		p_PointA.f_Pos[Z] = i_X;
		p_PointB.f_Pos[Z] = i_X + 1;
		gd_World.DrawEdge(&p_PointA, &p_PointB, &co_Gray);
	}
}

void Demo_DrawColoredCubes() 
{

	//gd_World.DrawObject(&obj_Plane, &co_Gray);

	obj_Cube.p_Anchor.f_Pos[X] = 0;
	obj_Cube.p_Anchor.f_Pos[Z] = 7;
	gd_World.DrawObject(&obj_Cube, &co_Blue);
	
	obj_Cube.p_Anchor.f_Pos[1] = 1;

	obj_Cube.p_Anchor.f_Pos[X] = 7;
	obj_Cube.p_Anchor.f_Pos[Z] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Red);

	obj_Cube.p_Anchor.f_Pos[X] = -7;
	obj_Cube.p_Anchor.f_Pos[Z] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Green);

	obj_Cube.p_Anchor.f_Pos[X] = 0;
	obj_Cube.p_Anchor.f_Pos[Z] = -7;
	gd_World.DrawObject(&obj_Cube, &co_Pink);

}

void Demo_CursorLaser(win::GDWIN* gd_win) 
{
	GDVEC3 p_Angle;
	GDVEC2 p_Cpos = _PTOP(gd_win->p_CursorPos);
	gd_camera.Relate(&p_Cpos, &p_Angle);
	GDVEC3 p_LaserStart = GDVEC3(0, 2, 0);
	GDVEC3 p_LaserStop  = GDVEC3(0, 0, 50);
	GDVEC3 p_Offset  = GDVEC3(0, 1, 0 );
	p_LaserStart = p_LaserStart + gd_World.gd_Camera->i_Position;
	p_LaserStop.RotateThis(p_Angle);
	p_LaserStop.RotateThis(gd_World.gd_Camera->i_Rotation);

	gd_World.DrawEdge(&p_LaserStart,&p_LaserStop,&co_Red);
	
	system("cls");
	printf("%f %f\n",p_Angle.f_Pos[X], p_Angle.f_Pos[1]);
}



void prefabLoader() 
{
	printf("RES: %X \n", obj_Cube.Read((const LPSTR)"src\\obj\\obj2.obj"));
	printf("RES: %X \n", obj_Monkey.Read((const LPSTR)"src\\obj\\obj3.obj"));
	printf("RES: %X \n", obj_Plane.Read((const LPSTR)"src\\obj\\obj4.obj"));
	printf("RES: %X \n", vmf_Map.Read((const LPSTR)"src\\obj\\obj1.vmf"));
}
unsigned char  gdMain(win::GDWIN * gd_win)
{
	gd_Console.Create();

	gd_win->i_Width = WINDOW_WIDTH * WINDOW_SIZE;
	gd_win->i_Height = WINDOW_HEIGHT * WINDOW_SIZE;


	gd_camera = GDCAMERA(
		(FLOAT)0.1, 
		(FLOAT)100.0 , 
		(UINT32)gd_win->i_Width ,
		(UINT32)gd_win->i_Height , 
		(FLOAT)2.0f, 
		GDVEC3(0, 0, 0), 
		GDVEC3(0, 0, 0)
	);

	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);

	gd_Codec = GD2DCODEC(&gd_Img);
	gd_World = GD3DCODEC(&gd_Img,&gd_camera);


	prefabLoader();

	return TRUE;
}

DWORD*  gdUpdate(win::GDWIN * gd_win)
{
	static int i_Counter = 0;
	i_Counter++;
	i_Counter %= 10;
	auto a_TimeA = std::chrono::high_resolution_clock::now();
	//BEGIN RENDER

	gd_Img.CleanBuffer();
	BasicCameraController(gd_win, &gd_camera,MSPEED,VSPEED,TRUE,2.0);


	Demo_DrawColoredCubes();
	//Demo_CursorLaser(gd_win);
	Demo_DrawGrid();

	DrawCrosshair(&gd_World, co_White, CHST_AXIS,0.2f);

	//END RENDER
	auto a_TimeB = std::chrono::high_resolution_clock::now();
	long long i_Time = std::chrono::duration_cast<std::chrono::milliseconds>(a_TimeB - a_TimeA).count();

	if (i_Counter == 1)
	{
		printf("%dll ms Aktive:%d\n", i_Time,gd_win->b_HasFocus);
	}
	return gd_Img.d_pOutputStream;
}
void  gdClose() {
	gd_Img.Dispose();
	vmf_Map.Dispose();
}

