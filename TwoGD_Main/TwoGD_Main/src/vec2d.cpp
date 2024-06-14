#include "twogd.h"

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
GDVEC2 operator * (GDVEC2  &p_Pos1, float &f_Lenght) {
	return GDVEC2(p_Pos1.f_Pos[0] * f_Lenght, p_Pos1.f_Pos[1] * f_Lenght);
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
