#include "..\twogd.h"

camera::camera()
{
	f_FOV = 2;
	f_Frustum[0] = 30;
	f_Frustum[1] = 1;
	i_Dimensions[0] = 16 * 50;
	i_Dimensions[1] = 9 * 50;
	i_Position = V3(0, 0, 0);
	i_Rotation = V3(0, 0, 0);
	V2 v_ScreenPos = V2((float)i_Dimensions[0], (float)i_Dimensions[1]);
	V3 v_Angles = V3(0, 0, 0);
	f_CutoffAngles[0] = v_Angles.f_Pos[X];
	f_CutoffAngles[1] = v_Angles.f_Pos[Y];
}

camera::camera(FLOAT f_Frustum_Low_, FLOAT f_Frustum_Far_, uint32_t i_Dimensions_Width_, uint32_t i_Dimensions_Height_, FLOAT f_FOV_, V3 i_Position_, V3 i_Rotation_)
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
	Relate(&v_ScreenPos, &v_Angles);
	f_CutoffAngles[0] = v_Angles.f_Pos[X] + DEGTORAD(10);
	f_CutoffAngles[1] = v_Angles.f_Pos[Y] + DEGTORAD(10);
}

inline uint8_t camera::Translate(V3* v_pPoint, V2* v_pResult)
{
	V3 o_Delta = *v_pPoint - i_Position;
	float f_abs = o_Delta.Length();

	if (f_abs > f_Frustum[1] || f_abs < f_Frustum[0]) return GD_OUTOFBOUND;

	o_Delta.CamRotateThisOpt(i_Rotation);

	if (o_Delta.f_Pos[2] < 0) return GD_OUTOFBOUND;

	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	if ((b_UseCutoffAngles == TRUE) &&
		(abs(f_leftright) > f_CutoffAngles[0] || abs(f_updown) > f_CutoffAngles[1])) return GD_OUTOFBOUND;

	float f_standoff = i_Dimensions[0] / f_FOV;
	v_pResult->f_Pos[0] = tan(f_leftright) * f_standoff;
	v_pResult->f_Pos[1] = (tan(f_updown) / cos(f_leftright)) * f_standoff;

	v_pResult->f_Pos[1] *= -1; // Flip because y=0 is on top
	v_pResult->f_Pos[0] += i_Dimensions[0] / 2.0f;
	v_pResult->f_Pos[1] += i_Dimensions[1] / 2.0f;

	return GD_TASK_OKAY;
}

uint8_t camera::Relate(V2* v_pScreenPos, V3* v_pAngle)
{
	float f_standoff = i_Dimensions[0] / f_FOV;
	V2 f_Input = V2(
		(v_pScreenPos->f_Pos[0] - (i_Dimensions[0] / 2.0f)) / f_standoff,
		(v_pScreenPos->f_Pos[1] - (i_Dimensions[1] / 2.0f)) / -f_standoff
	);

	v_pAngle->f_Pos[0] = atan(f_Input.f_Pos[0]);
	v_pAngle->f_Pos[1] = -atan(f_Input.f_Pos[1] * cos(v_pAngle->f_Pos[0]));

	return GD_TASK_OKAY;
}

uint8_t codec3d::DrawObject(OBJ3D* o_Object, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	V2 v_PointA, v_PointB;
	V3 v_pVertexA, v_pVertexB;
	COLOR c_Color;
	for (uint32_t i_Vertex = 0; i_Vertex < o_Object->i_Faces; i_Vertex++)
	{
		for (INT i_Index = 0; i_Index < 3; i_Index++)
		{
			c_Color = *c_pColor;
			v_pVertexA = o_Object->o_pFace[i_Vertex].v_Point[i_Index] + o_Object->v_Anchor;
			if (o_Camera->Translate(&v_pVertexA, &v_PointA) == GD_OUTOFBOUND)continue;
			v_pVertexB = o_Object->o_pFace[i_Vertex].v_Point[(i_Index + 1) % 3] + o_Object->v_Anchor;
			if (o_Camera->Translate(&v_pVertexB, &v_PointB) == GD_OUTOFBOUND)continue;

			if (o_Camera->s_Shader != NULL)
			{
				if (o_Camera->s_Shader(o_Camera, &v_pVertexA, &v_PointA, &c_Color) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
				if (o_Camera->s_Shader(o_Camera, &v_pVertexB, &v_PointB, &c_Color) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
			}
			DrawLine(&v_PointA, &v_PointB, &c_Color, i_PixelFlag, i_PrioFlag);
		}
	}
	return GD_TASK_OKAY;
}

uint8_t codec3d::DrawEdge(V3* v_pVertexA, V3* v_pVertexB, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	V2 v_PointA, v_PointB;
	COLOR c_Color = *c_pColor;
	if (o_Camera->Translate(v_pVertexA, &v_PointA) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	if (o_Camera->Translate(v_pVertexB, &v_PointB) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	if (o_Camera->s_Shader != NULL)
	{
		if (o_Camera->s_Shader(o_Camera, v_pVertexA, &v_PointA, &c_Color) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
		if (o_Camera->s_Shader(o_Camera, v_pVertexB, &v_PointB, &c_Color) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
	}
	return DrawLine(&v_PointA, &v_PointB, &c_Color, i_PixelFlag, i_PrioFlag);
}

uint8_t codec3d::DrawFace(FACE* o_Face, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	int i_YMin = 1000000000;
	int i_YMax = 0;
	LINE l_Lines[3];
	for (int i_Index = 0; i_Index < 3; i_Index++)
	{
		if (o_Camera->Translate(&o_Face->v_Point[i_Index], &l_Lines[i_Index].v_Point[0]) == GD_OUTOFBOUND)return GD_OUTOFBOUND;
		if (o_Camera->Translate(&o_Face->v_Point[(i_Index + 1) % 3], &l_Lines[i_Index].v_Point[1]) == GD_OUTOFBOUND)return GD_OUTOFBOUND;

		if (l_Lines[i_Index].v_Point[0].f_Pos[Y] > i_YMax)
		{
			i_YMax = (int)l_Lines[i_Index].v_Point[0].f_Pos[Y];
		}
		if (l_Lines[i_Index].v_Point[0].f_Pos[Y] < i_YMin)
		{
			i_YMin = (int)l_Lines[i_Index].v_Point[0].f_Pos[Y];
		}
	}

	V2 v_H, v_L, v_Point;
	for (int i_Y = i_YMin; i_Y < i_YMax; i_Y++)
	{
		float f_Edges[2];
		float f_EdgesSorted[2];
		int i_EdgeCount = 0;

		float f_Y = (i_Y + 0.01f);
		for (uint32_t i_Index = 0; i_Index < 3; i_Index++)
		{
			v_H = l_Lines[i_Index].v_Point[0];
			v_L = l_Lines[i_Index].v_Point[1];

			if (l_Lines[i_Index].v_Point[0].f_Pos[Y] < l_Lines[i_Index].v_Point[1].f_Pos[Y])
			{
				v_L = l_Lines[i_Index].v_Point[0];
				v_H = l_Lines[i_Index].v_Point[1];
			}

			if (!(v_H.f_Pos[Y] > f_Y && v_L.f_Pos[Y] < f_Y))
			{
				continue;
			}
			float f_k = (v_H.f_Pos[Y] - v_L.f_Pos[Y]) / (v_H.f_Pos[X] - v_L.f_Pos[X]);
			float f_d = v_H.f_Pos[Y] - v_H.f_Pos[X] * f_k;
			float f_X = (f_Y - f_d) / f_k;
			if (v_H.f_Pos[X] == v_L.f_Pos[X])
			{
				f_X = v_H.f_Pos[X];
			}
			f_Edges[i_EdgeCount] = f_X;
			i_EdgeCount++;
		}

		for (int i_Sort = 0; i_Sort < i_EdgeCount; i_Sort++)
		{
			int i_Highest = 0;
			for (int i_Cylce = 0; i_Cylce < i_EdgeCount; i_Cylce++)
			{
				if (f_Edges[i_Cylce] > f_Edges[i_Highest])
				{
					i_Highest = i_Cylce;
				}
			}
			f_EdgesSorted[i_EdgeCount - i_Sort - 1] = f_Edges[i_Highest];
			f_Edges[i_Highest] = 0;
		}
		if (i_EdgeCount % 2 != 0)
		{
			continue;
		}
		for (int i_Index = 0; i_Index < i_EdgeCount; i_Index += 2)
		{
			v_Point.f_Pos[Y] = f_Y;
			v_Point.f_Pos[X] = f_EdgesSorted[i_Index];
			DrawHLine(&v_Point, (uint32_t)(f_EdgesSorted[i_Index + 1] - f_EdgesSorted[i_Index]), c_pColor);
		}
	}
	return GD_TASK_OKAY;
}