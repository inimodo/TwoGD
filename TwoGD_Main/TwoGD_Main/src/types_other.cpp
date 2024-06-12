#include "twogd.h"
gd_color::gd_color()
{
	c_Color[0] = 0;
	c_Color[1] = 0;
	c_Color[2] = 0;
}
DWORD gd_color::GetAsHex()
{
	return RGB(c_Color[0], c_Color[1], c_Color[2]);
}
gd_color::gd_color(UCHAR c_R, UCHAR c_G, UCHAR c_B)
{
	c_Color[0] = c_R;
	c_Color[1] = c_G;
	c_Color[2] = c_B;
}

UCHAR
canvas::Dispose()
{
	free(d_pOutputStream);
	return GD_TASK_OKAY;

}
UCHAR
canvas::CleanBuffer()
{
	memset((void *)d_pOutputStream, 0, i_OutputSize * sizeof(DWORD));
	return GD_TASK_OKAY;

}
UCHAR
canvas::Prepare(int i_Width, int i_Height)
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

