#include<twogd.h>

GDCONSOLE gd_Console;
GD2DCODEC gd_Codec;
GDCANVAS gd_Img;




unsigned char __WAY gdmain(win::GDWIN * gd_win)
{
	gd_win->i_Width = 16 * 50;
	gd_win->i_Height = 9 * 50;

	gd_Console.Create();
	gd_Img.Prepare(gd_win->i_Width, gd_win->i_Height);
	gd_Codec = GD2DCODEC(&gd_Img);

	system("pause");
	return TRUE;
}

unsigned char __WAY gdupdate(win::GDWIN * gd_win)
{
	gd_Img.CleanBuffer();
	SetScreenBuffer(gd_Img.d_pOutputStream, gd_win->i_Width, gd_win->i_Height);



	system("cls");
	return TRUE;
}

void __WAY gdclose() 
{
	gd_Img.Dispose();
}