#include "twogd.h"

camera::camera() 
{
	f_FOV = 2;
	f_Frustum[0] = 30;
	f_Frustum[1] = 1;
	i_Dimensions[0] = 16*50;
	i_Dimensions[1] = 9*50;
	i_Position = V3(0, 0, 0);
	i_Rotation = V3(0, 0, 0);
	V2 v_ScreenPos = V2((float)i_Dimensions[0], (float)i_Dimensions[1]);
	V3 v_Angles = V3(0, 0, 0);
	f_CutoffAngles[0] = v_Angles.f_Pos[X];
	f_CutoffAngles[1] = v_Angles.f_Pos[Y];
}
camera::camera(FLOAT f_Frustum_Low_, FLOAT f_Frustum_Far_, UINT32 i_Dimensions_Width_, UINT32 i_Dimensions_Height_, FLOAT f_FOV_, V3 i_Position_, V3 i_Rotation_)
{
	f_FOV = f_FOV_;
	f_Frustum[0] = f_Frustum_Low_;
	f_Frustum[1] = f_Frustum_Far_;
	i_Dimensions[0] = i_Dimensions_Width_;
	i_Dimensions[1] = i_Dimensions_Height_;
	i_Position = i_Position_;
	i_Rotation = i_Rotation_;

	V2 v_ScreenPos = V2((float)i_Dimensions[0], (float)i_Dimensions[1]);
	V3 v_Angles = V3(0, 0, 0);
	Relate(&v_ScreenPos,&v_Angles);
	f_CutoffAngles[0] = v_Angles.f_Pos[X] + DEGTORAD(5);
	f_CutoffAngles[1] = v_Angles.f_Pos[Y] + DEGTORAD(10);
}

inline UCHAR
camera::Translate(V3 * v_pPoint, V2 * v_pResult)
{
	V3 o_Delta = *v_pPoint - this->i_Position;
	float f_abs = o_Delta.Length();

	o_Delta.CamRotateThis(this->i_Rotation);
	
	if (o_Delta.f_Pos[2] < 0) return GD_OUTOFBOUND;
	if (f_abs > this->f_Frustum[1] || f_abs < this->f_Frustum[0]) return GD_OUTOFBOUND;

	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	if ((b_UseCutoffAngles == TRUE)&&
		(abs(f_leftright) > this->f_CutoffAngles[0] || abs(f_updown) > this->f_CutoffAngles[1])) return GD_OUTOFBOUND;

	float f_standoff = this->i_Dimensions[0]/this->f_FOV;
	v_pResult->f_Pos[0] = tan(f_leftright)*f_standoff;
	v_pResult->f_Pos[1] = (tan(f_updown) / cos(f_leftright))*f_standoff;

	v_pResult->f_Pos[1] *= -1; // Flip because y=0 is on top
	v_pResult->f_Pos[0] += this->i_Dimensions[0] / 2.0f;
	v_pResult->f_Pos[1] += this->i_Dimensions[1] / 2.0f;

	return GD_TASK_OKAY;
}

UCHAR
camera::Relate(V2 * v_pScreenPos, V3 * v_pAngle) 
{
	float f_standoff = this->i_Dimensions[0] / this->f_FOV;
	V2 f_Input = V2(
		(v_pScreenPos->f_Pos[0] - (this->i_Dimensions[0] / 2.0f)) /  f_standoff,
		(v_pScreenPos->f_Pos[1] - (this->i_Dimensions[1] / 2.0f)) / -f_standoff
	);

	v_pAngle->f_Pos[0] = atan(f_Input.f_Pos[0]);
	v_pAngle->f_Pos[1] = -atan(f_Input.f_Pos[1] * cos(v_pAngle->f_Pos[0]));

	return GD_TASK_OKAY;
}


UCHAR
codec3d::DrawObject(OBJ3D * o_Object, COLOR * c_pColor)
{
	for (UINT32 i_Vertex = 0; i_Vertex < o_Object->i_Faces; i_Vertex++)
	{
		V2 v_PointA, v_PointB;
		V3 v_Coord;
		for (INT i_Index = 0; i_Index < 3; i_Index++)
		{
			v_Coord = o_Object->o_pFace[i_Vertex].v_Point[i_Index] + o_Object->v_Anchor;
			if (this->o_Camera->Translate(&v_Coord, &v_PointA) == GD_OUTOFBOUND)continue;
			v_Coord = o_Object->o_pFace[i_Vertex].v_Point[(i_Index + 1) % 3] + o_Object->v_Anchor;
			if (this->o_Camera->Translate(&v_Coord, &v_PointB) == GD_OUTOFBOUND)continue;
			this->DrawLine(&v_PointA, &v_PointB, c_pColor);
		}
	}
	return GD_TASK_OKAY;
}



UCHAR
codec3d::DrawEdge(V3 * v_pVertexA, V3 * v_pVertexB, COLOR * c_pColor)
{
	V2 v_PointA, v_PointB;
	if (this->o_Camera->Translate(v_pVertexA, &v_PointA) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	if (this->o_Camera->Translate(v_pVertexB, &v_PointB) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	return this->DrawLine(&v_PointA, &v_PointB, c_pColor);
	return 0;
}

//UCHAR
//codec3d::DrawObject(OBJ3D * o_Object, COLOR * c_pColor)
//{
//	for (INT i_Vertex = 0; i_Vertex < o_Object->i_Faces; i_Vertex++)
//	{
//		V2 v_PointA, v_PointB;
//		V3 v_Coord;
//		for (INT i_Index = 0; i_Index < 3; i_Index++)
//		{
//			COLOR col = *c_pColor;
//			v_Coord = o_Object->o_pFace[i_Vertex].v_Point[i_Index] + o_Object->v_Anchor;
//			if (this->o_Camera->Translate(&v_Coord, &v_PointA) == 1)col = COLOR(255, 255, 255);
//			v_Coord = o_Object->o_pFace[i_Vertex].v_Point[(i_Index + 1) % 3] + o_Object->v_Anchor;
//			if (this->o_Camera->Translate(&v_Coord, &v_PointB) == 1)col = COLOR(255, 255, 255);
//			this->DrawLine(&v_PointA, &v_PointB, &col);
//		}
//	}
//	return GD_TASK_OKAY;
//}
//UCHAR

//codec3d::DrawEdge(V3 * v_pVertexA, V3 * v_pVertexB, COLOR * c_pColor)
//{
//	V2 v_PointA, v_PointB;
//	COLOR col = *c_pColor;
//
//	if (this->o_Camera->Translate(v_pVertexA, &v_PointA) == 1)col = COLOR(255, 255, 255);
//	if (this->o_Camera->Translate(v_pVertexB, &v_PointB) == 1)col = COLOR(255, 255, 255);
//	return this->DrawLine(&v_PointA, &v_PointB, &col);
//}