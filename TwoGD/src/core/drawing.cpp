#include "..\twogd.h"
o_color::o_color()
{
	c_Color[0] = 0;
	c_Color[1] = 0;
	c_Color[2] = 0;
}
DWORD o_color::GetAsHex()
{
	return RGB(c_Color[0], c_Color[1], c_Color[2]);
}
o_color::o_color(UCHAR c_R, UCHAR c_G, UCHAR c_B)
{
	c_Color[0] = c_R;
	c_Color[1] = c_G;
	c_Color[2] = c_B;
}

UCHAR
canvas::Dispose()
{
	free(d_pOutputStream);
	free(d_pPixelFlags);
	free(d_pPrioFlags);
	return GD_TASK_OKAY;

}
UCHAR
canvas::CleanBuffer()
{
	memset((void *)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
	memset((void *)d_pPixelFlags, PF_OVERWRITE_ALLOWED , i_OutputSize * sizeof(UCHAR));
	memset((void *)d_pPrioFlags, 255, i_OutputSize * sizeof(UCHAR));
	return GD_TASK_OKAY;

}
UCHAR
canvas::Prepare(int i_Width, int i_Height)
{
	i_OutputSize = i_Width * i_Height;
	d_pOutputStream = (DWORD *)malloc(sizeof(DWORD)*i_OutputSize);
	d_pPixelFlags = (UCHAR *)malloc(sizeof(UCHAR)*i_OutputSize);
	d_pPrioFlags = (UCHAR *)malloc(sizeof(UCHAR)*i_OutputSize);
	
	if (d_pOutputStream == NULL || d_pPixelFlags == NULL || d_pPrioFlags == NULL) 
	{
		if (d_pOutputStream != NULL) free(d_pOutputStream);
		if (d_pPixelFlags != NULL) free(d_pPixelFlags);
		if (d_pPrioFlags != NULL) free(d_pPrioFlags);
		return GD_ALLOC_FAILED;
	}
	i_Pixels[1] = i_Height;
	i_Pixels[0] = i_Width;

	return this->CleanBuffer();
}

