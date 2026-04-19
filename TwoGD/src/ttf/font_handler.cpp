#include "..\twogd.h"
#include <vector>

using namespace std;

V2 Berzier(V2 v_A,V2 v_B,V2 v_S1,float f_T) 
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
    v_R.f_Pos[X] = (v_A.f_Pos[X] + v_B.f_Pos[X])/2.0f;
    v_R.f_Pos[Y] = (v_A.f_Pos[Y] + v_B.f_Pos[Y])/2.0f;
    return v_R;
}

//V2 Berzier2(V2 v_A, V2 v_B, V2 v_S1, V2 v_S2, float f_T)
//{
//    float f_NT = (1.0f - f_T);
//    V2 v_R = V2();
//    v_R.f_Pos[X] = v_A.f_Pos[X] * (f_NT * f_NT * f_NT) + 3.0f * f_NT * f_NT * f_T * v_S1.f_Pos[X] + 3.0f * f_NT * f_T * f_T * v_S2.f_Pos[X] + f_T * f_T * f_T * v_B.f_Pos[X];
//    v_R.f_Pos[Y] = v_A.f_Pos[Y] * (f_NT * f_NT * f_NT) + 3.0f * f_NT * f_NT * f_T * v_S1.f_Pos[Y] + 3.0f * f_NT * f_T * f_T * v_S2.f_Pos[Y] + f_T * f_T * f_T * v_B.f_Pos[Y];
//    return v_R;
//}
#define ISONCURVE(I) (this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_Flags[I] & TTF_FLAG_ONCURVEPOINT) 
#define ALLOWEDSTEP(I) ( I < this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_NumPoints )
#define V2FROMCOORD(P) V2(this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_XCoords[P],this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_YCoords[P]*-1)

font_handler::font_handler(CODEC2D* o_pCodec, LPSTR c_pFontFolder, int i_DivPerCurve)
{
	this->i_Padding = 2;
	this->i_SpaceWidth = 10;
	this->c_Color = COLOR(255,255,255);
    this->o_pCodec = o_pCodec;

	this->Load(c_pFontFolder);

	for (int i_Index = 0; i_Index < ASCII_CHARS; i_Index++)
	{
        this->v_pFont[i_Index] = CHARMAP();
		if (i_Index < ASCII_CHAR_START || i_Index > ASCII_CHAR_STOP)
		{
			continue;
		}
		printf("\n%c\n",(char)i_Index);
		if (this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_GlyphSize == 0 || 
            this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_NumberOfContours == 0) 
		{
			continue;
		}

        this->v_pFont[i_Index].b_Loaded = TRUE;
        this->v_pFont[i_Index].i_XMax = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_XMax;
        this->v_pFont[i_Index].i_XMin = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_XMin;
        this->v_pFont[i_Index].i_YMax = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_YMin*-1;
        this->v_pFont[i_Index].i_YMin = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_YMax*-1;
        this->v_pFont[i_Index].i_Height = this->v_pFont[i_Index].i_YMax - this->v_pFont[i_Index].i_YMin;
        this->v_pFont[i_Index].i_Width = this->v_pFont[i_Index].i_XMax - this->v_pFont[i_Index].i_XMin;

        vector<LINE> o_Lines;
        int16_t i_Start = 0;
        for (int i_Cont = 0; i_Cont < this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].ttf_HEAD.i_NumberOfContours; i_Cont++)
        {
            vector<V2> v_Dots;

            

            for (int i_Point = i_Start; i_Point < this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_EndPtsOfContours[i_Cont]+1; i_Point++)
            {
                printf("\t[%d] %d %d F: \t%u\n", i_Point,
                    this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_XCoords[i_Point],
                    this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_YCoords[i_Point],
                    this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_Flags[i_Point] & TTF_FLAG_ONCURVEPOINT
                );


                if (ISONCURVE(i_Point))
                {
                    if (!ALLOWEDSTEP(i_Point + 1)) // Last step
                    {
                        v_Dots.push_back(V2FROMCOORD(i_Point));
                        continue;
                    }
                    if (ALLOWEDSTEP(i_Point + 1))  // line
                    {
                        if ( ISONCURVE(i_Point + 1))
                        {
                            v_Dots.push_back(V2FROMCOORD(i_Point));
                            continue;
                        }
                    }
                    if (ALLOWEDSTEP(i_Point + 2))  // No mid
                    {
                        if (!ISONCURVE(i_Point+1) && ISONCURVE(i_Point + 2))
                        {
                            for (float f_T = 0; f_T < 1.0f; f_T+=(1.0f/(float)i_DivPerCurve))
                            {
                                V2 v_P = Berzier(V2FROMCOORD(i_Point), V2FROMCOORD(i_Point+2), V2FROMCOORD(i_Point+1),f_T);
                                v_Dots.push_back(v_P);
                            }
                            i_Point += 1;
                            continue;
                        }
                    }
                    if (ALLOWEDSTEP(i_Point + 3))  // With mid
                    {
                        if (!ISONCURVE(i_Point + 1) && !ISONCURVE(i_Point + 2) && ISONCURVE(i_Point + 3))
                        {
                            V2 v_Mid = Midpoint(V2FROMCOORD(i_Point+1), V2FROMCOORD(i_Point+2));
                            for (float f_T = 0; f_T < 1.0f; f_T += (1.0f / (float)i_DivPerCurve))
                            {
                                V2 v_P = Berzier(V2FROMCOORD(i_Point), v_Mid, V2FROMCOORD(i_Point + 1), f_T);
                                v_Dots.push_back(v_P);
                            }

                            for (float f_T = 0; f_T < 1.0f; f_T += (1.0f / (float)i_DivPerCurve))
                            {
                                V2 v_P = Berzier(v_Mid, V2FROMCOORD(i_Point + 3), V2FROMCOORD(i_Point + 2), f_T);
                                v_Dots.push_back(v_P);
                            }
                            i_Point += 2;
                            continue;

                        }
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

            i_Start = this->ttf_pGLYF[this->c_ASCIIMapping[i_Index]].i_EndPtsOfContours[i_Cont]+1;
        }


        this->v_pFont[i_Index].l_pLines = (LINE*)malloc(sizeof(LINE)* o_Lines.size());
        memcpy(this->v_pFont[i_Index].l_pLines, o_Lines.data(), o_Lines.size() * sizeof(LINE));
        this->v_pFont[i_Index].i_Count = o_Lines.size();
        this->v_pFont[i_Index].b_Loaded = TRUE;

        printf("\n\n");
        for (int i = 0; i < o_Lines.size(); i++)
        {
            printf("\t%.0f %.0f %.0f %.0f\n", o_Lines[i].v_Point[0].f_Pos[X], o_Lines[i].v_Point[0].f_Pos[Y], o_Lines[i].v_Point[1].f_Pos[X], o_Lines[i].v_Point[1].f_Pos[Y]);
        }

	}

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
		if ((int)c_new[i_Index] < 0 && (int)c_new[i_Index] >= ASCII_CHARS)
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


void font_handler::Free()
{
}

char font_handler::Load(LPSTR s_Path)
{
    f_File = fopen(s_Path, "rb");
    if (f_File == NULL)
    {
        printf("Failed to open .ttf\n");
        Free();
        return 0;
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
        printf("Memory Error!");
        Free();
        return 0;
    }

    fread(ttf_Tables, sizeof(TTFTABLEDIR), ttf_Header.i_NumTables, f_File);

    for (int index = 0; index < ttf_Header.i_NumTables; index++)
    {

        ttf_Tables[index].i_Checksum = _byteswap_ulong(ttf_Tables[index].i_Checksum);
        ttf_Tables[index].i_Offset = _byteswap_ulong(ttf_Tables[index].i_Offset);
        ttf_Tables[index].i_Length = _byteswap_ulong(ttf_Tables[index].i_Length);

        if (ttf_Tables[index].i_Tag[0] == 'h' && ttf_Tables[index].i_Tag[1] == 'e' && ttf_Tables[index].i_Tag[2] == 'a' && ttf_Tables[index].i_Tag[3] == 'd')
        {
            fseek(f_File, ttf_Tables[index].i_Offset, SEEK_SET);
            fread(&ttf_HEAD, sizeof(TTFHEAD), 1, f_File);
        }
        if (ttf_Tables[index].i_Tag[0] == 'm' && ttf_Tables[index].i_Tag[1] == 'a' && ttf_Tables[index].i_Tag[2] == 'x' && ttf_Tables[index].i_Tag[3] == 'p')
        {
            fseek(f_File, ttf_Tables[index].i_Offset, SEEK_SET);
            fread(&ttf_MAXP, sizeof(TTFMAXP), 1, f_File);
        }
        if (ttf_Tables[index].i_Tag[0] == 'g' && ttf_Tables[index].i_Tag[1] == 'l' && ttf_Tables[index].i_Tag[2] == 'y' && ttf_Tables[index].i_Tag[3] == 'f')
        {
            i_GlyphOffset = ttf_Tables[index].i_Offset;
        }
        if (ttf_Tables[index].i_Tag[0] == 'l' && ttf_Tables[index].i_Tag[1] == 'o' && ttf_Tables[index].i_Tag[2] == 'c' && ttf_Tables[index].i_Tag[3] == 'a')
        {
            i_LocaOffset = ttf_Tables[index].i_Offset;
        }
        if (ttf_Tables[index].i_Tag[0] == 'c' && ttf_Tables[index].i_Tag[1] == 'm' && ttf_Tables[index].i_Tag[2] == 'a' && ttf_Tables[index].i_Tag[3] == 'p')
        {
            i_CmapOffset = ttf_Tables[index].i_Offset;
        }
    }

    ttf_HEAD.i_IndexToLocFormat = _byteswap_ushort(ttf_HEAD.i_IndexToLocFormat);
    ttf_HEAD.i_MagicNumber = _byteswap_ulong(ttf_HEAD.i_MagicNumber);
    ttf_HEAD.i_UnitsPerEm = _byteswap_ushort(ttf_HEAD.i_UnitsPerEm);

    if (ttf_HEAD.i_MagicNumber != 0x5F0F3CF5)
    {
        printf("Magic number nthis->ttf_Header matching!");
        Free();
        return 0;
    }

    ttf_MAXP.i_Version = _byteswap_ulong(ttf_MAXP.i_Version);
    ttf_MAXP.i_NumGlyph = _byteswap_ushort(ttf_MAXP.i_NumGlyph);

    if (ttf_MAXP.i_Version != 65536)
    {
        printf("Invalid TTF Version!");
        Free();
        return 0;
    }


    ttf_pGLYF = (TTFGLYF*)malloc((ttf_MAXP.i_NumGlyph) * sizeof(TTFGLYF));

    fseek(f_File, i_LocaOffset, SEEK_SET);
    if (ttf_HEAD.i_IndexToLocFormat == 1)
    {
        uint32_t* loca = (uint32_t*)malloc((ttf_MAXP.i_NumGlyph + 1) * sizeof(uint32_t));
        if (loca == NULL)
        {
            printf("Memory Error!");
            Free();
            return 0;
        }
        fread(loca, sizeof(uint32_t), ttf_MAXP.i_NumGlyph + 1, f_File);
        for (int index = 0; index < ttf_MAXP.i_NumGlyph; index++)
        {
            ttf_pGLYF[index].i_GlyphStart = i_GlyphOffset + _byteswap_ulong(loca[index]);
            ttf_pGLYF[index].i_GlyphStop = i_GlyphOffset + _byteswap_ulong(loca[index + 1]);
            ttf_pGLYF[index].i_GlyphSize = ttf_pGLYF[index].i_GlyphStop - ttf_pGLYF[index].i_GlyphStart;
        }
        free(loca);
    }
    else
    {
        uint16_t* loca = (uint16_t*)malloc((ttf_MAXP.i_NumGlyph + 1) * sizeof(uint16_t));
        if (loca == NULL)
        {
            printf("Memory Error!");
            Free();
            return 0;
        }
        fread(loca, sizeof(uint16_t), ttf_MAXP.i_NumGlyph + 1, f_File);
        for (int index = 0; index < ttf_MAXP.i_NumGlyph; index++)
        {
            ttf_pGLYF[index].i_GlyphStart = i_GlyphOffset + _byteswap_ushort(loca[index]) * 2;
            ttf_pGLYF[index].i_GlyphStop = i_GlyphOffset + _byteswap_ushort(loca[index + 1]) * 2;
            ttf_pGLYF[index].i_GlyphSize = ttf_pGLYF[index].i_GlyphStop - ttf_pGLYF[index].i_GlyphStart;
        }
        free(loca);
    }

    for (int index = 0; index < ttf_MAXP.i_NumGlyph; index++)
    {
        if (ttf_pGLYF[index].i_GlyphSize < 10) continue;
        fseek(f_File, ttf_pGLYF[index].i_GlyphStart, SEEK_SET);
        fread(&(ttf_pGLYF[index].ttf_HEAD), sizeof(TTFGLYFH), 1, f_File);

        if (ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours & 0x1)
        {
            ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours = 0;
            continue;
        }

        ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours = _byteswap_ushort(ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours);
        ttf_pGLYF[index].ttf_HEAD.i_XMax = _byteswap_ushort(ttf_pGLYF[index].ttf_HEAD.i_XMax);
        ttf_pGLYF[index].ttf_HEAD.i_XMin = _byteswap_ushort(ttf_pGLYF[index].ttf_HEAD.i_XMin);
        ttf_pGLYF[index].ttf_HEAD.i_YMax = _byteswap_ushort(ttf_pGLYF[index].ttf_HEAD.i_YMax);
        ttf_pGLYF[index].ttf_HEAD.i_YMin = _byteswap_ushort(ttf_pGLYF[index].ttf_HEAD.i_YMin);

        ttf_pGLYF[index].i_EndPtsOfContours = (uint16_t*)malloc(sizeof(uint16_t) * ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours);
        if (ttf_pGLYF[index].i_EndPtsOfContours == NULL)
        {
            printf("Memory Error!");
            Free();
            return 0;
        }
        fread(ttf_pGLYF[index].i_EndPtsOfContours, sizeof(uint16_t), ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours, f_File);

        for (int cont = 0; cont < ttf_pGLYF[index].ttf_HEAD.i_NumberOfContours; cont++)
        {
            ttf_pGLYF[index].i_EndPtsOfContours[cont] = _byteswap_ushort(ttf_pGLYF[index].i_EndPtsOfContours[cont]);
            ttf_pGLYF[index].i_NumPoints = ttf_pGLYF[index].i_EndPtsOfContours[cont] + 1;
        }

        fread(&(ttf_pGLYF[index].i_InstructionLength), sizeof(uint16_t), 1, f_File);
        ttf_pGLYF[index].i_InstructionLength = _byteswap_ushort(ttf_pGLYF[index].i_InstructionLength);
        ttf_pGLYF[index].i_Instructions = (uint8_t*)malloc(sizeof(uint8_t) * ttf_pGLYF[index].i_InstructionLength);
        if (ttf_pGLYF[index].i_Instructions == NULL)
        {
            printf("Memory Error!");
            Free();
            return 0;
        }
        fread(ttf_pGLYF[index].i_Instructions, sizeof(uint8_t), ttf_pGLYF[index].i_InstructionLength, f_File);

        ttf_pGLYF[index].i_Flags = (uint8_t*)malloc(sizeof(uint8_t) * ttf_pGLYF[index].i_NumPoints);
        for (int flagIndex = 0; flagIndex < ttf_pGLYF[index].i_NumPoints; flagIndex++)
        {
            uint8_t flag;
            fread(&flag, sizeof(uint8_t), 1, f_File);
            ttf_pGLYF[index].i_Flags[flagIndex] = flag & ~TTF_FLAG_REPEAT;

            if (flag & TTF_FLAG_REPEAT)
            {
                uint8_t repeating;
                fread(&repeating, sizeof(uint8_t), 1, f_File);
                flagIndex++;
                for (int repeat = 0; repeat < repeating; repeat++)
                {
                    ttf_pGLYF[index].i_Flags[flagIndex] = flag & ~TTF_FLAG_REPEAT;
                    flagIndex++;
                }
                flagIndex--;
            }
        }

        ttf_pGLYF[index].i_XCoords = (int32_t*)malloc(sizeof(int32_t) * ttf_pGLYF[index].i_NumPoints);
        ttf_pGLYF[index].i_YCoords = (int32_t*)malloc(sizeof(int32_t) * ttf_pGLYF[index].i_NumPoints);
        if (ttf_pGLYF[index].i_XCoords == NULL || ttf_pGLYF[index].i_YCoords == NULL)
        {
            printf("Memory Error!");
            Free();
            return 0;
        }
        int i_Last = 0;
        for (int flagIndex = 0; flagIndex < ttf_pGLYF[index].i_NumPoints; flagIndex++)
        {
            int i_Delta;
            if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_XSHORTVEC)
            {
                uint8_t i_Read;
                fread(&i_Read, sizeof(uint8_t), 1, f_File);
                if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_XSAME)
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
                if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_XSAME)
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
            ttf_pGLYF[index].i_XCoords[flagIndex] = i_Last + i_Delta;
            i_Last = ttf_pGLYF[index].i_XCoords[flagIndex];

        }
        i_Last = 0;
        for (int flagIndex = 0; flagIndex < ttf_pGLYF[index].i_NumPoints; flagIndex++)
        {
            int i_Delta;
            if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_YSHORTVEC)
            {
                uint8_t i_Read;
                fread(&i_Read, sizeof(uint8_t), 1, f_File);
                if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_YSAME)
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
                if (ttf_pGLYF[index].i_Flags[flagIndex] & TTF_FLAG_YSAME)
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
            ttf_pGLYF[index].i_YCoords[flagIndex] = i_Last + i_Delta;
            i_Last = ttf_pGLYF[index].i_YCoords[flagIndex];
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
        printf("Memory Error!");
        Free();
        return 0;
    }
    fread(ttf_CMAP.ttf_Record, sizeof(TTFENCODINGRECORD), ttf_CMAP.i_NumTables, f_File);

    ttf_CMAP.i_UnicodeSubtableOffset = 0;
    for (int index = 0; index < ttf_CMAP.i_NumTables; index++)
    {
        ttf_CMAP.ttf_Record[index].i_EncodingID = _byteswap_ushort(ttf_CMAP.ttf_Record[index].i_EncodingID);
        ttf_CMAP.ttf_Record[index].i_PlatformID = _byteswap_ushort(ttf_CMAP.ttf_Record[index].i_PlatformID);
        ttf_CMAP.ttf_Record[index].i_SubtableOffset = _byteswap_ulong(ttf_CMAP.ttf_Record[index].i_SubtableOffset);

        if (ttf_CMAP.ttf_Record[index].i_EncodingID == TTF_UNICODE_ENCODINGID && ttf_CMAP.ttf_Record[index].i_PlatformID == TTF_UNICODE_PLATFORMID)
        {
            ttf_CMAP.i_UnicodeSubtableOffset = ttf_CMAP.ttf_Record[index].i_SubtableOffset;
        }
    }

    if (ttf_CMAP.i_UnicodeSubtableOffset == 0)
    {
        printf("No Unicode Table!");
        Free();
        return 0;
    }

    fseek(f_File, i_CmapOffset + ttf_CMAP.i_UnicodeSubtableOffset, SEEK_SET);

    fread(&(ttf_CMAP.i_Format), sizeof(uint16_t), 1, f_File);
    ttf_CMAP.i_Format = _byteswap_ushort(ttf_CMAP.i_Format);

    if (ttf_CMAP.i_Format != TTF_FORMAT)
    {
        printf("Wrong Format!");
        Free();
        return 0;
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
        printf("Memory Error!");
        Free();
        return 0;
    }
    fread(ttf_CMAP.i_EndCode, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);
    fread(&(ttf_CMAP.i_ReservedPad), sizeof(uint16_t), 1, f_File);
    fread(ttf_CMAP.i_StartCode, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);
    fread(ttf_CMAP.i_IdDelta, sizeof(int16_t), ttf_CMAP.i_SegCount, f_File);
    fread(ttf_CMAP.i_IdRangeOffset, sizeof(uint16_t), ttf_CMAP.i_SegCount, f_File);

    for (int index = 0; index < ttf_CMAP.i_SegCount; index++)
    {
        ttf_CMAP.i_EndCode[index] = _byteswap_ushort(ttf_CMAP.i_EndCode[index]);
        ttf_CMAP.i_StartCode[index] = _byteswap_ushort(ttf_CMAP.i_StartCode[index]);
        ttf_CMAP.i_IdDelta[index] = (int16_t)_byteswap_ushort(ttf_CMAP.i_IdDelta[index]);
        ttf_CMAP.i_IdRangeOffset[index] = _byteswap_ushort(ttf_CMAP.i_IdRangeOffset[index]);
    }

    uint32_t i_GlyphIdArraySize = (ttf_CMAP.i_Length - (16 + 8 * ttf_CMAP.i_SegCount)) / 2;
    ttf_CMAP.i_GlyphIdArray = (uint16_t*)malloc(sizeof(uint16_t) * i_GlyphIdArraySize);
    if (ttf_CMAP.i_GlyphIdArray == NULL)
    {
        printf("Memory Error!");
        Free();
        return 0;
    }

    fread(ttf_CMAP.i_GlyphIdArray, sizeof(uint16_t), i_GlyphIdArraySize, f_File);
    for (int i_Index = 0; i_Index < i_GlyphIdArraySize; i_Index++)
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
}


UCHAR font_handler::Dispose() 
{
	for (int i_Index = 0; i_Index < ASCII_CHARS; i_Index++)
	{
		if (this->v_pFont[i_Index].b_Loaded == FALSE)
		{
			continue;
		}
		this->v_pFont[i_Index].Dispose();
	}
	return GD_TASK_OKAY;
}

UCHAR cmap::Dispose()
{
    if (this->l_pLines != NULL)
    {
        free(this->l_pLines);
    }
    return GD_TASK_OKAY;
}
