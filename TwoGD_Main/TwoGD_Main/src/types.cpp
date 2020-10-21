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


void GDVEC3::Delta(GDVEC3 p_Pos)
{
	f_Pos[0] = f_Pos[0] - p_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - p_Pos.f_Pos[1];
}
double Distance3(GDVEC3 p_PosOne, GDVEC3 p_PosTwo)
{
	GDVEC3 p_Delta = p_PosOne - p_PosTwo;
	return sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1]);
}
double GDVEC3::Distance(GDVEC3 p_Pos)
{
	return Distance3(*this, p_Pos);
}

GDVEC3 operator - (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] - p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] - p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] - p_Pos2.f_Pos[2]);
}
GDVEC3 operator + (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] + p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] + p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] - p_Pos2.f_Pos[2]);
}
GDVEC3 operator * (GDVEC3  &p_Pos1, int &i_Lenght) {
	return GDVEC3(p_Pos1.f_Pos[0] * i_Lenght, p_Pos1.f_Pos[1] * i_Lenght, p_Pos1.f_Pos[2] * i_Lenght);
}
gd_vec3::gd_vec3()
{
	f_Pos[0] = 0;
	f_Pos[1] = 0;
	f_Pos[2] = 0;
}
gd_vec3::gd_vec3(float f_X = 0, float f_Y = 0, float f_Z = 0)
{
	f_Pos[0] = f_X;
	f_Pos[1] = f_Y;
	f_Pos[2] = f_Z;
}

gd_vec3::gd_vec3(float f_dPos[3])
{
	f_Pos[0] = f_dPos[0];
	f_Pos[1] = f_dPos[1];
	f_Pos[2] = f_dPos[2];
}



void GDVEC2::Delta(GDVEC2 p_Pos)
{
	f_Pos[0] = f_Pos[0] - p_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - p_Pos.f_Pos[1];
}
double Distance2(GDVEC2 p_PosOne, GDVEC2 p_PosTwo)
{
	GDVEC2 p_Delta = p_PosOne - p_PosTwo;
	return sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1]);
}
double GDVEC2::Distance(GDVEC2 p_Pos)
{
	return Distance2(*this, p_Pos);
}

GDVEC2 operator - (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2) {
	return GDVEC2(p_Pos1.f_Pos[0] - p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] - p_Pos2.f_Pos[1]);
}
GDVEC2 operator + (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2) {
	return GDVEC2(p_Pos1.f_Pos[0] + p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] + p_Pos2.f_Pos[1]);
}
GDVEC2 operator * (GDVEC2  &p_Pos1, int &i_Lenght) {
	return GDVEC2(p_Pos1.f_Pos[0] * i_Lenght, p_Pos1.f_Pos[1] * i_Lenght);
}
gd_vec2::gd_vec2()
{
	f_Pos[0] = 0;
	f_Pos[1] = 0;
}
gd_vec2::gd_vec2(float f_X = 0, float f_Y = 0)
{
	f_Pos[0] = f_X;
	f_Pos[1] = f_Y;
}

gd_vec2::gd_vec2(float f_dPos[2])
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
gd_color::gd_color(UCHAR c_R, UCHAR c_G, UCHAR c_B)
{
	c_Color[0] = c_R;
	c_Color[1] = c_G;
	c_Color[2] = c_B;
}