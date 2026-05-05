#include "..\twogd.h"

vectormap::vectormap()
{
	b_Loaded = FALSE;
	f_Stream = NULL;
}

vectormap::vectormap(LPSTR c_StreamName)
{
	f_Stream = NULL;
	i_Points = 0;
	if (Read(c_StreamName) != GD_TASK_OKAY)
	{
		Dispose();
		return;
	}
	b_Loaded = TRUE;
}

uint8_t vectormap::Read(LPSTR c_StreamName)
{
	if (OpenStream(c_StreamName) != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}

	if (ReadHeader() != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}

	if (Prepare() != GD_TASK_OKAY) {
		return GD_ALLOC_FAILED;
	}

	if (LoadFile() != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}

uint8_t vectormap::LoadFile()
{
	uint32_t i_MaxX = 0;
	uint32_t i_MaxY = 0;
	for (uint32_t i_Index = 0; i_Index < i_Points; i_Index++)
	{
		if (fscanf(f_Stream, GD_VEC_POINTS, &v_pPoint[i_Index].f_Pos[X], &v_pPoint[i_Index].f_Pos[Y]) == NULL)
		{
			return GD_FILE_FAILED;
		}
		if (v_pPoint[i_Index].f_Pos[X] > i_MaxX)
		{
			i_MaxX = (uint32_t)v_pPoint[i_Index].f_Pos[X];
		}
		if (v_pPoint[i_Index].f_Pos[Y] > i_MaxY)
		{
			i_MaxY = (uint32_t)v_pPoint[i_Index].f_Pos[Y];
		}
	}
	i_Height = i_MaxY;
	i_Width = i_MaxX;
	for (uint32_t i_Index = 0; i_Index < i_Colors; i_Index++)
	{
		if (fscanf(f_Stream, GD_VEC_COLORS,
			(int*)&(c_pColor[i_Index].c_Color[0]),
			(int*)&(c_pColor[i_Index].c_Color[1]),
			(int*)&(c_pColor[i_Index].c_Color[2])) == NULL)
		{
			return GD_FILE_FAILED;
		}
	}
	int i_Lone, i_Ltwo, i_Lcolor;
	for (uint32_t i_Index = 0; i_Index < i_Connections; i_Index++)
	{
		if (fscanf(f_Stream, GD_VEC_LINE, &i_Lone, &i_Ltwo, &i_Lcolor) == NULL)
		{
			return GD_FILE_FAILED;
		}

		l_pLines[i_Index].c_Color = c_pColor[i_Lcolor];
		l_pLines[i_Index].v_Point[0] = v_pPoint[i_Lone];
		l_pLines[i_Index].v_Point[1] = v_pPoint[i_Ltwo];
	}
	return GD_TASK_OKAY;
}

uint8_t vectormap::ReadHeader()
{
	if (fscanf(f_Stream, GD_VEC_HEADER, &i_Points, &i_Colors, &i_Connections) == NULL)
	{
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}

uint8_t vectormap::Prepare()
{
	v_pPoint = (V2*)malloc(sizeof(V2) * i_Points);
	l_pLines = (LINE*)malloc(sizeof(LINE) * i_Connections);
	c_pColor = (COLOR*)malloc(sizeof(COLOR) * i_Colors);

	if (v_pPoint == NULL || c_pColor == NULL || l_pLines == NULL) {
		Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

void vectormap::Dispose()
{
	b_Loaded = FALSE;
	if (c_pColor != NULL)
	{
		free(c_pColor);
	}
	if (v_pPoint != NULL)
	{
		free(v_pPoint);
	}
	if (l_pLines != NULL)
	{
		free(l_pLines);
	}
}
