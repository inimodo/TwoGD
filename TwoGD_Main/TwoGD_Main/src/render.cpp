#include <iostream>
#include "twogd.h"

UCHAR 
codec2d::SetPixel(GDVEC2 * p_pPoint, GDCOLOR * c_pColor)
{
	if (p_pPoint->f_Pos[0] <= 0 || p_pPoint->f_Pos[0] >= this->gd_Image->i_Pixels[0])return GD_OUTOFBOUND;
	if (p_pPoint->f_Pos[1] <= 0 || p_pPoint->f_Pos[1] >= this->gd_Image->i_Pixels[1])return GD_OUTOFBOUND;
	gd_Image->d_pOutputStream[_TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1])] = c_pColor->GetAsHex();
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawLine(GDVEC2 * p_pPointA, GDVEC2 * p_pPointB, GDCOLOR * c_pColor)
{
	GDVEC2 p_Delta = (*p_pPointA) - (*p_pPointB), p_Temp;
	__REGISTER float f_Idi, f_Max = sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1]);
	if (f_Max > this->gd_Image->i_Pixels[0] + this->gd_Image->i_Pixels[1])return GD_OUTOFBOUND;

	for (f_Idi = 0; f_Idi < f_Max; f_Idi++)
	{
		p_Temp.f_Pos[0] = (f_Idi / f_Max) * p_Delta.f_Pos[0] + p_pPointB->f_Pos[0];
		p_Temp.f_Pos[1] = (f_Idi / f_Max) * p_Delta.f_Pos[1] + p_pPointB->f_Pos[1];
		SetPixel(&p_Temp, c_pColor);
	}
	return GD_TASK_OKAY;

}
UCHAR 
codec2d::DrawRect(GDVEC2* p_pPointA, GDVEC2* p_pPointB, GDCOLOR* c_pColor)
{
	GDVEC2 p_Temp(p_pPointA->f_Pos[0], p_pPointA->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < p_pPointB->f_Pos[1] - p_pPointA->f_Pos[1]; i_Index++)
	{
		p_Temp.f_Pos[1] = p_pPointA->f_Pos[1] + i_Index;
		DrawHLine(&p_Temp, p_pPointB->f_Pos[0] - p_pPointA->f_Pos[0], c_pColor);
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawHLine(GDVEC2* p_pPoint, UINT32  i_Length, GDCOLOR* c_pColor)
{
	__REGISTER int i_rIndex = _TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		gd_Image->d_pOutputStream[i_Index + i_rIndex] = c_pColor->GetAsHex();
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVLine(GDVEC2* p_pPoint, UINT32  i_Length, GDCOLOR* c_pColor)
{
	__REGISTER int i_rIndex = _TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		gd_Image->d_pOutputStream[gd_Image->i_Pixels[0] * i_Index + i_rIndex] = c_pColor->GetAsHex();
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawCanvas(DWORD * d_pBuffer, GDVEC2 * p_pPos, UINT32  i_Pixel[2])
{
	__REGISTER int i_rIndex = _TOINDEX(p_pPos->f_Pos[0], p_pPos->f_Pos[1]);
	for (UINT32 i_Row = 0; i_Row < i_Pixel[1]; i_Row++)
	{
		memmove((void*)(gd_Image->d_pOutputStream+ (i_rIndex + (i_Row*gd_Image->i_Pixels[0]))), (void*)(d_pBuffer+ i_Row * i_Pixel[0]), i_Pixel[0]* sizeof(DWORD));
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVMap(GFVECTORMAP * gd_VecMap)
{
	for (UINT32 i_Index = 0; i_Index < gd_VecMap->i_Connections; i_Index++)
	{
		DrawLine(&gd_VecMap->l_pLines[i_Index].p_Point[0], &gd_VecMap->l_pLines[i_Index].p_Point[1], &gd_VecMap->l_pLines[i_Index].c_Color);
	}
	return GD_TASK_OKAY;
}



UCHAR 
camera::Translate(GDVEC3 * p_pPoint,GDVEC2 * p_pResult) 
{
	GDVEC3 o_Distance = this->i_Position - *p_pPoint;
	float f_distance = o_Distance.Length();
	float f_updown = asin(o_Distance.f_Pos[1] /f_distance ) + this->i_Rotation.f_Pos[1];
	float f_leftright = asin(o_Distance.f_Pos[0] / o_Distance.f_Pos[2]) + this->i_Rotation.f_Pos[0];

	if (f_distance > this->i_Frustum[1] || f_distance < this->i_Frustum[0])return GD_OUTOFBOUND;
	if (o_Distance.f_Pos[2]<0)return GD_OUTOFBOUND;

	p_pResult->f_Pos[0] = tan(f_leftright)*this->i_Dimensions[0];
	p_pResult->f_Pos[1] = (tan(f_updown) / cos(f_leftright))*this->i_Dimensions[0];

	p_pResult->f_Pos[0] += this->i_Dimensions[0] / 2;
	p_pResult->f_Pos[1] += this->i_Dimensions[1] / 2;	

	return GD_TASK_OKAY;

}


UCHAR 
codec3d::DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor) 
{
	for (INT i_Vertex = 0; i_Vertex < gd_Object->i_Faces; i_Vertex++)
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
	this->DrawLine(&p_PointA, &p_PointB, c_pColor);
}