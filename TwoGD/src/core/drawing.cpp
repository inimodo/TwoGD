#include "..\twogd.h"
color::color()
{
	c_Color[0] = 0;
	c_Color[1] = 0;
	c_Color[2] = 0;
}
DWORD color::GetAsHex()
{
	return RGB(c_Color[0], c_Color[1], c_Color[2]);
}
color::color(uint8_t c_R, uint8_t c_G, uint8_t c_B)
{
	c_Color[0] = c_R;
	c_Color[1] = c_G;
	c_Color[2] = c_B;
}

uint8_t
canvas::Dispose()
{
	free(d_pOutputStream);
	free(d_pPixelFlags);
	free(d_pPrioFlags);
	return GD_TASK_OKAY;

}
uint8_t
canvas::CleanBuffer()
{
	memset((void *)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
	memset((void *)d_pPixelFlags, PF_OVERWRITE_ALLOWED , i_OutputSize * sizeof(uint8_t));
	memset((void *)d_pPrioFlags, 255, i_OutputSize * sizeof(uint8_t));
	return GD_TASK_OKAY;

}
uint8_t
canvas::Prepare(int i_Width, int i_Height)
{
	i_OutputSize = i_Width * i_Height;
	d_pOutputStream = (DWORD *)malloc(sizeof(DWORD)*i_OutputSize);
	d_pPixelFlags = (uint8_t *)malloc(sizeof(uint8_t)*i_OutputSize);
	d_pPrioFlags = (uint8_t *)malloc(sizeof(uint8_t)*i_OutputSize);
	
	if (d_pOutputStream == NULL || d_pPixelFlags == NULL || d_pPrioFlags == NULL) 
	{
		if (d_pOutputStream != NULL) free(d_pOutputStream);
		if (d_pPixelFlags != NULL) free(d_pPixelFlags);
		if (d_pPrioFlags != NULL) free(d_pPrioFlags);
		return GD_ALLOC_FAILED;
	}
	i_Pixels[1] = i_Height;
	i_Pixels[0] = i_Width;

	return CleanBuffer();
}

