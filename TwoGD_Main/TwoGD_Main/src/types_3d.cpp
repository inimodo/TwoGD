#include "twogd.h"

void GDVEC3::NormalizeThis()
{
	*this = this->NormalizeTo();
}

GDVEC3 GDVEC3::NormalizeTo()
{
	float f_abs = this->Length();
	GDVEC3 o_Norm = GDVEC3(
		f_Pos[0]/ f_abs,
		f_Pos[1]/ f_abs,
		f_Pos[2]/ f_abs
	);
	return o_Norm;
}


float GDVEC3::DotProduct(GDVEC3 p_Pos)
{
	return f_Pos[0] * p_Pos.f_Pos[0] + f_Pos[1] * p_Pos.f_Pos[1] + f_Pos[2] * p_Pos.f_Pos[2];
}

GDVEC3 GDVEC3::RotateTo(GDVEC3 p_Rot)
{
	GDVEC3 o_New = GDVEC3(this->f_Pos);
	float f_temp_x, f_temp_y, f_temp_z;

	f_temp_y = o_New.f_Pos[1] * cos(p_Rot.f_Pos[1]) - o_New.f_Pos[2] * sin(p_Rot.f_Pos[1]);
	f_temp_z = o_New.f_Pos[1] * sin(p_Rot.f_Pos[1]) + o_New.f_Pos[2] * cos(p_Rot.f_Pos[1]);

	o_New.f_Pos[1] = f_temp_y;
	o_New.f_Pos[2] = f_temp_z;

	f_temp_x = o_New.f_Pos[0] * cos(p_Rot.f_Pos[0]) + o_New.f_Pos[2] * sin(p_Rot.f_Pos[0]);
	f_temp_z = -o_New.f_Pos[0] * sin(p_Rot.f_Pos[0]) + o_New.f_Pos[2] * cos(p_Rot.f_Pos[0]);

	o_New.f_Pos[0] = f_temp_x;
	o_New.f_Pos[2] = f_temp_z;

	return o_New;
}

GDVEC3 GDVEC3::AngleTo(GDVEC3 p_Pos)
{
	GDVEC3 o_Delta = *this - p_Pos;
	float f_abs = o_Delta.Length();
	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	return GDVEC3(f_leftright, f_updown,0);
}

GDVEC3 GDVEC3::Angle()
{
	return this->AngleTo(GDVEC3(0,0,0));
}

void GDVEC3::RotateThis(GDVEC3 p_Rot)
{   
	*this = this->RotateTo(p_Rot);
}


void GDVEC3::DeltaThis(GDVEC3 p_Pos)
{
	f_Pos[0] = f_Pos[0] - p_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - p_Pos.f_Pos[1];
	f_Pos[2] = f_Pos[2] - p_Pos.f_Pos[2];
}

float GDVEC3::Length()
{
	return sqrt(f_Pos[0] * f_Pos[0] + f_Pos[1] * f_Pos[1] + f_Pos[2] * f_Pos[2]);
}

double Distance3(GDVEC3 p_PosOne, GDVEC3 p_PosTwo)
{
	GDVEC3 p_Delta = p_PosOne - p_PosTwo;
	return sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1] + p_Delta.f_Pos[2] * p_Delta.f_Pos[2]);
}

double GDVEC3::DistanceTo(GDVEC3 p_Pos)
{
	return Distance3(*this, p_Pos);
}


GDVEC3 operator - (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] - p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] - p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] - p_Pos2.f_Pos[2]);
}
GDVEC3 operator + (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] + p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] + p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] + p_Pos2.f_Pos[2]);
}
GDVEC3 operator * (GDVEC3  &p_Pos1, float &i_Lenght) {
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
