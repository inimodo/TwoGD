#include"twogd.h"
#include <chrono>

// 10.000 Lines = 24 Fps

GDCONSOLE gd_Console;
GD2DCODEC gd_Codec;
GDCANVAS gd_Img;

GD3DCODEC gd_World;

GFVECTORMAP vmf_Map;
GFOBJECT obj_Prefab;
GDCAMERA gd_camera;

unsigned char __WAY gdmain(win::GDWIN * gd_win)
{
	gd_win->i_Width =16*50;
	gd_win->i_Height = 9*50;


	gd_Console.Create();
	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);
	gd_Codec = GD2DCODEC(&gd_Img);
	gd_World = GD3DCODEC(&gd_Img,&gd_camera);

	printf("RES:: %X \n", obj_Prefab.Read((const LPSTR)"src\\obj2.obj"));
	printf("RES:: %X \n",vmf_Map.Read((const LPSTR)"src\\obj1.vmf"));
	system("pause");
	return TRUE;
}
unsigned char __WAY gdupdate(win::GDWIN * gd_win)
{
	auto a_TimeA = std::chrono::high_resolution_clock::now();






	gd_Img.CleanBuffer();

	gd_World.

	
	SetScreenBuffer(gd_Img.d_pOutputStream, gd_win->i_Width, gd_win->i_Height);



	auto a_TimeB = std::chrono::high_resolution_clock::now();
	auto a_Time = std::chrono::duration_cast<std::chrono::microseconds>(a_TimeB - a_TimeA).count();
	system("cls");

	printf("%ffps \n", 1.f/(a_Time/1000000.0f));
	return TRUE;

}
void __WAY gdclose() {
	gd_Img.Dispose();
	vmf_Map.Dispose();
}