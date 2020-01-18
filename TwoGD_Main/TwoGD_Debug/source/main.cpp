#include"twogd.h"

#include <chrono>

GDCONSOLE gd_Console;
GDCODEC gd_Display;

//memmove

GDCOLOR gdc_Test(255,0,255);
GDPOINT gdp_POne(0,0);
GDPOINT gdp_PTwo(100,100);
GDPOINT gdp_PThree(70, 50);

GFVECTORMAP vmf_Map;

GFBITMAP map_Map;
unsigned char __WAY gdmain() {
	win::i_Width =300;
	win::i_Height = 200;
	gd_Console.Create();

	gd_Display.Prepare(win::i_Width, win::i_Height);

	map_Map.Prepare(50, 50);
	memset((void *)map_Map.d_pOutputStream, 200, map_Map.i_OutputSize * sizeof(DWORD));

	gd_Display.CleanBuffer();
	printf("%d\n",sizeof(DWORD));

	printf("RES:: %X \n",vmf_Map.Read("C:\\Users\\PyroJunkie\\Desktop\\Testobjects\\obj1.vmf"));

	return TRUE;
}
unsigned char __WAY gdupdate() {
	//auto t1 = std::chrono::high_resolution_clock::now();

	gd_Display.CleanBuffer();
	gdp_PTwo.f_Pos[1] = rand()%200;
	gdp_PTwo.f_Pos[0] = rand()%300;

	gd_Display.DrawLine(&gdp_POne, &gdp_PTwo, &gdc_Test);
	for (int i = 0; i < 10; i++)
	{
		gdp_PThree.f_Pos[0] = rand() % 250;
		gdp_PThree.f_Pos[1] = rand() % 150;
		gd_Display.DrawLine(&gdp_POne, &gdp_PThree, &gdc_Test);
		gd_Display.DrawBitmap(map_Map.d_pOutputStream, &gdp_PThree, map_Map.i_Pixels);
	}

	for (size_t i = 0; i < vmf_Map.i_Connections; i++)
	{
		gd_Display.DrawLine(&vmf_Map.l_pLines->p_Point[0], &vmf_Map.l_pLines->p_Point[1], &gdc_Test);

	}

	SetScreenBuffer(gd_Display.d_pOutputStream, win::i_Width, win::i_Height);

	//auto t2 = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	////printf("%f fps\n", 1.f/(float)((double)duration/1000000.0f));
	//printf("%d \n", duration);

	return TRUE;

}
void __WAY gdclose() {
	gd_Display.Dispose();
	map_Map.Dispose();

}