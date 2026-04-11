#include "..\twogd.h"

#define VINDEX(v2) ((int)v2->f_Pos[1] * o_Image->i_Pixels[0] + (int)v2->f_Pos[0])

inline UCHAR 
codec2d::SetPixel(V2 * v_pPoint, COLOR * c_pColor, UCHAR i_PixelFlag,UCHAR i_PrioFlag)
{
	if (v_pPoint->f_Pos[0] <= 0 || v_pPoint->f_Pos[0] >= this->o_Image->i_Pixels[0])return GD_OUTOFBOUND;
	if (v_pPoint->f_Pos[1] <= 0 || v_pPoint->f_Pos[1] >= this->o_Image->i_Pixels[1])return GD_OUTOFBOUND;
	if (o_Image->d_pOutputStream[VINDEX(v_pPoint)] != 0x0 && (
		b_AllowPixelOverwrite == FALSE || 
		o_Image->d_pPixelFlags[VINDEX(v_pPoint)] == PF_OVERWRITE_FORBIDDEN
		))return GD_OUTOFBOUND;
	if(o_Image->d_pPrioFlags[VINDEX(v_pPoint)] < i_PrioFlag)return GD_OUTOFBOUND;

	o_Image->d_pPrioFlags[VINDEX(v_pPoint)] = i_PrioFlag;
	o_Image->d_pPixelFlags[VINDEX(v_pPoint)] = i_PixelFlag;
	o_Image->d_pOutputStream[VINDEX(v_pPoint)] = c_pColor->GetAsHex();
	return GD_TASK_OKAY;
}

UCHAR
codec2d::DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	V2 v_Delta = (*v_pPointA) - (*v_pPointB), v_Temp;
	if (v_Delta.f_Pos[Y] == 0) 
	{
		if (v_Delta.f_Pos[X] > 0) 
		{
			return this->DrawHLine(v_pPointB, (UINT32)v_Delta.f_Pos[X],c_pColor,i_PixelFlag,i_PrioFlag);
		}
		return this->DrawHLine(v_pPointA, (UINT32)-v_Delta.f_Pos[X], c_pColor, i_PixelFlag, i_PrioFlag);

	}
	if (v_Delta.f_Pos[X] == 0)
	{
		if (v_Delta.f_Pos[Y] > 0)
		{
			return this->DrawVLine(v_pPointB, (UINT32)v_Delta.f_Pos[Y], c_pColor, i_PixelFlag, i_PrioFlag);
		}
		return this->DrawVLine(v_pPointA, (UINT32)-v_Delta.f_Pos[Y], c_pColor, i_PixelFlag, i_PrioFlag);
	}
	__REGISTER float f_Idi, f_Max = (float)v_Delta.Length();
	if (f_Max > this->o_Image->i_Pixels[0] + this->o_Image->i_Pixels[1])return GD_OUTOFBOUND;

	for (f_Idi = 0; f_Idi < f_Max; f_Idi++)
	{
		v_Temp.f_Pos[0] = (f_Idi / f_Max) * v_Delta.f_Pos[0] + v_pPointB->f_Pos[0];
		v_Temp.f_Pos[1] = (f_Idi / f_Max) * v_Delta.f_Pos[1] + v_pPointB->f_Pos[1];
		
		if (v_Temp.f_Pos[0] <= 0 || v_Temp.f_Pos[0] >= this->o_Image->i_Pixels[0])continue;
		if (v_Temp.f_Pos[1] <= 0 || v_Temp.f_Pos[1] >= this->o_Image->i_Pixels[1])continue;
		if (o_Image->d_pOutputStream[VINDEX((&v_Temp))] != 0x0 && (
			b_AllowPixelOverwrite == FALSE ||
			o_Image->d_pPixelFlags[VINDEX((&v_Temp))] == PF_OVERWRITE_FORBIDDEN
			))continue;
		if (o_Image->d_pPrioFlags[VINDEX((&v_Temp))] < i_PrioFlag)continue;

		o_Image->d_pPrioFlags[VINDEX((&v_Temp))] = i_PrioFlag;
		o_Image->d_pPixelFlags[VINDEX((&v_Temp))] = i_PixelFlag;
		o_Image->d_pOutputStream[VINDEX((&v_Temp))] = c_pColor->GetAsHex();
	}
	return GD_TASK_OKAY;
}

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
	__REGISTER UINT32 i_rIndex = VINDEX(v_pPoint), i_Pos = 0;
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		i_Pos = i_Index + i_rIndex;
		if (i_Pos >= this->o_Image->i_OutputSize || i_Pos < 0) continue;
		o_Image->d_pOutputStream[i_Pos] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[i_Pos] = i_PixelFlag;
		o_Image->d_pPrioFlags[i_Pos] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVLine(V2* v_pPoint, UINT32  i_Length, COLOR* c_pColor, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	__REGISTER UINT32 i_rIndex = VINDEX(v_pPoint), i_Pos = 0;
	for (UINT32 i_Index = 0; i_Index < i_Length; i_Index++)
	{
		i_Pos = o_Image->i_Pixels[0] * i_Index + i_rIndex;
		if (i_Pos >= this->o_Image->i_OutputSize || i_Pos < 0) continue;
		o_Image->d_pOutputStream[i_Pos] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[i_Pos] = i_PixelFlag;
		o_Image->d_pPrioFlags[i_Pos] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawCanvas(DWORD * d_pBuffer, V2 * v_pPos, UINT32  i_Pixel[2], UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	__REGISTER int i_rIndex = VINDEX(v_pPos);
	for (UINT32 i_Row = 0; i_Row < i_Pixel[1]; i_Row++)
	{
		memmove((void*)(o_Image->d_pOutputStream+ (i_rIndex + (i_Row*o_Image->i_Pixels[0]))), (void*)(d_pBuffer+ i_Row * i_Pixel[0]), i_Pixel[0]* sizeof(DWORD));
	}
	return GD_TASK_OKAY;
}
UCHAR 
codec2d::DrawVMap(VMAP * o_VecMap,V2* v_pAnchor, float f_Scale, UCHAR i_PixelFlag, UCHAR i_PrioFlag)
{
	for (UINT32 i_Index = 0; i_Index < o_VecMap->i_Connections; i_Index++)
	{
		V2 v_A = o_VecMap->l_pLines[i_Index].v_Point[0] * f_Scale;
		V2 v_B =o_VecMap->l_pLines[i_Index].v_Point[1] * f_Scale;
		v_A = v_A + (*v_pAnchor);
		v_B = v_B + (*v_pAnchor);
		DrawLine(&v_A, &v_B, &o_VecMap->l_pLines[i_Index].c_Color, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}





