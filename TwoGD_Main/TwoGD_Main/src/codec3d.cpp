#include "twogd.h"


UCHAR
camera::Translate(GDVEC3 * p_pPoint, GDVEC2 * p_pResult)
{
	GDVEC3 o_Delta = *p_pPoint - this->i_Position;
	float f_abs = o_Delta.Length();

	o_Delta.RotateThis(this->i_Rotation);
	
	if (o_Delta.f_Pos[2] < 0) return GD_OUTOFBOUND;
	if (f_abs > this->f_Frustum[1] || f_abs < this->f_Frustum[0]) return GD_OUTOFBOUND;

	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	float f_standoff = this->i_Dimensions[0]/this->f_FOV;
	p_pResult->f_Pos[0] = tan(f_leftright)*f_standoff;
	p_pResult->f_Pos[1] = (tan(f_updown) / cos(f_leftright))*f_standoff;

	p_pResult->f_Pos[1] *= -1; // Flip because y=0 is on top
	p_pResult->f_Pos[0] += this->i_Dimensions[0] / 2.0;
	p_pResult->f_Pos[1] += this->i_Dimensions[1] / 2.0;

	return GD_TASK_OKAY;
}

UCHAR
camera::Relate(GDVEC2 * p_pScreenPos, GDVEC3 * p_pAngle) 
{
	float f_standoff = this->i_Dimensions[0] / this->f_FOV;
	GDVEC2 f_Input = GDVEC2(
		(p_pScreenPos->f_Pos[0] - (this->i_Dimensions[0] / 2.0)) /  f_standoff,
		(p_pScreenPos->f_Pos[1] - (this->i_Dimensions[1] / 2.0)) / -f_standoff
	);

	p_pAngle->f_Pos[0] = atan(f_Input.f_Pos[0]);
	p_pAngle->f_Pos[1] = -atan(f_Input.f_Pos[1] * cos(p_pAngle->f_Pos[0]));

	return GD_TASK_OKAY;
}


UCHAR
codec3d::DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor)
{
	for (UINT32 i_Vertex = 0; i_Vertex < gd_Object->i_Faces; i_Vertex++)
	{
		GDVEC2 p_PointA, p_PointB;
		GDVEC3 p_Coord;
		for (INT i_Index = 0; i_Index < 3; i_Index++)
		{
			p_Coord = gd_Object->o_pFace[i_Vertex].p_Point[i_Index] + gd_Object->p_Anchor;
			if (this->gd_Camera->Translate(&p_Coord, &p_PointA) == GD_OUTOFBOUND)continue;
			p_Coord = gd_Object->o_pFace[i_Vertex].p_Point[(i_Index + 1) % 3] + gd_Object->p_Anchor;
			if (this->gd_Camera->Translate(&p_Coord, &p_PointB) == GD_OUTOFBOUND)continue;
			this->DrawLine(&p_PointA, &p_PointB, c_pColor);
		}
	}
	return GD_TASK_OKAY;
}



UCHAR
codec3d::DrawEdge(GDVEC3 * p_pVertexA, GDVEC3 * p_pVertexB, GDCOLOR * c_pColor)
{
	GDVEC2 p_PointA, p_PointB;
	if (this->gd_Camera->Translate(p_pVertexA, &p_PointA) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	if (this->gd_Camera->Translate(p_pVertexB, &p_PointB) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	return this->DrawLine(&p_PointA, &p_PointB, c_pColor);
}

//UCHAR
//codec3d::DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor)
//{
//	for (INT i_Vertex = 0; i_Vertex < gd_Object->i_Faces; i_Vertex++)
//	{
//		GDVEC2 p_PointA, p_PointB;
//		GDVEC3 p_Coord;
//		for (INT i_Index = 0; i_Index < 3; i_Index++)
//		{
//			GDCOLOR col = *c_pColor;
//			p_Coord = gd_Object->o_pFace[i_Vertex].p_Point[i_Index] + gd_Object->p_Anchor;
//			if (this->gd_Camera->Translate(&p_Coord, &p_PointA) == 1)col = GDCOLOR(255, 255, 255);
//			p_Coord = gd_Object->o_pFace[i_Vertex].p_Point[(i_Index + 1) % 3] + gd_Object->p_Anchor;
//			if (this->gd_Camera->Translate(&p_Coord, &p_PointB) == 1)col = GDCOLOR(255, 255, 255);
//			this->DrawLine(&p_PointA, &p_PointB, &col);
//		}
//	}
//	return GD_TASK_OKAY;
//}
//UCHAR

//codec3d::DrawEdge(GDVEC3 * p_pVertexA, GDVEC3 * p_pVertexB, GDCOLOR * c_pColor)
//{
//	GDVEC2 p_PointA, p_PointB;
//	GDCOLOR col = *c_pColor;
//
//	if (this->gd_Camera->Translate(p_pVertexA, &p_PointA) == 1)col = GDCOLOR(255, 255, 255);
//	if (this->gd_Camera->Translate(p_pVertexB, &p_PointB) == 1)col = GDCOLOR(255, 255, 255);
//	return this->DrawLine(&p_PointA, &p_PointB, &col);
//}