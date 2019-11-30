#include"twogd.h"

GDCONSOLE gd_Console;
GDDISPLAY gd_Display;


GDCOLOR gdc_Test(255,255,255);
GDPOINT gdp_POne(0,0);
GDPOINT gdp_PTwo(100,100);
unsigned char __cdecl gdmain() {
	win::i_Width =300;
	win::i_Height = 300;
	gd_Console.Create();

	gd_Display.Prepare(win::i_Width, win::i_Height);
	gd_Display.CleanBuffer();
	return TRUE;
}
unsigned char __cdecl gdupdate() {
	gd_Display.CleanBuffer();
	gdp_PTwo.f_Pos[1] = rand()%300;
	gdp_PTwo.f_Pos[0] = rand()%300;

	gd_Display.DrawLine(&gdp_POne, &gdp_PTwo, &gdc_Test);

	SetScreenBuffer(gd_Display.d_pOutputStream, win::i_Width, win::i_Height);
	return TRUE;

}
void __cdecl gdclose() {
	gd_Display.Dispose();

}