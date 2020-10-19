#include "twogd.h"
__STATUS __WAY
canvas::Dispose()
{
	free(d_pOutputStream);
	return GD_TASK_OKAY;

}
__STATUS __WAY
canvas::CleanBuffer()
{
	memset((void *)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
	return GD_TASK_OKAY;

}
__STATUS
canvas::Prepare(int i_Width, int i_Height)
{
	i_OutputSize = i_Width * i_Height;
	d_pOutputStream = (DWORD *)malloc(sizeof(DWORD)*i_OutputSize);
	if (d_pOutputStream == NULL) {
		return GD_ALLOC_FAILED;
	}
	i_Pixels[1] = i_Height;
	i_Pixels[0] = i_Width;
	return GD_TASK_OKAY;
}
void GDPOINT::Delta(GDPOINT p_Pos)
{
	f_Pos[0] = f_Pos[0] - p_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - p_Pos.f_Pos[1];
}
double Distance2(GDPOINT p_PosOne, GDPOINT p_PosTwo)
{
	GDPOINT p_Delta = p_PosOne - p_PosTwo;
	return sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1]);
}
double GDPOINT::Distance(GDPOINT p_Pos)
{
	return Distance2(*this, p_Pos);
}

GDPOINT operator - (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2) {
	return GDPOINT(p_Pos1.f_Pos[0] - p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] - p_Pos2.f_Pos[1]);
}
GDPOINT operator + (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2) {
	return GDPOINT(p_Pos1.f_Pos[0] + p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] + p_Pos2.f_Pos[1]);
}
GDPOINT operator * (GDPOINT  &p_Pos1, int &i_Lenght) {
	return GDPOINT(p_Pos1.f_Pos[0] * i_Lenght, p_Pos1.f_Pos[1] * i_Lenght);
}
gd_point::gd_point()
{
	f_Pos[0] = 0;
	f_Pos[1] = 0;
}
gd_point::gd_point(float f_X = 0, float f_Y = 0)
{
	f_Pos[0] = f_X;
	f_Pos[1] = f_Y;
}

gd_point::gd_point(float f_dPos[2])
{
	f_Pos[0] = f_dPos[0];
	f_Pos[1] = f_dPos[1];
}
gd_color::gd_color()
{
	c_Color[0] = 0;
	c_Color[1] = 0;
	c_Color[2] = 0;
}
DWORD gd_color::GetAsHex()
{
	return RGB(c_Color[0], c_Color[1], c_Color[2]);
}
gd_color::gd_color(uchar c_R, uchar c_G, uchar c_B)
{
	c_Color[0] = c_R;
	c_Color[1] = c_G;
	c_Color[2] = c_B;
}