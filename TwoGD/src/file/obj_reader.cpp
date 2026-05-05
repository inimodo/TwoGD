#include "..\twogd.h"

object::object()
{
	b_Loaded = FALSE;
	f_Stream = NULL;
	i_Faces = 0;
	i_Points = 0;
	v_Anchor = V3();
	c_pColor = COLOR();
}

object::object(LPSTR c_StreamName)
{
	f_Stream = NULL;
	f_Stream = NULL;
	i_Faces = 0;
	i_Points = 0;
	v_Anchor = V3();
	c_pColor = COLOR();
	if (Read(c_StreamName) != GD_TASK_OKAY)
	{
		Dispose();
		return;
	}
	b_Loaded = TRUE;
}


uint8_t object::Read(LPSTR c_StreamName)
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

uint8_t object::ReadHeader()
{
	CHAR c_Buffer[256];
	while (!feof(f_Stream))
	{
		fscanf(f_Stream, "%s", c_Buffer);
		if (c_Buffer[1] == '\0')
		{
			if (c_Buffer[0] == 'f')i_Faces++;
			if (c_Buffer[0] == 'v')i_Points++;
		}
	}
	rewind(f_Stream);
	return GD_TASK_OKAY;
}

uint8_t object::Prepare()
{
	v_pPoint = (V3*)malloc(sizeof(V3) * i_Points);
	o_pFace = (FACE*)malloc(sizeof(FACE) * i_Faces);

	if (v_pPoint == NULL || o_pFace == NULL) {
		Dispose();
		return GD_ALLOC_FAILED;
	}
	return GD_TASK_OKAY;
}

uint8_t object::LoadFile()
{
	CHAR c_Buffer[256];
	int i_VerticiesC = 0, i_FacesC = 0;
	while (!feof(f_Stream))
	{
		fscanf(f_Stream, "%s", c_Buffer);
		if (c_Buffer[1] == '\0')
		{
			if (i_VerticiesC == i_Points && i_FacesC == i_Faces)continue;
			if (c_Buffer[0] == 'f')
			{
				int i_Buffer = 0, i_A, i_B, i_C;
				fscanf(f_Stream, "%d/%d/%d %d/%d/%d %d/%d/%d", &i_A, &i_Buffer, &i_Buffer, &i_B, &i_Buffer, &i_Buffer, &i_C, &i_Buffer, &i_Buffer);
				o_pFace[i_FacesC].v_Point[0] = v_pPoint[i_A - 1];
				o_pFace[i_FacesC].v_Point[1] = v_pPoint[i_B - 1];
				o_pFace[i_FacesC].v_Point[2] = v_pPoint[i_C - 1];
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

void object::Dispose()
{
	b_Loaded = FALSE;
	if (v_pPoint != NULL) 
	{
		free(v_pPoint);
		v_pPoint = NULL;
	}
	if (o_pFace != NULL)
	{
		free(o_pFace);
		o_pFace = NULL;
	}
	fclose(f_Stream);
}
