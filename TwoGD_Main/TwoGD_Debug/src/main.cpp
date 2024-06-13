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

GDCOLOR co_Gray = GDCOLOR(55, 55, 55);
GDCOLOR co_White = GDCOLOR(255, 255, 255);
GDCOLOR co_Red = GDCOLOR(0, 0, 255);
GDCOLOR co_Green = GDCOLOR(0, 255, 0);
GDCOLOR co_Blue = GDCOLOR(255, 0, 0);
GDCOLOR co_Pink = GDCOLOR(255, 0, 255);

#define MSPEED 0.05f
#define VSPEED 0.05f*0.3f


void Demo_DrawGrid() 
{

	GDVEC3 vec_Null = GDVEC3(0.0f, 0.0f, 0.0f);
	GDVEC3 vec_UnitvX = GDVEC3(1.0f, 0.0f, 0.0f);
	GDVEC3 vec_UnitvY = GDVEC3(0.0f, 1.0f, 0.0f);
	GDVEC3 vec_UnitvZ = GDVEC3(0.0f, 0.0f, 1.0f);

	gd_World.DrawEdge(&vec_Null, &vec_UnitvX, &co_Red);
	gd_World.DrawEdge(&vec_Null, &vec_UnitvY, &co_Green);
	gd_World.DrawEdge(&vec_Null, &vec_UnitvZ, &co_Blue);
}

void Demo_DrawColoredCubes() 
{

	gd_World.DrawObject(&obj_Plane, &co_Gray);

	obj_Cube.p_Anchor.f_Pos[0] = 0;
	obj_Cube.p_Anchor.f_Pos[2] = 7;
	gd_World.DrawObject(&obj_Cube, &co_Blue);
	

	obj_Cube.p_Anchor.f_Pos[1] = 1;

	obj_Cube.p_Anchor.f_Pos[0] = 7;
	obj_Cube.p_Anchor.f_Pos[2] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Red);

	obj_Cube.p_Anchor.f_Pos[0] = -7;
	obj_Cube.p_Anchor.f_Pos[2] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Green);

	obj_Cube.p_Anchor.f_Pos[0] = 0;
	obj_Cube.p_Anchor.f_Pos[2] = -7;
	gd_World.DrawObject(&obj_Cube, &co_Pink);

}

void Demo_CursorLaser(win::GDWIN* gd_win) 
{
	GDVEC3 p_Angle;
	gd_camera.Relate(&gd_win->p_CursorPos, &p_Angle);
	GDVEC3 p_LaserStart = GDVEC3(0, 2, 0);
	GDVEC3 p_LaserStop  = GDVEC3(0, 0, 50);
	GDVEC3 p_Offset  = GDVEC3(0, 1, 0 );
	p_LaserStart = p_LaserStart + gd_World.gd_Camera->i_Position;
	p_LaserStop.RotateThis(p_Angle);
	p_LaserStop.RotateThis(gd_World.gd_Camera->i_Rotation);

	gd_World.DrawEdge(&p_LaserStart,&p_LaserStop,&co_Red);
	
	system("cls");
	printf("%f %f\n",p_Angle.f_Pos[0], p_Angle.f_Pos[1]);
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

	gd_win->i_Width =16*60;
	gd_win->i_Height = 9*60;

	gd_camera.i_Dimensions[0] = gd_win->i_Width;
	gd_camera.i_Dimensions[1] = gd_win->i_Height;

	gd_camera.f_Frustum[0] = 0.1;
	gd_camera.f_Frustum[1] = 100;
	gd_camera.f_FOV = 2;

	gd_Console.Create();
	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);

	gd_Codec = GD2DCODEC(&gd_Img);
	gd_World = GD3DCODEC(&gd_Img,&gd_camera);


	prefabLoader();

	return TRUE;
}

DWORD*  gdUpdate(win::GDWIN * gd_win)
{
	gd_Img.CleanBuffer();
	BasicCameraController(&gd_camera,MSPEED,VSPEED);

	//system("cls");
	//GDM3X3 m1 = GDM3X3(
	//	GDVEC3(1,4,7),
	//	GDVEC3(2,5,8),
	//	GDVEC3(3,6,9)
	//);
	//GDM3X3 m2 = GDM3X3(
	//	GDVEC3(3, 6, 9),
	//	GDVEC3(2, 5, 8),
	//	GDVEC3(1, 4, 7)
	//);

	//GDM3X3 res = m2 * m1;

	//m1.print();
	//m2.print();
	//res.print();



	Demo_DrawColoredCubes();
	//Demo_CursorLaser(gd_win);
	Demo_DrawGrid();

	DrawCrosshair(&gd_World, co_White,10);
	return gd_Img.d_pOutputStream;
}
void  gdClose() {
	gd_Img.Dispose();
	vmf_Map.Dispose();
}

