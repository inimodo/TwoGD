#include "..\twogd.h"

o_m3x3::o_m3x3(V3 v_v1, V3 v_v2, V3 v_v3)
{
	this->v_v1 = v_v1;
	this->v_v2 = v_v2;
	this->v_v3 = v_v3;
}


o_m3x3::o_m3x3(float f_Unit)
{
	this->v_v1 = V3(f_Unit,0,0);
	this->v_v2 = V3(0, f_Unit, 0);;
	this->v_v3 = V3(0, 0, f_Unit);;
}

M3X3 MMultM(M3X3 m_M1, M3X3 m_M2)
{
	V3 v_r1 = V3(m_M1.v_v1.f_Pos[0], m_M1.v_v2.f_Pos[0], m_M1.v_v3.f_Pos[0]);
	V3 v_r2 = V3(m_M1.v_v1.f_Pos[1], m_M1.v_v2.f_Pos[1], m_M1.v_v3.f_Pos[1]);
	V3 v_r3 = V3(m_M1.v_v1.f_Pos[2], m_M1.v_v2.f_Pos[2], m_M1.v_v3.f_Pos[2]);

	return M3X3(
		V3(v_r1.DotProduct(m_M2.v_v1), v_r2.DotProduct(m_M2.v_v1), v_r3.DotProduct(m_M2.v_v1)),
		V3(v_r1.DotProduct(m_M2.v_v2), v_r2.DotProduct(m_M2.v_v2), v_r3.DotProduct(m_M2.v_v2)),
		V3(v_r1.DotProduct(m_M2.v_v3), v_r2.DotProduct(m_M2.v_v3), v_r3.DotProduct(m_M2.v_v3))
	);
}


M3X3 MMultS(M3X3 m_M, float f_S)
{
	return M3X3(
		V3(m_M.v_v1.f_Pos[0] * f_S, m_M.v_v1.f_Pos[1] * f_S, m_M.v_v1.f_Pos[2] * f_S),
		V3(m_M.v_v2.f_Pos[0] * f_S, m_M.v_v2.f_Pos[1] * f_S, m_M.v_v2.f_Pos[2] * f_S),
		V3(m_M.v_v3.f_Pos[0] * f_S, m_M.v_v3.f_Pos[1] * f_S, m_M.v_v3.f_Pos[2] * f_S)
	);
}


M3X3 MAddM(M3X3 m_M1, M3X3 m_M2)
{
	return M3X3(
		V3(m_M1.v_v1.f_Pos[0] + m_M2.v_v1.f_Pos[0], m_M1.v_v1.f_Pos[1] + m_M2.v_v1.f_Pos[1], m_M1.v_v1.f_Pos[2] + m_M2.v_v1.f_Pos[2]),
		V3(m_M1.v_v2.f_Pos[0] + m_M2.v_v2.f_Pos[0], m_M1.v_v2.f_Pos[1] + m_M2.v_v2.f_Pos[1], m_M1.v_v2.f_Pos[2] + m_M2.v_v2.f_Pos[2]),
		V3(m_M1.v_v3.f_Pos[0] + m_M2.v_v3.f_Pos[0], m_M1.v_v3.f_Pos[1] + m_M2.v_v3.f_Pos[1], m_M1.v_v3.f_Pos[2] + m_M2.v_v3.f_Pos[2])
	);
}

V3 MMultV(M3X3 m_M, V3 v_V)
{
	return V3(
		m_M.v_v1.f_Pos[0] * v_V.f_Pos[0] + m_M.v_v2.f_Pos[0] * v_V.f_Pos[1] + m_M.v_v3.f_Pos[0] * v_V.f_Pos[2],
		m_M.v_v1.f_Pos[1] * v_V.f_Pos[0] + m_M.v_v2.f_Pos[1] * v_V.f_Pos[1] + m_M.v_v3.f_Pos[1] * v_V.f_Pos[2],
		m_M.v_v1.f_Pos[2] * v_V.f_Pos[0] + m_M.v_v2.f_Pos[2] * v_V.f_Pos[1] + m_M.v_v3.f_Pos[2] * v_V.f_Pos[2]
	);
}

M3X3 M3X3RotU(V3 v_UnitV, float f_phi)
{
	float f_sin = sin(f_phi);
	float f_cos = cos(f_phi);
	//Rodrigues' Rotation Formula
	return M3X3(
		V3(
			f_cos + v_UnitV.f_Pos[X]* v_UnitV.f_Pos[X]*(1.0f- f_cos),
			v_UnitV.f_Pos[Z]* f_sin + v_UnitV.f_Pos[X]* v_UnitV.f_Pos[Y]*(1.0f- f_cos),
			-v_UnitV.f_Pos[Y]* f_sin + v_UnitV.f_Pos[X]* v_UnitV.f_Pos[Z]*(1.0f- f_cos)
		),
		V3(
			v_UnitV.f_Pos[X]* v_UnitV.f_Pos[Y]*(1.0f- f_cos)- v_UnitV.f_Pos[Z]* f_sin,
			f_cos + v_UnitV.f_Pos[Y]* v_UnitV.f_Pos[Y]*(1.0f- f_cos),
			v_UnitV.f_Pos[X]* f_sin + v_UnitV.f_Pos[Y]* v_UnitV.f_Pos[Z]*(1.0f- f_cos)
		),
		V3(
			v_UnitV.f_Pos[Y]* f_sin + v_UnitV.f_Pos[X]* v_UnitV.f_Pos[Z]*(1.0f- f_cos),
			-v_UnitV.f_Pos[X]* f_sin + v_UnitV.f_Pos[Y]* v_UnitV.f_Pos[Z]*(1.0f- f_cos),
			f_cos + v_UnitV.f_Pos[Z]* v_UnitV.f_Pos[Z]*(1.0f- f_cos)
		)
	);
}

M3X3 operator*(M3X3 & m_M, float & f_S)
{
	return MMultS(m_M,f_S);
}
M3X3 operator+(M3X3 & m_M1, M3X3 & m_M2)
{
	return MAddM(m_M1, m_M2);
}
M3X3 operator*(M3X3 & m_M1, M3X3 & m_M2)
{
	return MMultM(m_M1, m_M2);
}

V3 operator*(M3X3 & m_M, V3 & v_V)
{
	return MMultV(m_M,v_V);
}

M3X3 operator*(M3X3 const& m_M, float const& f_S)
{
	return MMultS(m_M, f_S);
}
M3X3 operator+(M3X3 const& m_M1, M3X3 const& m_M2)
{
	return MAddM(m_M1, m_M2);
}
M3X3 operator*(M3X3 const& m_M1, M3X3 const& m_M2)
{
	return MMultM(m_M1, m_M2);
}

V3 operator*(M3X3 const& m_M, V3 const& v_V)
{
	return MMultV(m_M, v_V);
}

void o_m3x3::print()
{
	printf("%f\t %f\t %f\n"  ,this->v_v1.f_Pos[0], this->v_v2.f_Pos[0], this->v_v3.f_Pos[0]);
	printf("%f\t %f\t %f\n"  ,this->v_v1.f_Pos[1], this->v_v2.f_Pos[1], this->v_v3.f_Pos[1]);
	printf("%f\t %f\t %f\n\n",this->v_v1.f_Pos[2], this->v_v2.f_Pos[2], this->v_v3.f_Pos[2]);
}

M3X3 M3X3RotX(float f_phi)
{
	return M3X3(
		V3(1,0,0),
		V3(0,cos(f_phi),sin(f_phi)),
		V3(0,-sin(f_phi),cos(f_phi))
	);
}
M3X3 M3X3RotY(float f_phi)
{
	return M3X3(
		V3(cos(f_phi), 0, -sin(f_phi)),
		V3(0, 1.0, 0),
		V3(sin(f_phi), 0, cos(f_phi))
	);
}

M3X3 M3X3RotZ(float f_phi)
{
	return M3X3(
		V3(cos(f_phi), sin(f_phi), 0),
		V3(-sin(f_phi), cos(f_phi), 0),
		V3(0, 0, 1)
	);
}