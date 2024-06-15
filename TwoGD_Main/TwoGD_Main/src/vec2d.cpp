#include "twogd.h"

void V2::Delta(V2 v_Pos)
{
	f_Pos[0] = f_Pos[0] - v_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - v_Pos.f_Pos[1];
}
double Distance2(V2 v_PosOne, V2 v_PosTwo)
{
	V2 v_Delta = v_PosOne - v_PosTwo;
	return sqrt(v_Delta.f_Pos[0] * v_Delta.f_Pos[0] + v_Delta.f_Pos[1] * v_Delta.f_Pos[1]);
}
double V2::Distance(V2 v_Pos)
{
	return Distance2(*this, v_Pos);
}

V2 operator - (V2  &v_Pos1, V2  &v_Pos2) {
	return V2(v_Pos1.f_Pos[0] - v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] - v_Pos2.f_Pos[1]);
}
V2 operator + (V2  &v_Pos1, V2  &v_Pos2) {
	return V2(v_Pos1.f_Pos[0] + v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] + v_Pos2.f_Pos[1]);
}
V2 operator * (V2  &v_Pos1, int &i_Lenght) {
	return V2(v_Pos1.f_Pos[0] * i_Lenght, v_Pos1.f_Pos[1] * i_Lenght);
}
V2 operator * (V2  &v_Pos1, float &f_Lenght) {
	return V2(v_Pos1.f_Pos[0] * f_Lenght, v_Pos1.f_Pos[1] * f_Lenght);
}
o_vec2::o_vec2()
{
	f_Pos[0] = 0;
	f_Pos[1] = 0;
}
o_vec2::o_vec2(float f_X = 0, float f_Y = 0)
{
	f_Pos[0] = f_X;
	f_Pos[1] = f_Y;
}

o_vec2::o_vec2(float f_dPos[2])
{
	f_Pos[0] = f_dPos[0];
	f_Pos[1] = f_dPos[1];
}
