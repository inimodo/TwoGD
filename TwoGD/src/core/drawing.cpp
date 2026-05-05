#include "..\twogd.h"

canvas::canvas()
{
	Dispose();
}

canvas::canvas(uint32_t i_Width, uint32_t i_Height)
{
	i_OutputSize = i_Width * i_Height;
	d_pOutputStream = (DWORD*)malloc(sizeof(DWORD) * i_OutputSize);
	d_pPixelFlags = (uint8_t*)malloc(sizeof(uint8_t) * i_OutputSize);
	d_pPrioFlags = (uint8_t*)malloc(sizeof(uint8_t) * i_OutputSize);

	if (d_pOutputStream == NULL || d_pPixelFlags == NULL || d_pPrioFlags == NULL)
	{
		Dispose();
		return;
	}
	i_Pixels[Y] = i_Height;
	i_Pixels[X] = i_Width;

	CleanBuffer();
}

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

void canvas::Dispose()
{
	i_Pixels[X] = 0;
	i_Pixels[Y] = 0;
	i_OutputSize = 0;
	if (d_pOutputStream != NULL) 
	{
		free(d_pOutputStream);
		d_pOutputStream = NULL;
	}
	if (d_pPixelFlags != NULL)
	{
		free(d_pPixelFlags);
		d_pPixelFlags = NULL;
	}
	if (d_pPrioFlags != NULL)
	{
		free(d_pPrioFlags);
		d_pPrioFlags = NULL;
	}
}

uint8_t canvas::CleanBuffer()
{
	if (d_pOutputStream == NULL || d_pPixelFlags == NULL || d_pPrioFlags == NULL)
	{
		Dispose();
		return GD_ALLOC_FAILED;
	}
	memset((void*)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
	memset((void*)d_pPixelFlags, PF_OVERWRITE_ALLOWED, i_OutputSize * sizeof(uint8_t));
	memset((void*)d_pPrioFlags, 255, i_OutputSize * sizeof(uint8_t));
	return GD_TASK_OKAY;

}


