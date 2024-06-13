#include"twogd.h"

gd_m3x3::gd_m3x3(GDVEC3 p_v1, GDVEC3 p_v2, GDVEC3 p_v3)
{
	this->p_v1 = p_v1;
	this->p_v2 = p_v2;
	this->p_v3 = p_v3;
}


gd_m3x3::gd_m3x3(float f_Unit)
{
	this->p_v1 = GDVEC3(f_Unit,0,0);
	this->p_v2 = GDVEC3(0, f_Unit, 0);;
	this->p_v3 = GDVEC3(0, 0, f_Unit);;
}

GDM3X3 MMultM(GDM3X3 m_M1, GDM3X3 m_M2)
{
	GDVEC3 p_r1 = GDVEC3(m_M1.p_v1.f_Pos[0], m_M1.p_v2.f_Pos[0], m_M1.p_v3.f_Pos[0]);
	GDVEC3 p_r2 = GDVEC3(m_M1.p_v1.f_Pos[1], m_M1.p_v2.f_Pos[1], m_M1.p_v3.f_Pos[1]);
	GDVEC3 p_r3 = GDVEC3(m_M1.p_v1.f_Pos[2], m_M1.p_v2.f_Pos[2], m_M1.p_v3.f_Pos[2]);

	return GDM3X3(
		GDVEC3(p_r1.DotProduct(m_M2.p_v1), p_r2.DotProduct(m_M2.p_v1), p_r3.DotProduct(m_M2.p_v1)),
		GDVEC3(p_r1.DotProduct(m_M2.p_v2), p_r2.DotProduct(m_M2.p_v2), p_r3.DotProduct(m_M2.p_v2)),
		GDVEC3(p_r1.DotProduct(m_M2.p_v3), p_r2.DotProduct(m_M2.p_v3), p_r3.DotProduct(m_M2.p_v3))
	);
}


GDM3X3 MMultS(GDM3X3 m_M, float f_S)
{
	return GDM3X3(
		GDVEC3(m_M.p_v1.f_Pos[0] * f_S, m_M.p_v1.f_Pos[1] * f_S, m_M.p_v1.f_Pos[2] * f_S),
		GDVEC3(m_M.p_v2.f_Pos[0] * f_S, m_M.p_v2.f_Pos[1] * f_S, m_M.p_v2.f_Pos[2] * f_S),
		GDVEC3(m_M.p_v3.f_Pos[0] * f_S, m_M.p_v3.f_Pos[1] * f_S, m_M.p_v3.f_Pos[2] * f_S)
	);
}


GDM3X3 MAddM(GDM3X3 m_M1, GDM3X3 m_M2)
{
	return GDM3X3(
		GDVEC3(m_M1.p_v1.f_Pos[0] + m_M2.p_v1.f_Pos[0], m_M1.p_v1.f_Pos[1] + m_M2.p_v1.f_Pos[1], m_M1.p_v1.f_Pos[2] + m_M2.p_v1.f_Pos[2]),
		GDVEC3(m_M1.p_v2.f_Pos[0] + m_M2.p_v2.f_Pos[0], m_M1.p_v2.f_Pos[1] + m_M2.p_v2.f_Pos[1], m_M1.p_v2.f_Pos[2] + m_M2.p_v2.f_Pos[2]),
		GDVEC3(m_M1.p_v3.f_Pos[0] + m_M2.p_v3.f_Pos[0], m_M1.p_v3.f_Pos[1] + m_M2.p_v3.f_Pos[1], m_M1.p_v3.f_Pos[2] + m_M2.p_v3.f_Pos[2])
	);
}

GDVEC3 MMultV(GDM3X3 m_M, GDVEC3 p_V)
{
	return GDVEC3(
		m_M.p_v1.f_Pos[0] * p_V.f_Pos[0] + m_M.p_v2.f_Pos[0] * p_V.f_Pos[1] + m_M.p_v3.f_Pos[0] * p_V.f_Pos[2],
		m_M.p_v1.f_Pos[1] * p_V.f_Pos[0] + m_M.p_v2.f_Pos[1] * p_V.f_Pos[1] + m_M.p_v3.f_Pos[1] * p_V.f_Pos[2],
		m_M.p_v1.f_Pos[2] * p_V.f_Pos[0] + m_M.p_v2.f_Pos[2] * p_V.f_Pos[1] + m_M.p_v3.f_Pos[2] * p_V.f_Pos[2]
	);
}

GDM3X3 M3X3RotU(GDVEC3 p_UnitV, float f_phi)
{
	//Rodrigues' Rotation Formula
	GDM3X3 I = GDM3X3(1.0f);
	GDM3X3 w = GDM3X3(
		GDVEC3(0, p_UnitV.f_Pos[2], -p_UnitV.f_Pos[1]),
		GDVEC3(-p_UnitV.f_Pos[2], 0, p_UnitV.f_Pos[0]),
		GDVEC3(p_UnitV.f_Pos[1], -p_UnitV.f_Pos[0], 0)
	);

	return I + (w * sin(f_phi)) + ((w*w)*(1.0f - cos(f_phi)));
}

GDM3X3 operator*(GDM3X3 & m_M, float & f_S)
{
	return MMultS(m_M,f_S);
}
GDM3X3 operator+(GDM3X3 & m_M1, GDM3X3 & m_M2)
{
	return MAddM(m_M1, m_M2);
}
GDM3X3 operator*(GDM3X3 & m_M1, GDM3X3 & m_M2)
{
	return MMultM(m_M1, m_M2);
}

GDVEC3 operator*(GDM3X3 & m_M, GDVEC3 & p_V)
{
	return MMultV(m_M,p_V);
}

GDM3X3 operator*(GDM3X3 const& m_M, float const& f_S)
{
	return MMultS(m_M, f_S);
}
GDM3X3 operator+(GDM3X3 const& m_M1, GDM3X3 const& m_M2)
{
	return MAddM(m_M1, m_M2);
}
GDM3X3 operator*(GDM3X3 const& m_M1, GDM3X3 const& m_M2)
{
	return MMultM(m_M1, m_M2);
}

GDVEC3 operator*(GDM3X3 const& m_M, GDVEC3 const& p_V)
{
	return MMultV(m_M, p_V);
}

void gd_m3x3::print()
{
	printf("%f\t %f\t %f\n"  ,this->p_v1.f_Pos[0], this->p_v2.f_Pos[0], this->p_v3.f_Pos[0]);
	printf("%f\t %f\t %f\n"  ,this->p_v1.f_Pos[1], this->p_v2.f_Pos[1], this->p_v3.f_Pos[1]);
	printf("%f\t %f\t %f\n\n",this->p_v1.f_Pos[2], this->p_v2.f_Pos[2], this->p_v3.f_Pos[2]);
}

GDM3X3 M3X3RotX(float f_phi)
{
	return GDM3X3(
		GDVEC3(1,0,0),
		GDVEC3(0,cos(f_phi),sin(f_phi)),
		GDVEC3(0,-sin(f_phi),cos(f_phi))
	);
}
GDM3X3 M3X3RotY(float f_phi)
{
	return GDM3X3(
		GDVEC3(cos(f_phi), 0, -sin(f_phi)),
		GDVEC3(0, 1.0, 0),
		GDVEC3(sin(f_phi), 0, cos(f_phi))
	);
}

GDM3X3 M3X3RotZ(float f_phi)
{
	return GDM3X3(
		GDVEC3(cos(f_phi), sin(f_phi), 0),
		GDVEC3(-sin(f_phi), cos(f_phi), 0),
		GDVEC3(0, 0, 1)
	);
}