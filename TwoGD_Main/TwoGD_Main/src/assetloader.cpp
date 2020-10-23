#include "twogd.h"

#define GD_VEC_HEADER "v %d %d %d\n"
#define GD_VEC_POINTS "p %f %f\n"
#define GD_VEC_COLORS "c %d %d %d\n"
#define GD_VEC_LINE "l %d %d %d\n"

__STATUS __WAY
filer::CloseStream() 
{
	if (this->f_Stream != NULL) 
	{
		return fclose(this->f_Stream);
	}
	return GD_TASK_OKAY;
}
__STATUS __WAY
filer::OpenStream(LPSTR c_StreamName)
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
vectormap::Read(LPSTR c_StreamName)
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
		if (fscanf(this->f_Stream, GD_VEC_COLORS, &(this->c_pColor[i_Index].c_Color[0]), &(this->c_pColor[i_Index].c_Color[1]), &(this->c_pColor[i_Index].c_Color[2])) == NULL)
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
	this->p_pPoint = (GDVEC2*)malloc(sizeof(GDVEC2)*this->i_Points);
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

	free(this->c_pColor);
	free(this->p_pPoint);
	free(this->l_pLines);
	return GD_TASK_OKAY;
}


__STATUS __WAY
object::Read(LPSTR c_StreamName) 
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
object::ReadHeader() 
{
	CHAR c_Buffer[256];
	while (!feof(this->f_Stream)) 
	{
		fscanf(this->f_Stream,"%s",c_Buffer);
		if (c_Buffer[1] == '\0') 
		{
			if (c_Buffer[0] == 'f')this->i_Faces++;
			if (c_Buffer[0] == 'v')this->i_Points++;
		}
	}
	rewind(this->f_Stream);
	return GD_TASK_OKAY;
}

__STATUS __WAY
object::Prepare()
{
	this->p_pPoint = (GDVEC3*)malloc(sizeof(GDVEC3)*this->i_Points);
	this->o_pFace = (GDFACE*)malloc(sizeof(GDFACE)*this->i_Faces);

	if (this->p_pPoint == NULL || this->o_pFace == NULL ) {
		this->Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

__STATUS __WAY
object::LoadFile()
{
	CHAR c_Buffer[256];
		int i_VerticiesC = 0, i_FacesC = 0;
	while (!feof(this->f_Stream))
	{
		fscanf(this->f_Stream, "%s", c_Buffer);
		if (c_Buffer[1] == '\0')
		{
			if (i_VerticiesC  == this->i_Points && i_FacesC  == this->i_Faces)continue;
			if (c_Buffer[0] == 'f') 
			{
				int i_Buffer = 0,i_A,i_B,i_C;
				fscanf(this->f_Stream, "%d/%d/%d %d/%d/%d %d/%d/%d",&i_A,&i_Buffer,&i_Buffer,&i_B,&i_Buffer,&i_Buffer,&i_C,&i_Buffer,&i_Buffer);	
				this->o_pFace[i_FacesC].p_Point[0] = this->p_pPoint[i_A-1];
				this->o_pFace[i_FacesC].p_Point[1] = this->p_pPoint[i_B-1];
				this->o_pFace[i_FacesC].p_Point[2] = this->p_pPoint[i_C-1];
				i_FacesC++;
			}
			if (c_Buffer[0] == 'v') 
			{
				fscanf(this->f_Stream, "%f %f %f", 
					&this->p_pPoint[i_VerticiesC].f_Pos[0],
					&this->p_pPoint[i_VerticiesC].f_Pos[1],
					&this->p_pPoint[i_VerticiesC].f_Pos[2]);
				
				i_VerticiesC++;
			}
		}
	}
	return GD_TASK_OKAY;
}

__STATUS __WAY
object::Dispose() 
{
	free(this->p_pPoint);
	free(this->o_pFace);
	return GD_TASK_OKAY;

}