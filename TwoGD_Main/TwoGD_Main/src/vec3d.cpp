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

void gd_vec3::RotateAroundThis(gd_vec3 p_UnitV,float f_phi)
{
	*this = M3X3RotU(p_UnitV, f_phi) * *this;
}

GDVEC3 gd_vec3::RotateAroundTo(gd_vec3 p_UnitV, float f_phi)
{
	return M3X3RotU(p_UnitV, f_phi) * *this;
}


//GDVEC3 GDVEC3::RotateTo(GDVEC3 p_Rot)
//{
//	GDM3X3 m_X = M3X3RotX(p_Rot.f_Pos[1]);
//	GDM3X3 m_Y = M3X3RotY(p_Rot.f_Pos[0]);
//	GDM3X3 m_Z = M3X3RotZ(p_Rot.f_Pos[2]);
//
//	return (m_Z*m_Y*m_X) * *this;
//}


GDVEC3 GDVEC3::RotateTo(GDVEC3 p_Rot)
{
	GDM3X3 m_Y = M3X3RotY(-p_Rot.f_Pos[X]);
	GDVEC3 p_UntX = m_Y * GDVEC3(-1.0f, 0, 0);
	GDM3X3 m_X = M3X3RotU(p_UntX, p_Rot.f_Pos[Y]);

	return m_X * m_Y * *this;

}

GDVEC3 GDVEC3::CamRotateTo(GDVEC3 p_Rot)
{
	GDM3X3 m_Y = M3X3RotY(p_Rot.f_Pos[0]);
	GDM3X3 m_YY = M3X3RotY(-p_Rot.f_Pos[0]);
	GDVEC3 p_UntX = m_YY*GDVEC3(1.0f, 0, 0);
	GDM3X3 m_X = M3X3RotU(p_UntX,p_Rot.f_Pos[1]);

	return m_Y * (m_X * *this);
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

void GDVEC3::CamRotateThis(GDVEC3 p_Rot)
{   
	*this = this->CamRotateTo(p_Rot);
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
GDVEC3 operator + (GDVEC3  &p_Pos1, GDVEC3  const&p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] + p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] + p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] + p_Pos2.f_Pos[2]);
}
GDVEC3 operator * (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2) {
	return GDVEC3(p_Pos1.f_Pos[0] * p_Pos2.f_Pos[0], p_Pos1.f_Pos[1] * p_Pos2.f_Pos[1], p_Pos1.f_Pos[2] * p_Pos2.f_Pos[2]);
}
GDVEC3 operator * (GDVEC3  &p_Pos, float const&f_Lenght) {
	return GDVEC3(p_Pos.f_Pos[0] * f_Lenght, p_Pos.f_Pos[1] * f_Lenght, p_Pos.f_Pos[2] * f_Lenght);
}
GDVEC3 operator * (GDVEC3  &p_Pos, float &f_Lenght) {
	return GDVEC3(p_Pos.f_Pos[0] * f_Lenght, p_Pos.f_Pos[1] * f_Lenght, p_Pos.f_Pos[2] * f_Lenght);
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
