#include "..\twogd.h"

void V3::NormalizeThis()
{
	*this = this->NormalizeTo();
}

V3 V3::NormalizeTo()
{
	float f_abs = this->Length();
	V3 o_Norm = V3(
		f_Pos[0]/ f_abs,
		f_Pos[1]/ f_abs,
		f_Pos[2]/ f_abs
	);
	return o_Norm;
}

float V3::DotProduct(V3 v_Pos)
{
	return f_Pos[0] * v_Pos.f_Pos[0] + f_Pos[1] * v_Pos.f_Pos[1] + f_Pos[2] * v_Pos.f_Pos[2];
}

void o_vec3::RotateAroundThis(o_vec3 v_UnitV,float f_phi)
{
	*this = M3X3RotU(v_UnitV, f_phi) * *this;
}

V3 o_vec3::RotateAroundTo(o_vec3 v_UnitV, float f_phi)
{
	return M3X3RotU(v_UnitV, f_phi) * *this;
}

V3 V3::RotateTo(V3 v_Rot)
{
	M3X3 m_Y = M3X3RotY(-v_Rot.f_Pos[X]);
	V3 v_UntX = m_Y * V3(-1.0f, 0, 0);
	M3X3 m_X = M3X3RotU(v_UntX, v_Rot.f_Pos[Y]);

	return m_X * m_Y * *this;
}

V3 V3::CamRotateTo(V3 v_Rot)// NEED HEAVY OPT.
{
	M3X3 m_Y = M3X3(
		V3(cos(v_Rot.f_Pos[0]), 0, -sin(v_Rot.f_Pos[0])),
		V3(0, 1.0, 0),
		V3(sin(v_Rot.f_Pos[0]), 0, cos(v_Rot.f_Pos[0]))
	);

	V3 v_UnitV = V3(
		cos(-v_Rot.f_Pos[0]),
		0,
		-sin(-v_Rot.f_Pos[0])
	);

	M3X3 m_X = M3X3RotU(v_UnitV,v_Rot.f_Pos[1]);

	return m_Y *( m_X * *this);
}

V3 V3::AngleTo(V3 v_Pos)
{
	V3 o_Delta = *this - v_Pos;
	float f_abs = o_Delta.Length();
	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	return V3(f_leftright, f_updown,0);
}

V3 V3::Angle()
{
	return this->AngleTo(V3(0,0,0));
}

void V3::CamRotateThis(V3 v_Rot)
{   
	*this = this->CamRotateTo(v_Rot);
}

void V3::RotateThis(V3 v_Rot)
{
	*this = this->RotateTo(v_Rot);
}

void V3::DeltaThis(V3 v_Pos)
{
	f_Pos[0] = f_Pos[0] - v_Pos.f_Pos[0];
	f_Pos[1] = f_Pos[1] - v_Pos.f_Pos[1];
	f_Pos[2] = f_Pos[2] - v_Pos.f_Pos[2];
}

float V3::Length()
{
	return sqrt(f_Pos[0] * f_Pos[0] + f_Pos[1] * f_Pos[1] + f_Pos[2] * f_Pos[2]);
}

double Distance3(V3 v_PosOne, V3 v_PosTwo)
{
	V3 v_Delta = v_PosOne - v_PosTwo;
	return sqrt(v_Delta.f_Pos[0] * v_Delta.f_Pos[0] + v_Delta.f_Pos[1] * v_Delta.f_Pos[1] + v_Delta.f_Pos[2] * v_Delta.f_Pos[2]);
}

double V3::DistanceTo(V3 v_Pos)
{
	return Distance3(*this, v_Pos);
}

V3 operator - (V3  &v_Pos1, V3  &v_Pos2) {
	return V3(v_Pos1.f_Pos[0] - v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] - v_Pos2.f_Pos[1], v_Pos1.f_Pos[2] - v_Pos2.f_Pos[2]);
}
V3 operator + (V3  &v_Pos1, V3  &v_Pos2) {
	return V3(v_Pos1.f_Pos[0] + v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] + v_Pos2.f_Pos[1], v_Pos1.f_Pos[2] + v_Pos2.f_Pos[2]);
}
V3 operator + (V3  &v_Pos1, V3  const&v_Pos2) {
	return V3(v_Pos1.f_Pos[0] + v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] + v_Pos2.f_Pos[1], v_Pos1.f_Pos[2] + v_Pos2.f_Pos[2]);
}
V3 operator * (V3  &v_Pos1, V3  &v_Pos2) {
	return V3(v_Pos1.f_Pos[0] * v_Pos2.f_Pos[0], v_Pos1.f_Pos[1] * v_Pos2.f_Pos[1], v_Pos1.f_Pos[2] * v_Pos2.f_Pos[2]);
}
V3 operator * (V3  &v_Pos, float const&f_Lenght) {
	return V3(v_Pos.f_Pos[0] * f_Lenght, v_Pos.f_Pos[1] * f_Lenght, v_Pos.f_Pos[2] * f_Lenght);
}
V3 operator * (V3  &v_Pos, float &f_Lenght) {
	return V3(v_Pos.f_Pos[0] * f_Lenght, v_Pos.f_Pos[1] * f_Lenght, v_Pos.f_Pos[2] * f_Lenght);
}

o_vec3::o_vec3()
{
	f_Pos[0] = 0;
	f_Pos[1] = 0;
	f_Pos[2] = 0;
}
o_vec3::o_vec3(float f_X = 0, float f_Y = 0, float f_Z = 0)
{
	f_Pos[0] = f_X;
	f_Pos[1] = f_Y;
	f_Pos[2] = f_Z;
}

o_vec3::o_vec3(float f_dPos[3])
{
	f_Pos[0] = f_dPos[0];
	f_Pos[1] = f_dPos[1];
	f_Pos[2] = f_dPos[2];
}
