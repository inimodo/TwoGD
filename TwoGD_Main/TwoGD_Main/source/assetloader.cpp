#include "twogd.h"

#define GD_VEC_HEADER "v_%d_%d_%d\n"
#define GD_VEC_POINTS "p_%f_%f\n"
#define GD_VEC_COLORS "c_%d_%d_%d\n"
#define GD_VEC_LINE "l_%d_%d_%d\n"

__STATUS __WAY
mapformat::CloseStream() 
{
	if (this->f_Stream != NULL) 
	{
		return fclose(this->f_Stream);
	}
	return GD_TASK_OKAY;
}
__STATUS __WAY
mapformat::OpenStream(const char* c_StreamName)
{
	if (this->f_Stream == NULL) 
	{
		this->f_Stream = fopen(c_StreamName, "r");
		if (this->f_Stream == NULL)
		{
			return GD_FILE_FAILED;
		}
		return GD_TASK_OKAY;
	}
	return GD_FILE_FAILED;
}
__STATUS __WAY 
vectormap::Read(const char* c_StreamName)
{
	if (this->OpenStream(c_StreamName) != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}
	
	if (this->ReadHeader() != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}

	if (this->Prepare() != GD_TASK_OKAY) {
		return GD_ALLOC_FAILED;
	}

	if (this->LoadFile() != GD_TASK_OKAY) {
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}
__STATUS __WAY
vectormap::LoadFile()
{
	for (int i_Index = 0; i_Index < this->i_Points; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_POINTS, &this->p_pPoint[i_Index].f_Pos[0], &this->p_pPoint[i_Index].f_Pos[1]) == NULL)
		{
			return GD_FILE_FAILED;
		}
	}
	for (int i_Index = 0; i_Index < this->i_Colors; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_COLORS, &this->c_pColor[i_Index].c_Color[0], &this->c_pColor[i_Index].c_Color[1], &this->c_pColor[i_Index].c_Color[2]) == NULL)
		{
			return GD_FILE_FAILED;
		}
	}
	int i_Lone, i_Ltwo, i_Lcolor;
	for (int i_Index = 0; i_Index < this->i_Connections; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_LINE, &i_Lone, &i_Ltwo, &i_Lcolor) == NULL)
		{
			return GD_FILE_FAILED;
		}
		this->l_pLines[i_Index].c_Color = this->c_pColor[i_Lcolor];
		this->l_pLines[i_Index].p_Point[0] = this->p_pPoint[i_Lone];
		this->l_pLines[i_Index].p_Point[1] = this->p_pPoint[i_Ltwo];
	}
	return GD_TASK_OKAY;
}
__STATUS __WAY
vectormap::ReadHeader()
{
	if (fscanf(this->f_Stream, GD_VEC_HEADER, &this->i_Points, &this->i_Colors, &this->i_Connections) == NULL)
	{
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}
__STATUS __WAY
vectormap::Prepare()
{
	this->p_pPoint = (GDPOINT*)malloc(sizeof(GDPOINT)*this->i_Points);
	this->l_pLines = (GDLINE*)malloc(sizeof(GDLINE)*this->i_Connections);
	this->c_pColor = (GDCOLOR*)malloc(sizeof(GDCOLOR)*this->i_Colors);

	if (this->p_pPoint == NULL || this->c_pColor == NULL || this->l_pLines == NULL) {
		this->Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

__STATUS __WAY 
vectormap::Dispose() 
{
	free(this->p_pPoint);
	free(this->c_pColor);
	free(this->l_pLines);
	return GD_TASK_OKAY;
}
