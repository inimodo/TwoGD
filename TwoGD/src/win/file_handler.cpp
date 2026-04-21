#include "..\twogd.h"

#define GD_VEC_HEADER "v %d %d %d\n"
#define GD_VEC_POINTS "p %f %f\n"
#define GD_VEC_COLORS "c %d %d %d\n"
#define GD_VEC_LINE "l %d %d %d\n"

uint8_t 
filer::CloseStream() 
{
	if (f_Stream != NULL) 
	{
		return fclose(f_Stream);
	}
	return GD_TASK_OKAY;
}
uint8_t 
filer::OpenStream(LPSTR c_StreamName)
{
	if (f_Stream == NULL) 
	{
		f_Stream = fopen(c_StreamName, "r");

		if (f_Stream == NULL)
		{
			return GD_FILE_FAILED;
		}
		return GD_TASK_OKAY;
	}
	return GD_FILE_FAILED;
}

vectormap::vectormap()
{
	b_Loaded = FALSE;
	f_Stream = NULL;
}

uint8_t  
vectormap::Read(LPSTR c_StreamName)
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
	b_Loaded = TRUE;
	return GD_TASK_OKAY;
}
uint8_t 
vectormap::LoadFile()
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
uint8_t
vectormap::ReadHeader()
{
	if (fscanf(f_Stream, GD_VEC_HEADER, &i_Points, &i_Colors, &i_Connections) == NULL)
	{
		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}
uint8_t 
vectormap::Prepare()
{
	v_pPoint = (V2*)malloc(sizeof(V2)*i_Points);
	l_pLines = (LINE*)malloc(sizeof(LINE)*i_Connections);
	c_pColor = (COLOR*)malloc(sizeof(COLOR)*i_Colors);

	if (v_pPoint == NULL || c_pColor == NULL || l_pLines == NULL) {
		Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

uint8_t  
vectormap::Dispose() 
{
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
	return GD_TASK_OKAY;
}

object::object()
{
	f_Stream = NULL;
	i_Faces = 0;
	i_Points = 0;
	v_Anchor = V3();
}

object::object(LPSTR c_StreamName)
{
	f_Stream = NULL;
	i_Faces = 0;
	i_Points = 0;
	v_Anchor = V3();
	Read(c_StreamName);
}

uint8_t 
object::Read(LPSTR c_StreamName) 
{
	if (OpenStream(c_StreamName) != GD_TASK_OKAY) 
	{
		return GD_FILE_FAILED;
	}

	if (ReadHeader() != GD_TASK_OKAY) 
	{
		return GD_FILE_FAILED;
	}

	if (Prepare() != GD_TASK_OKAY) 
	{
		return GD_ALLOC_FAILED;
	}

	if (LoadFile() != GD_TASK_OKAY) {

		return GD_FILE_FAILED;
	}
	return GD_TASK_OKAY;
}


uint8_t
object::ReadHeader() 
{
	CHAR c_Buffer[256];
	while (!feof(f_Stream)) 
	{
		fscanf(f_Stream,"%s",c_Buffer);
		if (c_Buffer[1] == '\0') 
		{
			if (c_Buffer[0] == 'f')i_Faces++;
			if (c_Buffer[0] == 'v')i_Points++;
		}
	}
	rewind(f_Stream);
	return GD_TASK_OKAY;
}

uint8_t 
object::Prepare()
{
	v_pPoint = (V3*)malloc(sizeof(V3)*i_Points);
	o_pFace = (FACE*)malloc(sizeof(FACE)*i_Faces);

	if (v_pPoint == NULL || o_pFace == NULL ) {
		Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

uint8_t 
object::LoadFile()
{
	CHAR c_Buffer[256];
		int i_VerticiesC = 0, i_FacesC = 0;
	while (!feof(f_Stream))
	{
		fscanf(f_Stream, "%s", c_Buffer);
		if (c_Buffer[1] == '\0')
		{
			if (i_VerticiesC  == i_Points && i_FacesC  == i_Faces)continue;
			if (c_Buffer[0] == 'f') 
			{
				int i_Buffer = 0,i_A,i_B,i_C;
				fscanf(f_Stream, "%d/%d/%d %d/%d/%d %d/%d/%d",&i_A,&i_Buffer,&i_Buffer,&i_B,&i_Buffer,&i_Buffer,&i_C,&i_Buffer,&i_Buffer);	
				o_pFace[i_FacesC].v_Point[0] = v_pPoint[i_A-1];
				o_pFace[i_FacesC].v_Point[1] = v_pPoint[i_B-1];
				o_pFace[i_FacesC].v_Point[2] = v_pPoint[i_C-1];
				i_FacesC++;
			}
			if (c_Buffer[0] == 'v') 
			{
				fscanf(f_Stream, "%f %f %f", 
					&v_pPoint[i_VerticiesC].f_Pos[0],
					&v_pPoint[i_VerticiesC].f_Pos[1],
					&v_pPoint[i_VerticiesC].f_Pos[2]);
				
				i_VerticiesC++;
			}
		}
	}
	return GD_TASK_OKAY;
}

uint8_t 
object::Dispose() 
{
	free(v_pPoint);
	free(o_pFace);
	fclose(f_Stream);
	return GD_TASK_OKAY;

}

layer::layer()
{
	o_Obj = OBJ3D();
}

layer::layer(LPSTR c_StreamName, COLOR c_Color_)
{
	c_Color = c_Color_;
	o_Obj = OBJ3D(c_StreamName);
}
