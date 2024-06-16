#include "..\twogd.h"

#define GD_VEC_HEADER "v %d %d %d\n"
#define GD_VEC_POINTS "p %f %f\n"
#define GD_VEC_COLORS "c %c %c %c\n"
#define GD_VEC_LINE "l %d %d %d\n"

UCHAR 
filer::CloseStream() 
{
	if (this->f_Stream != NULL) 
	{
		return fclose(this->f_Stream);
	}
	return GD_TASK_OKAY;
}
UCHAR 
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
UCHAR  
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
UCHAR 
vectormap::LoadFile()
{
	for (UINT32 i_Index = 0; i_Index < this->i_Points; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_POINTS, &this->v_pPoint[i_Index].f_Pos[0], &this->v_pPoint[i_Index].f_Pos[1]) == NULL)
		{
			return GD_FILE_FAILED;
		}
	}
	for (UINT32 i_Index = 0; i_Index < this->i_Colors; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_COLORS, &(this->c_pColor[i_Index].c_Color[0]), &(this->c_pColor[i_Index].c_Color[1]), &(this->c_pColor[i_Index].c_Color[2])) == NULL)
		{
			return GD_FILE_FAILED;
		}
	}
	int i_Lone, i_Ltwo, i_Lcolor;
	for (UINT32 i_Index = 0; i_Index < this->i_Connections; i_Index++)
	{
		if (fscanf(this->f_Stream, GD_VEC_LINE, &i_Lone, &i_Ltwo, &i_Lcolor) == NULL)
		{
			return GD_FILE_FAILED;
		}
		this->l_pLines[i_Index].c_Color = this->c_pColor[i_Lcolor];
		this->l_pLines[i_Index].v_Point[0] = this->v_pPoint[i_Lone];
		this->l_pLines[i_Index].v_Point[1] = this->v_pPoint[i_Ltwo];
	}
	return GD_TASK_OKAY;
}
UCHAR 
vectormap::ReadHeader()
{
	if (fscanf(this->f_Stream, GD_VEC_HEADER, &this->i_Points, &this->i_Colors, &this->i_Connections) == NULL)
	{
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}
UCHAR 
vectormap::Prepare()
{
	this->v_pPoint = (V2*)malloc(sizeof(V2)*this->i_Points);
	this->l_pLines = (LINE*)malloc(sizeof(LINE)*this->i_Connections);
	this->c_pColor = (COLOR*)malloc(sizeof(COLOR)*this->i_Colors);

	if (this->v_pPoint == NULL || this->c_pColor == NULL || this->l_pLines == NULL) {
		this->Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

UCHAR  
vectormap::Dispose() 
{

	free(this->c_pColor);
	free(this->v_pPoint);
	free(this->l_pLines);
	return GD_TASK_OKAY;
}


UCHAR 
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

UCHAR 
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

UCHAR 
object::Prepare()
{
	this->v_pPoint = (V3*)malloc(sizeof(V3)*this->i_Points);
	this->o_pFace = (FACE*)malloc(sizeof(FACE)*this->i_Faces);

	if (this->v_pPoint == NULL || this->o_pFace == NULL ) {
		this->Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

UCHAR 
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
				this->o_pFace[i_FacesC].v_Point[0] = this->v_pPoint[i_A-1];
				this->o_pFace[i_FacesC].v_Point[1] = this->v_pPoint[i_B-1];
				this->o_pFace[i_FacesC].v_Point[2] = this->v_pPoint[i_C-1];
				i_FacesC++;
			}
			if (c_Buffer[0] == 'v') 
			{
				fscanf(this->f_Stream, "%f %f %f", 
					&this->v_pPoint[i_VerticiesC].f_Pos[0],
					&this->v_pPoint[i_VerticiesC].f_Pos[1],
					&this->v_pPoint[i_VerticiesC].f_Pos[2]);
				
				i_VerticiesC++;
			}
		}
	}
	return GD_TASK_OKAY;
}

UCHAR 
object::Dispose() 
{
	free(this->v_pPoint);
	free(this->o_pFace);
	return GD_TASK_OKAY;

}