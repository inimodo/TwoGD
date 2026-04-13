#include "..\twogd.h"

font_handler::font_handler(CODEC2D* o_pCodec, LPSTR c_pFontFolder) 
{
	this->i_Padding = 2;
	this->i_SpaceWidth = 10;
	this->c_Color = COLOR(255,255,255);
	for (int i_Index = 0; i_Index < ASCII_CHAR_COUNT; i_Index++)
	{
		this->v_pFont[i_Index] = VMAP();
	}
	WIN32_FIND_DATAA o_FileData;
	HANDLE f_Handle = FindFirstFileA(c_pFontFolder, &o_FileData);
	if (f_Handle == INVALID_HANDLE_VALUE) return;
	do
	{
		if (o_FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) continue;
		if (strstr(o_FileData.cFileName, FILEEND_VMF) == NULL) continue;
		
		CHAR c_File[MAX_PATH];
		strcpy(c_File, c_pFontFolder);
		c_File[strlen(c_File)-1] = '\0';
		strcat(c_File, o_FileData.cFileName);
		
		int i_ASCIIIndex = -1;

		if (o_FileData.cFileName[0] >= 'A' && o_FileData.cFileName[0] <= 'Z') 
		{
			i_ASCIIIndex = o_FileData.cFileName[0];
		}
		if (o_FileData.cFileName[0]== '_' && o_FileData.cFileName[1] >= 'a' && o_FileData.cFileName[1] <= 'z')
		{
			i_ASCIIIndex = o_FileData.cFileName[1];
		}
		if (o_FileData.cFileName[0] >= '0' && o_FileData.cFileName[0] <= '9')
		{
			i_ASCIIIndex = o_FileData.cFileName[0];
		}
		if (o_FileData.cFileName[0] == '#')
		{
			i_ASCIIIndex = '!' + o_FileData.cFileName[1] - 'a';
		}
		if (o_FileData.cFileName[0] == '$')
		{
			i_ASCIIIndex = ':' + o_FileData.cFileName[1] - 'a';
		}
		if (o_FileData.cFileName[0] == '%')
		{
			i_ASCIIIndex = '[' + o_FileData.cFileName[1] - 'a';
		}
		if (o_FileData.cFileName[0] == '&')
		{
			i_ASCIIIndex = '{' + o_FileData.cFileName[1] - 'a';
		}

		if (i_ASCIIIndex >= 0) 
		{
			v_pFont[i_ASCIIIndex].Read(c_File);
		}

	} while (FindNextFileA(f_Handle, &o_FileData) != 0);
	FindClose(f_Handle);
	this->o_pCodec = o_pCodec;
}

void font_handler::Write(V2 v_pAnchor, float f_Scale, const char* c_pformat, ...) 
{
	va_list o_args;
	va_start(o_args, c_pformat);
	CHAR c_new[MAXIMAL_TEXT_LENGTH];
	vsprintf(c_new, c_pformat, o_args);
	V2 v_Cursor = V2(v_pAnchor.f_Pos);
	int i_Index = 0;
	while (c_new[i_Index] != '\0')
	{
		if ((int)c_new[i_Index] < 0 && (int)c_new[i_Index] >= ASCII_CHAR_COUNT) 
		{
			i_Index++;
			continue;
		}
		if (this->v_pFont[(int)c_new[i_Index]].b_Loaded == FALSE) 
		{
			v_Cursor.f_Pos[X] += this->i_Padding + this->i_SpaceWidth * f_Scale;
			i_Index++;
			continue;
		}
		this->o_pCodec->DrawChar(&this->v_pFont[(int)c_new[i_Index]], &v_Cursor, &this->c_Color, f_Scale);
		v_Cursor.f_Pos[X] += this->i_Padding + this->v_pFont[(int)c_new[i_Index]].i_Width * f_Scale;
		i_Index++;
	}
}


UCHAR font_handler::Dispose() 
{
	for (int i_Index = 0; i_Index < ASCII_CHAR_COUNT; i_Index++)
	{
		if (this->v_pFont[i_Index].b_Loaded == FALSE)
		{
			continue;
		}
		this->v_pFont[i_Index].Dispose();
	}
	return GD_TASK_OKAY;
}