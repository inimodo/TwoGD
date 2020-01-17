#include"twogd.h"

GDCONSOLE gd_Console;
GDCODEC gd_Display;

//memmove

GDCOLOR gdc_Test(255,255,255);
GDPOINT gdp_POne(0,0);
GDPOINT gdp_PTwo(100,100);

GFVECTORMAP vmf_Map;

unsigned char __WAY gdmain() {
	win::i_Width =300;
	win::i_Height = 300;
	gd_Console.Create();

	gd_Display.Prepare(win::i_Width, win::i_Height);
	gd_Display.CleanBuffer();

	printf("RES:: %X \n",vmf_Map.Read("C:\\Users\\PyroJunkie\\Desktop\\Testobjects\\obj1.vmf"));

	return TRUE;
}
unsigned char __WAY gdupdate() {
	gd_Display.CleanBuffer();
	gdp_PTwo.f_Pos[1] = rand()%300;
	gdp_PTwo.f_Pos[0] = rand()%300;

	gd_Display.DrawLine(&gdp_POne, &gdp_PTwo, &gdc_Test);

	SetScreenBuffer(gd_Display.d_pOutputStream, win::i_Width, win::i_Height);
	return TRUE;

}
void __WAY gdclose() {
	gd_Display.Dispose();

}