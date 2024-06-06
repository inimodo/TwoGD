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

#define TICKSPEED 0.05f

#define BUFFERS 50
int i_Ticks;
float f_Buffered[BUFFERS];


void draw() 
{
	//obj_Monkey.p_Anchor.f_Pos[1] = 1;
	//gd_World.DrawObject(&obj_Monkey, &co_White);

	gd_World.DrawObject(&obj_Plane, &co_Gray);

	obj_Cube.p_Anchor.f_Pos[1] = 1;

	obj_Cube.p_Anchor.f_Pos[0] = 7;
	obj_Cube.p_Anchor.f_Pos[2] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Red);	

	obj_Cube.p_Anchor.f_Pos[0] = -7;
	obj_Cube.p_Anchor.f_Pos[2] = 0;
	gd_World.DrawObject(&obj_Cube, &co_Green);

	obj_Cube.p_Anchor.f_Pos[0] = 0;
	obj_Cube.p_Anchor.f_Pos[2] = 7;
	gd_World.DrawObject(&obj_Cube, &co_Blue);

	obj_Cube.p_Anchor.f_Pos[0] = 0;
	obj_Cube.p_Anchor.f_Pos[2] = -7;
	gd_World.DrawObject(&obj_Cube, &co_Pink);
	
	GDVEC3 vec_Null = GDVEC3(0.0f,0.0f,0.0f);
	GDVEC3 vec_Offset = GDVEC3(0.0f,0.0f,0.0f);
	vec_Offset.f_Pos[0] = 1;
	gd_World.DrawEdge(&vec_Null,&vec_Offset,&co_Red);
	vec_Offset.f_Pos[0] = 0;
	vec_Offset.f_Pos[1] = 1;
	gd_World.DrawEdge(&vec_Null,&vec_Offset,&co_Green);
	vec_Offset.f_Pos[0] = 0;
	vec_Offset.f_Pos[1] = 0;
	vec_Offset.f_Pos[2] = 1;
	gd_World.DrawEdge(&vec_Null,&vec_Offset,&co_Blue);
}

void keyInput() 
{
	if (GetAsyncKeyState(VK_NUMPAD2) != 0)gd_camera.i_Position.f_Pos[2] -= TICKSPEED;
	if (GetAsyncKeyState(VK_NUMPAD8) != 0)gd_camera.i_Position.f_Pos[2] += TICKSPEED;

	if (GetAsyncKeyState(VK_NUMPAD6) != 0)gd_camera.i_Position.f_Pos[0] += TICKSPEED;
	if (GetAsyncKeyState(VK_NUMPAD4) != 0)gd_camera.i_Position.f_Pos[0] -= TICKSPEED;

	if (GetAsyncKeyState(VK_NUMPAD1) != 0)gd_camera.i_Position.f_Pos[1] += TICKSPEED;
	if (GetAsyncKeyState(VK_NUMPAD0) != 0)gd_camera.i_Position.f_Pos[1] -= TICKSPEED;

	if (GetAsyncKeyState(VK_NUMPAD9) != 0)gd_camera.i_Rotation.f_Pos[0] += TICKSPEED * 0.3;
	if (GetAsyncKeyState(VK_MULTIPLY) != 0)gd_camera.i_Rotation.f_Pos[0] -= TICKSPEED * 0.3;

	if (GetAsyncKeyState(VK_ADD) != 0)gd_camera.i_Rotation.f_Pos[1] += TICKSPEED * 0.3;
	if (GetAsyncKeyState(VK_SUBTRACT) != 0)gd_camera.i_Rotation.f_Pos[1] -= TICKSPEED * 0.3;
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

	gd_win->i_Width =16*50;
	gd_win->i_Height = 9*50;

	gd_camera.i_Dimensions[0] = gd_win->i_Width;
	gd_camera.i_Dimensions[1] = gd_win->i_Height;

	gd_camera.i_Frustum[0] = 1;
	gd_camera.i_Frustum[1] = 30;

	gd_Console.Create();
	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);

	gd_Codec = GD2DCODEC(&gd_Img);
	gd_World = GD3DCODEC(&gd_Img,&gd_camera);


	prefabLoader();

	return TRUE;
}
unsigned char  gdUpdate(win::GDWIN * gd_win)
{
	auto a_TimeA = std::chrono::high_resolution_clock::now();

	keyInput();

	gd_Img.CleanBuffer();

	draw();
	
	SetScreenBuffer(gd_Img.d_pOutputStream, gd_win->i_Width, gd_win->i_Height);

	auto a_TimeB = std::chrono::high_resolution_clock::now();
	auto a_Time = std::chrono::duration_cast<std::chrono::microseconds>(a_TimeB - a_TimeA).count();

	system("cls");
	printf("Viewport: \nFramerate %dfps ", (int)(1.f/(a_Time/100000.0f)));

	if (i_Ticks >= BUFFERS)i_Ticks = 0;
	f_Buffered[i_Ticks] = 1.f / (a_Time / 100000.0f);
	i_Ticks++;
	float i_Avrg = 0;
	for (INT i = 0; i < BUFFERS; i++)
	{
		i_Avrg += f_Buffered[i];
	}
	printf("(Average %dfps) \n", (int)(i_Avrg / BUFFERS));

	printf("\nCamera:\n");
	printf("Postion %f %f %f\n", gd_camera.i_Position.f_Pos[0], gd_camera.i_Position.f_Pos[1], gd_camera.i_Position.f_Pos[2]);
	printf("Angle   %f %f %f\n", gd_camera.i_Rotation.f_Pos[0], gd_camera.i_Rotation.f_Pos[1], gd_camera.i_Rotation.f_Pos[2]);
	return TRUE;

}
void  gdClose() {
	gd_Img.Dispose();
	vmf_Map.Dispose();
}


