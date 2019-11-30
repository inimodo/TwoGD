#include <iostream>
#include "twogd.h"
unsigned char display::Dispose()
{
	free(d_pOutputStream);
	return 0;
}
void display::CleanBuffer()
{
	memset((void *)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
}
_STATUS
display::Prepare(int i_Width, int i_Height)
{
	i_OutputSize = i_Width * i_Height;
	d_pOutputStream = (DWORD *)malloc(sizeof(DWORD)*i_OutputSize);
	if (d_pOutputStream == NULL) {
		return GD_ALLOC_FAILED;
	}
	i_Pixels[1] = i_Height;
	i_Pixels[0] = i_Width;
	return GD_TASK_OKAY;
}
_STATUS __stdcall
display::SetPixel(GDPOINT * p_pPoint, GDCOLOR * c_pColor)
{
	__REGISTER u_int i_rIndex = _TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1]);
	if (i_rIndex > 0 && i_rIndex < i_OutputSize) {
		this->d_pOutputStream[i_rIndex] = c_pColor->GetAsHex();
		return GD_TASK_OKAY;
	}
	return GD_OUTOFBOUND;
}
_STATUS __stdcall
display::DrawLine(GDPOINT * p_pPointA, GDPOINT * p_pPointB, GDCOLOR * c_pColor)
{
	GDPOINT p_Delta = (*p_pPointA) - (*p_pPointB), p_Temp;
	__REGISTER float f_Idi, f_Max = sqrt(p_Delta.f_Pos[0] * p_Delta.f_Pos[0] + p_Delta.f_Pos[1] * p_Delta.f_Pos[1]);
	for (f_Idi = 0; f_Idi < f_Max; f_Idi++)
	{
		p_Temp.f_Pos[0] = (f_Idi / f_Max) * p_Delta.f_Pos[0] + p_pPointB->f_Pos[0];
		p_Temp.f_Pos[1] = (f_Idi / f_Max) * p_Delta.f_Pos[1] + p_pPointB->f_Pos[1];
		SetPixel(&p_Temp, c_pColor);
	}
	return GD_TASK_OKAY;

}
_STATUS __stdcall
display::DrawRect(GDPOINT* p_pPointA, GDPOINT* p_pPointB, GDCOLOR* c_pColor)
{
	GDPOINT p_Temp(p_pPointA->f_Pos[0], p_pPointA->f_Pos[1]);
	for (u_int i_Index = 0; i_Index < p_pPointB->f_Pos[1] - p_pPointA->f_Pos[1]; i_Index++)
	{
		p_Temp.f_Pos[1] = p_pPointA->f_Pos[1] + i_Index;
		DrawHLine(&p_Temp, p_pPointB->f_Pos[0] - p_pPointA->f_Pos[0], c_pColor);
	}
	return GD_TASK_OKAY;
}
_STATUS __stdcall
display::DrawHLine(GDPOINT* p_pPoint, u_int  i_Length, GDCOLOR* c_pColor)
{
	__REGISTER int i_rIndex = _TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1]);
	for (u_int i_Index = 0; i_Index < i_Length; i_Index++)
	{
		this->d_pOutputStream[i_Index + i_rIndex] = c_pColor->GetAsHex();
	}
	return GD_TASK_OKAY;
}
_STATUS __stdcall
display::DrawVLine(GDPOINT* p_pPoint, u_int  i_Length, GDCOLOR* c_pColor)
{
	__REGISTER int i_rIndex = _TOINDEX(p_pPoint->f_Pos[0], p_pPoint->f_Pos[1]);
	for (u_int i_Index = 0; i_Index < i_Length; i_Index++)
	{
		this->d_pOutputStream[this->i_Pixels[0] * i_Index + i_rIndex] = c_pColor->GetAsHex();
	}
	return GD_TASK_OKAY;
}