#include "..\twogd.h"

#define VINDEX(v2) ((int)v2->f_Pos[1] * o_Image->i_Pixels[0] + (int)v2->f_Pos[0])

inline uint8_t 
codec2d::SetPixel(V2 * v_pPoint, COLOR * c_pColor, uint8_t i_PixelFlag,uint8_t i_PrioFlag)
{
	if (v_pPoint->f_Pos[0] <= 0 || v_pPoint->f_Pos[0] >= o_Image->i_Pixels[0])return GD_OUTOFBOUND;
	if (v_pPoint->f_Pos[1] <= 0 || v_pPoint->f_Pos[1] >= o_Image->i_Pixels[1])return GD_OUTOFBOUND;
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

uint8_t
codec2d::DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	V2 v_Delta = (*v_pPointA) - (*v_pPointB), v_Temp;
	if (v_Delta.f_Pos[Y] == 0) 
	{
		if (v_Delta.f_Pos[X] > 0) 
		{
			return DrawHLine(v_pPointB, (uint32_t)v_Delta.f_Pos[X],c_pColor,i_PixelFlag,i_PrioFlag);
		}
		return DrawHLine(v_pPointA, (uint32_t)-v_Delta.f_Pos[X], c_pColor, i_PixelFlag, i_PrioFlag);

	}
	if (v_Delta.f_Pos[X] == 0)
	{
		if (v_Delta.f_Pos[Y] > 0)
		{
			return DrawVLine(v_pPointB, (uint32_t)v_Delta.f_Pos[Y], c_pColor, i_PixelFlag, i_PrioFlag);
		}
		return DrawVLine(v_pPointA, (uint32_t)-v_Delta.f_Pos[Y], c_pColor, i_PixelFlag, i_PrioFlag);
	}
	__REGISTER float f_Idi, f_Max = (float)v_Delta.Length();
	if (f_Max > o_Image->i_Pixels[0] + o_Image->i_Pixels[1])return GD_OUTOFBOUND;

	for (f_Idi = 0; f_Idi < f_Max; f_Idi++)
	{
		v_Temp.f_Pos[0] = (f_Idi / f_Max) * v_Delta.f_Pos[0] + v_pPointB->f_Pos[0];
		v_Temp.f_Pos[1] = (f_Idi / f_Max) * v_Delta.f_Pos[1] + v_pPointB->f_Pos[1];
		
		if (v_Temp.f_Pos[0] <= 0 || v_Temp.f_Pos[0] >= o_Image->i_Pixels[0])continue;
		if (v_Temp.f_Pos[1] <= 0 || v_Temp.f_Pos[1] >= o_Image->i_Pixels[1])continue;
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

uint8_t 
codec2d::DrawRect(V2* v_pPointA, V2* v_pPointB, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	V2 v_Temp(v_pPointA->f_Pos[0], v_pPointA->f_Pos[1]);
	for (uint32_t i_Index = 0; i_Index < v_pPointB->f_Pos[1] - v_pPointA->f_Pos[1]; i_Index++)
	{
		v_Temp.f_Pos[1] = v_pPointA->f_Pos[1] + (float)i_Index;
		DrawHLine(&v_Temp,(uint32_t)( v_pPointB->f_Pos[0] - v_pPointA->f_Pos[0]), c_pColor, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}
uint8_t 
codec2d::DrawHLine(V2* v_pPoint, uint32_t  i_Length, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	__REGISTER uint32_t i_rIndex = VINDEX(v_pPoint), i_Pos = 0;
	for (uint32_t i_Index = 0; i_Index < i_Length; i_Index++)
	{
		i_Pos = i_Index + i_rIndex;
		if (i_Pos >= o_Image->i_OutputSize || i_Pos < 0) continue;
		o_Image->d_pOutputStream[i_Pos] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[i_Pos] = i_PixelFlag;
		o_Image->d_pPrioFlags[i_Pos] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
uint8_t 
codec2d::DrawVLine(V2* v_pPoint, uint32_t  i_Length, COLOR* c_pColor, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	__REGISTER uint32_t i_rIndex = VINDEX(v_pPoint), i_Pos = 0;
	for (uint32_t i_Index = 0; i_Index < i_Length; i_Index++)
	{
		i_Pos = o_Image->i_Pixels[0] * i_Index + i_rIndex;
		if (i_Pos >= o_Image->i_OutputSize || i_Pos < 0) continue;
		o_Image->d_pOutputStream[i_Pos] = c_pColor->GetAsHex();
		o_Image->d_pPixelFlags[i_Pos] = i_PixelFlag;
		o_Image->d_pPrioFlags[i_Pos] = i_PrioFlag;
	}
	return GD_TASK_OKAY;
}
uint8_t 
codec2d::DrawCanvas(DWORD * d_pBuffer, V2 * v_pPos, uint32_t  i_Pixel[2], uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	__REGISTER int i_rIndex = VINDEX(v_pPos);
	for (uint32_t i_Row = 0; i_Row < i_Pixel[1]; i_Row++)
	{
		memmove((void*)(o_Image->d_pOutputStream+ (i_rIndex + (i_Row*o_Image->i_Pixels[0]))), (void*)(d_pBuffer+ i_Row * i_Pixel[0]), i_Pixel[0]* sizeof(DWORD));
	}
	return GD_TASK_OKAY;
}
uint8_t 
codec2d::DrawVMap(VMAP * o_VecMap,V2* v_pAnchor, float f_Scale, uint8_t i_PixelFlag, uint8_t i_PrioFlag)
{
	for (uint32_t i_Index = 0; i_Index < o_VecMap->i_Connections; i_Index++)
	{
		V2 v_A = o_VecMap->l_pLines[i_Index].v_Point[0] * f_Scale;
		V2 v_B =o_VecMap->l_pLines[i_Index].v_Point[1] * f_Scale;
		v_A = v_A + (*v_pAnchor);
		v_B = v_B + (*v_pAnchor);
		DrawLine(&v_A, &v_B, &o_VecMap->l_pLines[i_Index].c_Color, i_PixelFlag, i_PrioFlag);
	}
	return GD_TASK_OKAY;
}
uint8_t
codec2d::DrawChar(CHARMAP* o_VecMap, V2* v_pAnchor, COLOR* c_pColor, float f_Scale)
{
	
	V2 v_H, v_L, v_Point;
	for (int i_Y = (int)(o_VecMap->i_YMin * f_Scale); i_Y < (int)(o_VecMap->i_YMax * f_Scale); i_Y++)
	{
		float f_Edges[128];
		float f_EdgesSorted[128];
		int i_EdgeCount = 0;

		float f_Y = (i_Y + 0.01f) ;
		for (uint32_t i_Index = 0; i_Index < o_VecMap->i_Count; i_Index++)
		{
			v_H = o_VecMap->l_pLines[i_Index].v_Point[0];
			v_L = o_VecMap->l_pLines[i_Index].v_Point[1];

			if (o_VecMap->l_pLines[i_Index].v_Point[0].f_Pos[Y] < o_VecMap->l_pLines[i_Index].v_Point[1].f_Pos[Y])
			{
				v_L = o_VecMap->l_pLines[i_Index].v_Point[0];
				v_H = o_VecMap->l_pLines[i_Index].v_Point[1];
			}

			v_L = v_L * f_Scale;
			v_H = v_H * f_Scale;

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
			f_EdgesSorted[i_EdgeCount-i_Sort-1] = f_Edges[i_Highest];
			f_Edges[i_Highest] = 0;
		}
		if (i_EdgeCount % 2 != 0) 
		{
			continue;
		}
		for (int i_Index = 0; i_Index < i_EdgeCount; i_Index+=2)
		{
			v_Point = *v_pAnchor;
			v_Point.f_Pos[Y] += f_Y;
			v_Point.f_Pos[X] += f_EdgesSorted[i_Index];
			DrawHLine(&v_Point, (uint32_t)(f_EdgesSorted[i_Index + 1] - f_EdgesSorted[i_Index]), c_pColor, PF_FONT, 0);
		}
	}

	return GD_TASK_OKAY;
}




