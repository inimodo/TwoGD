#include "..\twogd.h"
#include <vector>

#define ISONCURVE(I) (this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_Flags[I] & TTF_FLAG_ONCURVEPOINT) 
#define V2FROMCOORD(P) V2((float)this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_XCoords[P] , (float)this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_YCoords[P]*-1.0f)

using namespace std;

V2 Berzier(V2 v_A, V2 v_B, V2 v_S1, float f_T)
{
	float f_NT = (1.0f - f_T);
	V2 v_R = V2();
	v_R.f_Pos[X] = v_A.f_Pos[X] * (f_NT * f_NT) + 2.0f * f_NT * f_T * v_S1.f_Pos[X] + f_T * f_T * v_B.f_Pos[X];
	v_R.f_Pos[Y] = v_A.f_Pos[Y] * (f_NT * f_NT) + 2.0f * f_NT * f_T * v_S1.f_Pos[Y] + f_T * f_T * v_B.f_Pos[Y];
	return v_R;
}

V2 Midpoint(V2 v_A, V2 v_B)
{
	V2 v_R = V2();
	v_R.f_Pos[X] = (v_A.f_Pos[X] + v_B.f_Pos[X]) / 2.0f;
	v_R.f_Pos[Y] = (v_A.f_Pos[Y] + v_B.f_Pos[Y]) / 2.0f;
	return v_R;
}

font_handler::font_handler()
{
}

font_handler::font_handler(CODEC2D* o_pCodec, LPSTR c_pFontFolder, int i_DivPerCurve)
{
	this->i_Padding = 5;
	this->i_SpaceWidth = 10;
	this->c_Color = COLOR(255, 255, 255);
	this->o_pCodec = o_pCodec;
	i_LastError = this->Load(c_pFontFolder);

	for (int i_Index = 0; i_Index < ASCII_CHARS; i_Index++)
	{
		this->v_pFont[i_Index] = CHARMAP();

		if (i_Index < ASCII_CHAR_START || i_Index > ASCII_CHAR_STOP)
		{
			continue;
		}

		if (this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_GlyphSize == 0 ||
			this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_NumberOfContours == 0)
		{
			continue;
		}

		this->v_pFont[i_Index].b_Loaded = TRUE;
		this->v_pFont[i_Index].i_XMax = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_XMax;
		this->v_pFont[i_Index].i_XMin = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_XMin;
		this->v_pFont[i_Index].i_YMax = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_YMin * -1;
		this->v_pFont[i_Index].i_YMin = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_YMax * -1;
		this->v_pFont[i_Index].i_Height = this->v_pFont[i_Index].i_YMax - this->v_pFont[i_Index].i_YMin;
		this->v_pFont[i_Index].i_Width = this->v_pFont[i_Index].i_XMax - this->v_pFont[i_Index].i_XMin;

		vector<LINE> o_Lines;
		int16_t i_Start = 0;
		for (int i_Cont = 0; i_Cont < this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_NumberOfContours; i_Cont++)
		{
			vector<V2> v_Dots;
			V2 v_Current = V2FROMCOORD(i_Start);
			uint16_t i_Size = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_EndPtsOfContours[i_Cont] + 1;
			for (int i_Point = i_Start; i_Point < i_Size; i_Point++)
			{
				uint16_t i_Next = i_Point + 1;
				if (i_Point + 1 > i_Size - 1)
				{
					i_Next = i_Start;
				}

				V2 v_P1 = V2FROMCOORD(i_Point);
				V2 v_P2 = V2FROMCOORD(i_Next);

				if (ISONCURVE(i_Point))
				{
					v_Dots.push_back(v_P1);
					v_Current = v_P1;
				}
				else
				{
					if (ISONCURVE(i_Point + 1))
					{
						for (float f_T = 0; f_T <= 1.0f; f_T += (1.0f / (float)i_DivPerCurve))
						{
							V2 v_P = Berzier(v_Current, v_P2, v_P1, f_T);
							v_Dots.push_back(v_P);
						}
						v_Current = v_P2;
						i_Point++;
					}
					else
					{
						V2 v_Mid = Midpoint(v_P1, v_P2);
						for (float f_T = 0; f_T <= 1.0f; f_T += (1.0f / (float)i_DivPerCurve))
						{
							V2 v_P = Berzier(v_Current, v_Mid, v_P1, f_T);
							v_Dots.push_back(v_P);
						}
						v_Current = v_Mid;
					}
				}
			}

			V2 v_Start, v_Latest;
			for (int i_Dot = 0; i_Dot < v_Dots.size(); i_Dot++)
			{
				if (i_Dot == 0)
				{
					v_Latest = v_Dots[i_Dot];
					v_Start = v_Dots[i_Dot];
					continue;
				}

				LINE o_Line = LINE();
				o_Line.c_Color = COLOR(0, 0, 0);
				o_Line.v_Point[0] = v_Latest;
				o_Line.v_Point[1] = v_Dots[i_Dot];
				o_Lines.push_back(o_Line);
				v_Latest = v_Dots[i_Dot];
			}
			LINE o_Line = LINE();
			o_Line.c_Color = COLOR(0, 0, 0);
			o_Line.v_Point[0] = v_Latest;
			o_Line.v_Point[1] = v_Start;
			o_Lines.push_back(o_Line);

			i_Start = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_EndPtsOfContours[i_Cont] + 1;
		}

		this->v_pFont[i_Index].l_pLines = (LINE*)malloc(sizeof(LINE) * o_Lines.size());
		memcpy(this->v_pFont[i_Index].l_pLines, o_Lines.data(), o_Lines.size() * sizeof(LINE));
		this->v_pFont[i_Index].i_Count = (uint32_t)o_Lines.size();
		this->v_pFont[i_Index].b_Loaded = TRUE;

	}
}

void font_handler::Write(V2 v_pAnchor, float f_Scale, const char* c_pformat, ...)
{
	va_list o_args;
	va_start(o_args, c_pformat);
	CHAR c_new[MAXIMAL_TEXT_LENGTH];
	vsprintf(c_new, c_pformat, o_args);
	V2 v_Cursor = V2(v_pAnchor.f_Pos);
	float f_PixelScale = f_Scale / (float)ttf_HEAD.i_UnitsPerEm;
	int i_Index = 0;
	while (c_new[i_Index] != '\0')
	{
		if ((int)c_new[i_Index] < 0 && (int)c_new[i_Index] >= ASCII_CHARS)
		{
			i_Index++;
			continue;
		}
		if (this->v_pFont[(int)c_new[i_Index]].b_Loaded == FALSE)
		{
			v_Cursor.f_Pos[X] += this->i_Padding + this->i_SpaceWidth * f_PixelScale;
			i_Index++;
			continue;
		}
		this->o_pCodec->DrawChar(&this->v_pFont[(int)c_new[i_Index]], &v_Cursor, &this->c_Color, f_PixelScale);
		v_Cursor.f_Pos[X] += this->i_Padding + this->v_pFont[(int)c_new[i_Index]].i_Width * f_PixelScale;
		i_Index++;
	}
}

void font_handler::Free()
{
	if (ttf_Tables != NULL)
	{
		free(ttf_Tables);
	}
	if (ttf_pGLYF != NULL)
	{
		for (int i_Index = 0; i_Index < ttf_MAXP.i_NumGlyph; i_Index++)
		{
			if (ttf_pGLYF[i_Index].i_GlyphSize < 10)
			{
				continue;
			}
			if (ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours == 0)
			{
				continue;
			}
			free(ttf_pGLYF[i_Index].i_EndPtsOfContours);
			free(ttf_pGLYF[i_Index].i_Instructions);
			free(ttf_pGLYF[i_Index].i_Flags);
			free(ttf_pGLYF[i_Index].i_XCoords);
			free(ttf_pGLYF[i_Index].i_YCoords);

		}
	}
	free(ttf_pGLYF);
	fclose(f_File);
}

uint8_t font_handler::Load(LPSTR s_Path)
{
	f_File = fopen(s_Path, "rb");
	if (f_File == NULL)
	{
		Free();
		return GD_FILE_FAILED;
	}

	fread(&ttf_Header, sizeof(TTFHEADER), 1, f_File);
	ttf_Header.i_ScalerType = _byteswap_ulong(ttf_Header.i_ScalerType);
	ttf_Header.i_NumTables = _byteswap_ushort(ttf_Header.i_NumTables);
	ttf_Header.i_SearchRange = _byteswap_ushort(ttf_Header.i_SearchRange);
	ttf_Header.i_EntrySelector = _byteswap_ushort(ttf_Header.i_EntrySelector);
	ttf_Header.i_RangeShift = _byteswap_ushort(ttf_Header.i_RangeShift);

	ttf_Tables = (TTFTABLEDIR*)malloc(sizeof(TTFTABLEDIR) * ttf_Header.i_NumTables);
	if (ttf_Tables == NULL)
	{
		Free();
		return GD_ALLOC_FAILED;
	}

	fread(ttf_Tables, sizeof(TTFTABLEDIR), ttf_Header.i_NumTables, f_File);

	for (int i_Index = 0; i_Index < ttf_Header.i_NumTables; i_Index++)
	{

		ttf_Tables[i_Index].i_Checksum = _byteswap_ulong(ttf_Tables[i_Index].i_Checksum);
		ttf_Tables[i_Index].i_Offset = _byteswap_ulong(ttf_Tables[i_Index].i_Offset);
		ttf_Tables[i_Index].i_Length = _byteswap_ulong(ttf_Tables[i_Index].i_Length);

		if (ttf_Tables[i_Index].i_Tag[0] == 'h' && ttf_Tables[i_Index].i_Tag[1] == 'e' && ttf_Tables[i_Index].i_Tag[2] == 'a' && ttf_Tables[i_Index].i_Tag[3] == 'd')
		{
			fseek(f_File, ttf_Tables[i_Index].i_Offset, SEEK_SET);
			fread(&ttf_HEAD, sizeof(TTFHEAD), 1, f_File);
		}
		if (ttf_Tables[i_Index].i_Tag[0] == 'm' && ttf_Tables[i_Index].i_Tag[1] == 'a' && ttf_Tables[i_Index].i_Tag[2] == 'x' && ttf_Tables[i_Index].i_Tag[3] == 'p')
		{
			fseek(f_File, ttf_Tables[i_Index].i_Offset, SEEK_SET);
			fread(&ttf_MAXP, sizeof(TTFMAXP), 1, f_File);
		}
		if (ttf_Tables[i_Index].i_Tag[0] == 'g' && ttf_Tables[i_Index].i_Tag[1] == 'l' && ttf_Tables[i_Index].i_Tag[2] == 'y' && ttf_Tables[i_Index].i_Tag[3] == 'f')
		{
			i_GlyphOffset = ttf_Tables[i_Index].i_Offset;
		}
		if (ttf_Tables[i_Index].i_Tag[0] == 'l' && ttf_Tables[i_Index].i_Tag[1] == 'o' && ttf_Tables[i_Index].i_Tag[2] == 'c' && ttf_Tables[i_Index].i_Tag[3] == 'a')
		{
			i_LocaOffset = ttf_Tables[i_Index].i_Offset;
		}
		if (ttf_Tables[i_Index].i_Tag[0] == 'c' && ttf_Tables[i_Index].i_Tag[1] == 'm' && ttf_Tables[i_Index].i_Tag[2] == 'a' && ttf_Tables[i_Index].i_Tag[3] == 'p')
		{
			i_CmapOffset = ttf_Tables[i_Index].i_Offset;
		}
	}

	ttf_HEAD.i_IndexToLocFormat = _byteswap_ushort(ttf_HEAD.i_IndexToLocFormat);
	ttf_HEAD.i_MagicNumber = _byteswap_ulong(ttf_HEAD.i_MagicNumber);
	ttf_HEAD.i_UnitsPerEm = _byteswap_ushort(ttf_HEAD.i_UnitsPerEm);

	if (ttf_HEAD.i_MagicNumber != 0x5F0F3CF5)
	{
		Free();
		return GD_TTF_ERROR;
	}

	ttf_MAXP.i_Version = _byteswap_ulong(ttf_MAXP.i_Version);
	ttf_MAXP.i_NumGlyph = _byteswap_ushort(ttf_MAXP.i_NumGlyph);

	if (ttf_MAXP.i_Version != 65536)
	{
		Free();
		return GD_TTF_ERROR;
	}


	ttf_pGLYF = (TTFGLYF*)malloc((ttf_MAXP.i_NumGlyph) * sizeof(TTFGLYF));

	fseek(f_File, i_LocaOffset, SEEK_SET);
	if (ttf_HEAD.i_IndexToLocFormat == 1)
	{
		uint32_t* loca = (uint32_t*)malloc((ttf_MAXP.i_NumGlyph + 1) * sizeof(uint32_t));
		if (loca == NULL)
		{
			Free();
			return GD_ALLOC_FAILED;
		}
		fread(loca, sizeof(uint32_t), ttf_MAXP.i_NumGlyph + 1, f_File);
		for (int i_Index = 0; i_Index < ttf_MAXP.i_NumGlyph; i_Index++)
		{
			ttf_pGLYF[i_Index].i_GlyphStart = i_GlyphOffset + _byteswap_ulong(loca[i_Index]);
			ttf_pGLYF[i_Index].i_GlyphStop = i_GlyphOffset + _byteswap_ulong(loca[i_Index + 1]);
			ttf_pGLYF[i_Index].i_GlyphSize = ttf_pGLYF[i_Index].i_GlyphStop - ttf_pGLYF[i_Index].i_GlyphStart;
		}
		free(loca);
	}
	else
	{
		uint16_t* loca = (uint16_t*)malloc((ttf_MAXP.i_NumGlyph + 1) * sizeof(uint16_t));
		if (loca == NULL)
		{
			Free();
			return GD_ALLOC_FAILED;
		}
		fread(loca, sizeof(uint16_t), ttf_MAXP.i_NumGlyph + 1, f_File);
		for (int i_Index = 0; i_Index < ttf_MAXP.i_NumGlyph; i_Index++)
		{
			ttf_pGLYF[i_Index].i_GlyphStart = i_GlyphOffset + _byteswap_ushort(loca[i_Index]) * 2;
			ttf_pGLYF[i_Index].i_GlyphStop = i_GlyphOffset + _byteswap_ushort(loca[i_Index + 1]) * 2;
			ttf_pGLYF[i_Index].i_GlyphSize = ttf_pGLYF[i_Index].i_GlyphStop - ttf_pGLYF[i_Index].i_GlyphStart;
		}
		free(loca);
	}

	for (int i_Index = 0; i_Index < ttf_MAXP.i_NumGlyph; i_Index++)
	{
		if (ttf_pGLYF[i_Index].i_GlyphSize < 10) continue;
		fseek(f_File, ttf_pGLYF[i_Index].i_GlyphStart, SEEK_SET);
		fread(&(ttf_pGLYF[i_Index].ttf_HEAD), sizeof(TTFGLYFH), 1, f_File);

		if (ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours & 0x1)
		{
			ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours = 0;
			continue;
		}

		ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours = _byteswap_ushort(ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours);
		ttf_pGLYF[i_Index].ttf_HEAD.i_XMax = _byteswap_ushort(ttf_pGLYF[i_Index].ttf_HEAD.i_XMax);
		ttf_pGLYF[i_Index].ttf_HEAD.i_XMin = _byteswap_ushort(ttf_pGLYF[i_Index].ttf_HEAD.i_XMin);
		ttf_pGLYF[i_Index].ttf_HEAD.i_YMax = _byteswap_ushort(ttf_pGLYF[i_Index].ttf_HEAD.i_YMax);
		ttf_pGLYF[i_Index].ttf_HEAD.i_YMin = _byteswap_ushort(ttf_pGLYF[i_Index].ttf_HEAD.i_YMin);

		ttf_pGLYF[i_Index].i_EndPtsOfContours = (uint16_t*)malloc(sizeof(uint16_t) * ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours);
		if (ttf_pGLYF[i_Index].i_EndPtsOfContours == NULL)
		{
			Free();
			return GD_ALLOC_FAILED;
		}
		fread(ttf_pGLYF[i_Index].i_EndPtsOfContours, sizeof(uint16_t), ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours, f_File);

		for (int cont = 0; cont < ttf_pGLYF[i_Index].ttf_HEAD.i_NumberOfContours; cont++)
		{
			ttf_pGLYF[i_Index].i_EndPtsOfContours[cont] = _byteswap_ushort(ttf_pGLYF[i_Index].i_EndPtsOfContours[cont]);
			ttf_pGLYF[i_Index].i_NumPoints = ttf_pGLYF[i_Index].i_EndPtsOfContours[cont] + 1;
		}

		fread(&(ttf_pGLYF[i_Index].i_InstructionLength), sizeof(uint16_t), 1, f_File);
		ttf_pGLYF[i_Index].i_InstructionLength = _byteswap_ushort(ttf_pGLYF[i_Index].i_InstructionLength);
		ttf_pGLYF[i_Index].i_Instructions = (uint8_t*)malloc(sizeof(uint8_t) * ttf_pGLYF[i_Index].i_InstructionLength);
		if (ttf_pGLYF[i_Index].i_Instructions == NULL)
		{
			Free();
			return GD_ALLOC_FAILED;
		}
		fread(ttf_pGLYF[i_Index].i_Instructions, sizeof(uint8_t), ttf_pGLYF[i_Index].i_InstructionLength, f_File);

		ttf_pGLYF[i_Index].i_Flags = (uint8_t*)malloc(sizeof(uint8_t) * ttf_pGLYF[i_Index].i_NumPoints);
		for (int flagIndex = 0; flagIndex < ttf_pGLYF[i_Index].i_NumPoints; flagIndex++)
		{
			uint8_t flag;
			fread(&flag, sizeof(uint8_t), 1, f_File);
			ttf_pGLYF[i_Index].i_Flags[flagIndex] = flag & ~TTF_FLAG_REPEAT;

			if (flag & TTF_FLAG_REPEAT)
			{
				uint8_t repeating;
				fread(&repeating, sizeof(uint8_t), 1, f_File);
				flagIndex++;
				for (int repeat = 0; repeat < repeating; repeat++)
				{
					ttf_pGLYF[i_Index].i_Flags[flagIndex] = flag & ~TTF_FLAG_REPEAT;
					flagIndex++;
				}
				flagIndex--;
			}
		}

		ttf_pGLYF[i_Index].i_XCoords = (int32_t*)malloc(sizeof(int32_t) * ttf_pGLYF[i_Index].i_NumPoints);
		ttf_pGLYF[i_Index].i_YCoords = (int32_t*)malloc(sizeof(int32_t) * ttf_pGLYF[i_Index].i_NumPoints);
		if (ttf_pGLYF[i_Index].i_XCoords == NULL || ttf_pGLYF[i_Index].i_YCoords == NULL)
		{
			Free();
			return GD_ALLOC_FAILED;
		}
		int i_Last = 0;
		for (int flagIndex = 0; flagIndex < ttf_pGLYF[i_Index].i_NumPoints; flagIndex++)
		{
			int i_Delta;
			if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_XSHORTVEC)
			{
				uint8_t i_Read;
				fread(&i_Read, sizeof(uint8_t), 1, f_File);
				if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_XSAME)
				{
					i_Delta = i_Read;
				}
				else
				{
					i_Delta = -i_Read;
				}
			}
			else
			{
				if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_XSAME)
				{
					i_Delta = 0;
				}
				else
				{
					uint16_t i_Read;
					fread(&i_Read, sizeof(uint16_t), 1, f_File);
					i_Read = _byteswap_ushort(i_Read);
					i_Delta = (int16_t)i_Read;
				}
			}
			ttf_pGLYF[i_Index].i_XCoords[flagIndex] = i_Last + i_Delta;
			i_Last = ttf_pGLYF[i_Index].i_XCoords[flagIndex];

		}
		i_Last = 0;
		for (int flagIndex = 0; flagIndex < ttf_pGLYF[i_Index].i_NumPoints; flagIndex++)
		{
			int i_Delta;
			if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_YSHORTVEC)
			{
				uint8_t i_Read;
				fread(&i_Read, sizeof(uint8_t), 1, f_File);
				if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_YSAME)
				{
					i_Delta = i_Read;
				}
				else
				{
					i_Delta = -i_Read;
				}
			}
			else
			{
				if (ttf_pGLYF[i_Index].i_Flags[flagIndex] & TTF_FLAG_YSAME)
				{
					i_Delta = 0;
				}
				else
				{
					uint16_t i_Read;
					fread(&i_Read, sizeof(uint16_t), 1, f_File);
					i_Read = _byteswap_ushort(i_Read);
					i_Delta = (int16_t)i_Read;
				}
			}
			ttf_pGLYF[i_Index].i_YCoords[flagIndex] = i_Last + i_Delta;
			i_Last = ttf_pGLYF[i_Index].i_YCoords[flagIndex];
		}
	}

	fseek(f_File, i_CmapOffset, SEEK_SET);
	fread(&(ttf_CMAP.i_Version), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_NumTables), sizeof(uint16_t), 1, f_File);
	ttf_CMAP.i_NumTables = _byteswap_ushort(ttf_CMAP.i_NumTables);
	ttf_CMAP.i_Version = _byteswap_ushort(ttf_CMAP.i_Version);

	ttf_CMAP.ttf_Record = (TTFENCODINGRECORD*)malloc(sizeof(TTFENCODINGRECORD) * ttf_CMAP.i_NumTables);
	if (ttf_CMAP.ttf_Record == NULL)
	{
		Free();
		return GD_ALLOC_FAILED;
	}
	fread(ttf_CMAP.ttf_Record, sizeof(TTFENCODINGRECORD), ttf_CMAP.i_NumTables, f_File);

	ttf_CMAP.i_UnicodeSubtableOffset = 0;
	for (int i_Index = 0; i_Index < ttf_CMAP.i_NumTables; i_Index++)
	{
		ttf_CMAP.ttf_Record[i_Index].i_EncodingID = _byteswap_ushort(ttf_CMAP.ttf_Record[i_Index].i_EncodingID);
		ttf_CMAP.ttf_Record[i_Index].i_PlatformID = _byteswap_ushort(ttf_CMAP.ttf_Record[i_Index].i_PlatformID);
		ttf_CMAP.ttf_Record[i_Index].i_SubtableOffset = _byteswap_ulong(ttf_CMAP.ttf_Record[i_Index].i_SubtableOffset);

		if (ttf_CMAP.ttf_Record[i_Index].i_EncodingID == TTF_UNICODE_ENCODINGID && ttf_CMAP.ttf_Record[i_Index].i_PlatformID == TTF_UNICODE_PLATFORMID)
		{
			ttf_CMAP.i_UnicodeSubtableOffset = ttf_CMAP.ttf_Record[i_Index].i_SubtableOffset;
		}
	}

	if (ttf_CMAP.i_UnicodeSubtableOffset == 0)
	{
		Free();
		return GD_TTF_ERROR;
	}

	fseek(f_File, i_CmapOffset + ttf_CMAP.i_UnicodeSubtableOffset, SEEK_SET);

	fread(&(ttf_CMAP.i_Format), sizeof(uint16_t), 1, f_File);
	ttf_CMAP.i_Format = _byteswap_ushort(ttf_CMAP.i_Format);

	if (ttf_CMAP.i_Format != TTF_FORMAT)
	{
		Free();
		return GD_TTF_ERROR;
	}

	fread(&(ttf_CMAP.i_Length), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_Language), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_SegCount), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_SearchRange), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_EntrySelector), sizeof(uint16_t), 1, f_File);
	fread(&(ttf_CMAP.i_RangeShift), sizeof(uint16_t), 1, f_File);

	ttf_CMAP.i_Length = _byteswap_ushort(ttf_CMAP.i_Length);
	ttf_CMAP.i_Language = _byteswap_ushort(ttf_CMAP.i_Language);
	ttf_CMAP.i_SegCount = _byteswap_ushort(ttf_CMAP.i_SegCount) / 2;
	ttf_CMAP.i_SearchRange = _byteswap_ushort(ttf_CMAP.i_SearchRange);
	ttf_CMAP.i_EntrySelector = _byteswap_ushort(ttf_CMAP.i_EntrySelector);
	ttf_CMAP.i_RangeShift = _byteswap_ushort(ttf_CMAP.i_RangeShift);

	ttf_CMAP.i_EndCode = (uint16_t*)malloc(sizeof(uint16_t) * ttf_CMAP.i_SegCount);
	ttf_CMAP.i_StartCode = (uint16_t*)malloc(sizeof(uint16_t) * ttf_CMAP.i_SegCount);
	ttf_CMAP.i_IdDelta = (int16_t*)malloc(sizeof(int16_t) * ttf_CMAP.i_SegCount);
	ttf_CMAP.i_IdRangeOffset = (uint16_t*)malloc(sizeof(uint16_t) * ttf_CMAP.i_SegCount);

	if (ttf_CMAP.i_EndCode == NULL || ttf_CMAP.i_StartCode == NULL || ttf_CMAP.i_IdDelta == NULL || ttf_CMAP.i_IdRangeOffset == NULL)
	{
		Free();
		return GD_ALLOC_FAILED;
	}
	fread(ttf_CMAP.i_EndCode, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);
	fread(&(ttf_CMAP.i_ReservedPad), sizeof(uint16_t), 1, f_File);
	fread(ttf_CMAP.i_StartCode, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);
	fread(ttf_CMAP.i_IdDelta, sizeof(int16_t), ttf_CMAP.i_SegCount, f_File);
	fread(ttf_CMAP.i_IdRangeOffset, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);

	for (int i_Index = 0; i_Index < ttf_CMAP.i_SegCount; i_Index++)
	{
		ttf_CMAP.i_EndCode[i_Index] = _byteswap_ushort(ttf_CMAP.i_EndCode[i_Index]);
		ttf_CMAP.i_StartCode[i_Index] = _byteswap_ushort(ttf_CMAP.i_StartCode[i_Index]);
		ttf_CMAP.i_IdDelta[i_Index] = (int16_t)_byteswap_ushort(ttf_CMAP.i_IdDelta[i_Index]);
		ttf_CMAP.i_IdRangeOffset[i_Index] = _byteswap_ushort(ttf_CMAP.i_IdRangeOffset[i_Index]);
	}

	uint32_t i_GlyphIdArraySize = (ttf_CMAP.i_Length - (16 + 8 * ttf_CMAP.i_SegCount)) / 2;
	ttf_CMAP.i_GlyphIdArray = (uint16_t*)malloc(sizeof(uint16_t) * i_GlyphIdArraySize);
	if (ttf_CMAP.i_GlyphIdArray == NULL)
	{
		Free();
		return GD_ALLOC_FAILED;
	}

	fread(ttf_CMAP.i_GlyphIdArray, sizeof(uint16_t), i_GlyphIdArraySize, f_File);
	for (uint32_t i_Index = 0; i_Index < i_GlyphIdArraySize; i_Index++)
	{
		ttf_CMAP.i_GlyphIdArray[i_Index] = _byteswap_ushort(ttf_CMAP.i_GlyphIdArray[i_Index]);
	}
	for (uint16_t c_ASCII = 0; c_ASCII < ASCII_CHARS; c_ASCII++)
	{

		if (c_ASCII < ASCII_CHAR_START || c_ASCII > ASCII_CHAR_STOP)
		{
			c_ASCIIMapping[c_ASCII] = 0;
			continue;
		}
		uint32_t i_Segment = 0;
		for (i_Segment = 0; i_Segment < ttf_CMAP.i_SegCount; i_Segment++)
		{
			if (c_ASCII >= ttf_CMAP.i_StartCode[i_Segment] && c_ASCII <= ttf_CMAP.i_EndCode[i_Segment])
			{
				break;
			}
		}

		if (ttf_CMAP.i_IdRangeOffset[i_Segment] == 0)
		{
			c_ASCIIMapping[c_ASCII] = (c_ASCII + ttf_CMAP.i_IdDelta[i_Segment]) & 0xFFFF;
		}
		else
		{
			int32_t i_Index = (ttf_CMAP.i_IdRangeOffset[i_Segment] / 2) + (c_ASCII - ttf_CMAP.i_StartCode[i_Segment]) - (ttf_CMAP.i_SegCount - i_Segment);
			c_ASCIIMapping[c_ASCII] = ttf_CMAP.i_GlyphIdArray[i_Index];
			if (c_ASCIIMapping[c_ASCII] != 0)
			{
				c_ASCIIMapping[c_ASCII] = (c_ASCIIMapping[c_ASCII] + ttf_CMAP.i_IdDelta[i_Segment]) & 0xFFFF;
			}
		}

	}
	return 0;
}


void font_handler::Dispose()
{
	for (int i_Index = 0; i_Index < ASCII_CHARS; i_Index++)
	{
		if (v_pFont[i_Index].b_Loaded == FALSE)
		{
			continue;
		}
		v_pFont[i_Index].b_Loaded = FALSE;
		if (v_pFont[i_Index].l_pLines != NULL) 
		{
			free(v_pFont[i_Index].l_pLines);
			v_pFont[i_Index].l_pLines = NULL;
		}
	}
	Free();
}

