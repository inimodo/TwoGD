#include"twogd.h"

#include <chrono>

GDCONSOLE gd_Console;
GDCODEC gd_Codec;
GDCANVAS gd_Img;


GFVECTORMAP vmf_Map;


unsigned char __WAY gdmain(win::GDWIN * gd_win)
{
	gd_win->i_Width =300;
	gd_win->i_Height = 200;


	gd_Console.Create();
	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);
	gd_Codec = GDCODEC(&gd_Img);


	printf("RES:: %X \n",vmf_Map.Read("src\\obj1.vmf"));
	system("pause");
	return TRUE;
}
unsigned char __WAY gdupdate(win::GDWIN * gd_win)
{
	auto a_TimeA = std::chrono::high_resolution_clock::now();






	gd_Img.CleanBuffer();

	for (int i = 0; i < 1000; i++)
	{

	gd_Codec.DrawVMap(&vmf_Map);
	}
	
	SetScreenBuffer(gd_Img.d_pOutputStream, gd_win->i_Width, gd_win->i_Height);




	auto a_TimeB = std::chrono::high_resolution_clock::now();
	auto a_Time = std::chrono::duration_cast<std::chrono::microseconds>(a_TimeB - a_TimeA).count();
	printf("%d \n", a_Time);
	return TRUE;

}
void __WAY gdclose() {
	gd_Img.Dispose();
	vmf_Map.Dispose();
}