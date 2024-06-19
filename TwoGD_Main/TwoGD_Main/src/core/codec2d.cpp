#include "..\twogd.h"

UCHAR 
codec2d::SetPixel(V2 * v_pPoint, COLOR * c_pColor, UCHAR i_PixelFlag,UCHAR i_PrioFlag)
{
	if (v_pPoint->f_Pos[0] <= 0 || v_pPoint->f_Pos[0] >= this->o_Image->i_Pixels[0])return GD_OUTOFBOUND;
	if (v_pPoint->f_Pos[1] <= 0 || v_pPoint->f_Pos[1] >= this->o_Image->i_Pixels[1])return GD_OUTOFBOUND;
	if (o_Image->d_pOutputStream[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] != 0x0 && (
		b_AllowPixelOverwrite == FALSE || 
		o_Image->d_pPixelFlags[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] == PF_OVERWRITE_FORBIDDEN
		))return GD_OUTOFBOUND;
	if(o_Image->d_pPrioFlags[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] < i_PrioFlag)return GD_OUTOFBOUND;

	o_Image->d_pPrioFlags[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] = i_PrioFlag;
	o_Image->d_pPixelFlags[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] = i_PixelFlag;
	o_Image->d_pOutputStream[_TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1])] = c_pColor->GetAsHex();
	return GD_TASK_OKAY;
}

UCHAR
codec2d::DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	V2 v_Delta = (*v_pPointA) - (*v_pPointB), v_Temp;
	__REGISTER float f_Idi, f_Max = sqrt(v_Delta.f_Pos[0] * v_Delta.f_Pos[0] + v_Delta.f_Pos[1] * v_Delta.f_Pos[1]);
	if (f_Max > this->o_Image->i_Pixels[0] + this->o_Image->i_Pixels[1])return GD_OUTOFBOUND;

	for (f_Idi = 0; f_Idi < f_Max; f_Idi++)
	{
		v_Temp.f_Pos[0] = (f_Idi / f_Max) * v_Delta.f_Pos[0] + v_pPointB->f_Pos[0];
		v_Temp.f_Pos[1] = (f_Idi / f_Max) * v_Delta.f_Pos[1] + v_pPointB->f_Pos[1];
		SetPixel(&v_Temp, c_pColor, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}
//UCHAR 
//codec2d::DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag)
//{
//	V2 v_Delta = (*v_pPointB) - (*v_pPointA), v_Temp;
//	float K = v_Delta.f_Pos[Y] / v_Delta.f_Pos[X];
//
//	for (UINT32 i_X = 0; i_X < v_Delta.f_Pos[X]; i_X++)
//	{
//		float i_Y = floor(K * i_X);
//		v_Temp.f_Pos[X] = i_X+ (*v_pPointA).f_Pos[X];
//		v_Temp.f_Pos[Y] = i_Y + (*v_pPointA).f_Pos[Y];
//
//		this->SetPixel(&v_Temp, c_pColor, i_PixelFlag);
//	}
//
//	return GD_TASK_OKAY;
//}

UCHAR 
codec2d::DrawRect(V2* v_pPointA, V2* v_pPointB, COLOR* c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	V2 v_Temp(v_pPointA->f_Pos[0], v_pPointA->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < v_pPointB->f_Pos[1] - v_pPointA->f_Pos[1]; i_Index++)
	{
		v_Temp.f_Pos[1] = v_pPointA->f_Pos[1] + (float)i_Index;
		DrawHLine(&v_Temp,(UINT32)( v_pPointB->f_Pos[0] - v_pPointA->f_Pos[0]), c_pColor, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawHLine(V2* v_pPoint, UINT32  i_Length, COLOR* c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	__REGISTER int i_rIndex = _TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		o_Image->d_pOutputStream[i_Index + i_rIndex] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[i_Index + i_rIndex] = i_PixelFlag;
		o_Image->d_pPrioFlags[i_Index + i_rIndex] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVLine(V2* v_pPoint, UINT32  i_Length, COLOR* c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	__REGISTER int i_rIndex = _TOINDEX(v_pPoint->f_Pos[0], v_pPoint->f_Pos[1]);
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		o_Image->d_pOutputStream[o_Image->i_Pixels[0] * i_Index + i_rIndex] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[o_Image->i_Pixels[0] * i_Index + i_rIndex] = i_PixelFlag;
		o_Image->d_pPrioFlags[o_Image->i_Pixels[0] * i_Index + i_rIndex] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawCanvas(DWORD * d_pBuffer, V2 * v_pPos, UINT32  i_Pixel[2], UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	__REGISTER int i_rIndex = _TOINDEX(v_pPos->f_Pos[0], v_pPos->f_Pos[1]);
	for (UINT32 i_Row = 0; i_Row < i_Pixel[1]; i_Row++)
	{
		memmove((void*)(o_Image->d_pOutputStream+ (i_rIndex + (i_Row*o_Image->i_Pixels[0]))), (void*)(d_pBuffer+ i_Row * i_Pixel[0]), i_Pixel[0]* sizeof(DWORD));
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVMap(VMAP * o_VecMap, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	for (UINT32 i_Index = 0; i_Index < o_VecMap->i_Connections; i_Index++)
	{
		DrawLine(&o_VecMap->l_pLines[i_Index].v_Point[0], &o_VecMap->l_pLines[i_Index].v_Point[1], &o_VecMap->l_pLines[i_Index].c_Color, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}





