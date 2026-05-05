// TwoGD Win32 Socket for C++ 
// Markus Kotar 2026

#pragma once
#define UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <chrono>
#include <xmmintrin.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>
#include <stdarg.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

typedef struct win {
	DWORD dw_ExStyle = 0;
	wchar_t* c_WinClassName = (wchar_t*)"WINCLASSEWS";
	DWORD  dw_Style;
	int i_XPos = CW_USEDEFAULT;
	int i_YPos = CW_USEDEFAULT;
	int i_CmdShow;
	HWND hd_WndParent = NULL;
	HMENU h_Menu = NULL;
	LPVOID lv_Param = NULL;
	wchar_t* c_WinTitle;
	uint32_t i_Width, i_Height;
	uint32_t i_FullScreenWidth, i_FullScreenHeight;
	HINSTANCE h_Instance;
	HWND hd_WindowHandle;
	MSG msg_WindowMessage;
	HDC hdc_WindowHdc;
	WNDCLASS w_WndClass;
	POINT v_CursorPos;
	void(*v_pLeftMouseDown)(POINT v_ClickPoint);
	void(*v_pLeftMouseUp)(POINT v_ClickPoint);
	void(*v_pRightMouseDown)(POINT v_ClickPoint);
	void(*v_pRightMouseUp)(POINT v_ClickPoint);
	void(*v_pMouseScroll)(BOOL b_Up, POINT v_ScrollPoint);
	BOOL b_HasFocus;
	BOOL b_HideCursor = FALSE;
}WIN;

extern HANDLE  CreateExec(HINSTANCE h_Instance);
extern long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

extern void gdCreateWinExec(WIN*);
extern unsigned char  gdMain(WIN*);
extern DWORD* gdUpdate(WIN*);
extern void  gdClose();

typedef unsigned char uint8_t;

#define __REGISTER register
#define PTOV2(POI) V2((float)((POINT)POI).x,(float)((POINT)POI).y)
#define X 0
#define Y 1
#define Z 2

typedef class vec2 { 
public:
	vec2();
	vec2(float f_X, float f_Y);
	vec2(float f_Pos[2]);

	float f_Pos[Z];

	void Delta(vec2 v_Pos);
	double Distance(vec2 v_Pos);
	double Length();
}V2;

V2 operator - (V2& v_Pos1, V2& v_Pos2);
V2 operator + (V2& v_Pos1, V2& v_Pos2);
V2 operator * (V2& v_Pos1, int& i_Lenght);
V2 operator * (V2& v_Pos1, float& f_Lenght);

typedef class vec3 { 
public:
	vec3();
	vec3(float f_X, float f_Y, float f_Z);
	vec3(float f_Pos[3]);

	void NormalizeThis();
	vec3 NormalizeTo();
	void RotateThis(vec3 v_Rot);
	vec3 RotateTo(vec3 v_Rot);
	void CamRotateThis(vec3 v_Rot);
	void CamRotateThisOpt(vec3 v_Rot);
	void CamRotateThisStatic(vec3 v_Rot);
	vec3 CamRotateTo(vec3 v_Rot);
	void RotateAroundThis(vec3 v_UnitV, float f_phi);
	vec3 RotateAroundTo(vec3 v_UnitV, float f_phi);
	void DeltaThis(vec3 v_Pos);
	vec3 DeltaTo(vec3 v_Pos);
	vec3 AngleTo(vec3 v_Pos);
	vec3 Angle();
	double DistanceTo(vec3 v_Pos);
	float Length();
	float DotProduct(vec3 v_Pos);
	void CrossProduct(vec3 v_Pos);

	float f_Pos[3];
}V3;

V3 operator-(V3& v_Pos1, V3& v_Pos2);
V3 operator+(V3& v_Pos1, V3& v_Pos2);
V3 operator*(V3& v_Pos1, V3& v_Pos2);
V3 operator*(V3& v_Pos1, V3  const& v_Pos2);
V3 operator*(V3& v_Pos, float const& f_Lenght);
V3 operator*(V3& v_Pos, float& f_Lenght);

double Distance2(V2 v_PosOne, V2 v_PosTwo);
double Distance3(V3 v_PosOne, V3 v_PosTwo);

#define PF_OVERWRITE_ALLOWED   0xA
#define PF_OVERWRITE_FORBIDDEN 0xF
#define PF_FONT 0xF0

#define GD_ALLOC_FAILED 0x1F
#define GD_TASK_OKAY 0x1A
#define GD_FILE_FAILED 0x2F
#define GD_OUTOFBOUND 0x2E
#define GD_TTF_ERROR 0xF0

#define DEGTORAD(X) (float)(X*2*M_PI/360.0)
#define RADTODEG(X) (float)(X/(2*M_PI)*360.0)

typedef class canvas { 
public:
	canvas();
	canvas(uint32_t i_Width, uint32_t i_Height);

	uint32_t i_Pixels[2];
	uint32_t i_OutputSize;
	DWORD* d_pOutputStream;
	uint8_t* d_pPixelFlags;
	uint8_t* d_pPrioFlags;

	uint8_t CleanBuffer();
	void Dispose();
}CANVAS;

typedef class color { 
public:
	color();
	color(uint8_t c_R, uint8_t c_G, uint8_t c_B);
	
	uint8_t c_Color[3];
	
	DWORD GetAsHex();
} COLOR;

static const COLOR co_Gray = COLOR(55, 55, 55);
static const COLOR co_White = COLOR(255, 255, 255);
static const COLOR co_Red = COLOR(0, 0, 255);
static const COLOR co_Green = COLOR(0, 255, 0);
static const COLOR co_Blue = COLOR(255, 0, 0);
static const COLOR co_Pink = COLOR(255, 0, 255);

typedef struct line {
	V2 v_Point[2];
	COLOR c_Color;
}LINE;

typedef struct face {
	V3 v_Point[3];
}FACE;


#define GD_VEC_HEADER "v %d %d %d\n"
#define GD_VEC_POINTS "p %f %f\n"
#define GD_VEC_COLORS "c %d %d %d\n"
#define GD_VEC_LINE "l %d %d %d\n"

typedef class filer { 
protected:
	uint8_t  OpenStream(LPSTR c_StreamName);
	void  CloseStream();

	FILE* f_Stream = NULL;
}FILER;

typedef class object : public FILER {
public:
	object();
	object(LPSTR c_StreamName);

	BOOL b_Loaded;
	V3 v_Anchor;
	V3* v_pPoint;
	COLOR c_pColor;
	FACE* o_pFace;
	uint32_t i_Faces, i_Points;

private:
	uint8_t ReadHeader();
	uint8_t LoadFile();
	uint8_t Prepare();
	uint8_t Read(LPSTR c_StreamName);

public:
	void Dispose();
}OBJ3D;

typedef class vectormap : public FILER {
public:
	vectormap();
	vectormap(LPSTR c_StreamName);

	BOOL b_Loaded;
	V2* v_pPoint;
	COLOR* c_pColor;
	LINE* l_pLines;
	uint32_t i_Connections, i_Points, i_Colors;
	uint32_t i_Width, i_Height;

private:
	uint8_t  ReadHeader();
	uint8_t  LoadFile();
	uint8_t  Prepare();
	uint8_t  Read(LPSTR c_StreamName);

public:
	void  Dispose();
}VMAP;

V2 Berzier(V2 v_A, V2 v_B, V2 v_S1, float f_T);

typedef struct cmap {
	BOOL b_Loaded;
	LINE* l_pLines;
	uint32_t i_Count;
	uint32_t i_Width;
	uint32_t i_Height;
	int16_t i_XMin;
	int16_t i_YMin;
	int16_t i_XMax;
	int16_t i_YMax;
}CHARMAP;

typedef class codec2d { 
public:
	codec2d();
	codec2d(CANVAS* o_pCanvas);

	BOOL b_AllowPixelOverwrite = TRUE;

	uint8_t  SetPixel(V2* v_pPoint, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawLine(V2* v_pPointA, V2* v_pPointB, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawBezier(V2* v_pPointA, V2* v_pPointS, V2* v_pPointB, COLOR* c_pColor, int i_DivPerCurve = 4, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawRect(V2* v_pPointA, V2* v_pPointB, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawHLine(V2* v_pPoint, uint32_t  i_Length, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawVLine(V2* v_pPoint, uint32_t  i_Length, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawCanvas(DWORD* d_pBuffer, V2* v_pPos, uint32_t  i_Pixels[2], uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawVMap(VMAP* o_VecMap, V2* v_pAnchor, float f_Scale, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawChar(CHARMAP* o_VecMap, V2* v_pAnchor, COLOR* c_pColor, float f_Scale);

protected:
	CANVAS* o_Image;

}CODEC2D;

typedef class camera {
public:
	camera();
	camera(uint32_t i_Width, uint32_t i_Height);

	uint8_t(*s_Shader)(camera* o_Cam, V3* v_Vertex, V2* v_Point, COLOR* c_Color) = NULL;
	FLOAT f_Frustum[2];
	uint32_t i_Dimensions[2];
	FLOAT f_FOV;
	V3 i_Position;
	V3 i_Rotation;
	FLOAT f_CutoffAngles[2];
	BOOL b_UseCutoffAngles = TRUE;

	uint8_t  Translate(V3* v_pPoint, V2* v_pResult);
	uint8_t  Relate(V2* v_pScreenPos, V3* v_pAngle);
}CAM3D;

typedef class codec3d : public CODEC2D {
public:
	codec3d();
	codec3d(CANVAS* o_pCanvas, CAM3D* o_pCamera);

	CAM3D* o_Camera;

	uint8_t  DrawObjectFilled(OBJ3D* o_Object, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawObjectWireframe(OBJ3D* o_Object, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawEdge(V3* v_pVertexA, V3* v_pVertexB, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
	uint8_t  DrawFace(FACE* o_Face, COLOR* c_pColor, uint8_t i_PixelFlag = PF_OVERWRITE_ALLOWED, uint8_t i_PrioFlag = 0);
}CODEC3D;

typedef class console {
public:
	console();

	FILE* f_Console;
	HANDLE h_ConsoleHandle;

	void  Clear();
	BOOL  Destroy();
	BOOL  Rename(const wchar_t* c_ConsoleTitle);
	void  SetColor(int i_HexColor);
}CONSOLE;

#define CHST_BASIC 0xA
#define CHST_AXIS  0xB

typedef class camctrlr {
public:
	camctrlr();
	camctrlr(CAM3D* o_camera, CODEC3D* o_pCodec);

	CAM3D* o_camera;
	CODEC3D* o_pCodec;
	BOOL b_MouseCtrl = TRUE;
	float f_MoveSpeed = 0.4f;
	float f_ViewSpeed = 0.1f;
	float f_MouseSensitivity = 2.0f;
	BOOL b_ShowCH = TRUE;
	COLOR c_CHColor = co_White;
	int i_CHStyle = CHST_AXIS;
	float f_CHSize = 0.2f;

	void UpdateCamCtrlr(WIN* o_Win);
	void DrawCrosshair();
}CAMCTRLR;

typedef struct m3x3 {
public:
	m3x3();
	m3x3(V3 v_V1, V3 v_V2, V3 v_V3);
	m3x3(float f_UnitValue);

	V3 v_v1, v_v2, v_v3;

} M3X3;

M3X3 M3X3RotX(float f_phi);
M3X3 M3X3RotY(float f_phi);
M3X3 M3X3RotZ(float f_phi);
M3X3 M3X3RotU(V3 v_UnitV, float f_phi);

M3X3 operator * (M3X3& m_M, float& f_S);
M3X3 operator * (M3X3& m_M1, M3X3& m_M2);
M3X3 operator + (M3X3& m_M1, M3X3& m_M2);
V3 operator * (M3X3& m_M, V3& v_V);
M3X3 operator * (M3X3  const& m_M, float const& f_S);
M3X3 operator * (M3X3  const& m_M1, M3X3 const& m_M2);
M3X3 operator + (M3X3  const& m_M1, M3X3 const& m_M2);
V3 operator * (M3X3  const& m_M, V3 const& v_V);

typedef class perlog {
public:
	perlog();
	perlog(int i_BufferSize);

	void Start();
	void Stop();
	float GetDelta();
	uint8_t Dispose();
private:
	std::chrono::high_resolution_clock::time_point a_Start, a_Stop;
	int* i_Buffer;
	int i_BufferSize;
} PERLOG;

typedef struct {
	uint32_t i_ScalerType;
	uint16_t i_NumTables;
	uint16_t i_SearchRange;
	uint16_t i_EntrySelector;
	uint16_t i_RangeShift;
} TTFHEADER;

typedef struct {
	uint8_t i_Tag[4];
	uint32_t i_Checksum;
	uint32_t i_Offset;
	uint32_t i_Length;
} TTFTABLEDIR;

typedef struct {
	uint32_t i_Version;
	uint32_t i_FontRevision;
	uint32_t i_CheckSumAdjustment;
	uint32_t i_MagicNumber;
	uint16_t i_Flags;
	uint16_t i_UnitsPerEm;
	uint64_t i_Created;
	uint64_t i_Modified;
	uint16_t i_XMin;
	uint16_t i_YMin;
	uint16_t i_XMax;
	uint16_t i_YMax;
	uint16_t i_MacStyle;
	uint16_t i_LowestRecPPEM;
	uint16_t i_FontDirectionHint;
	uint16_t i_IndexToLocFormat;
	uint16_t i_GlyphDataFormat;
}TTFHEAD;

typedef struct {
	uint32_t i_Version;
	uint16_t i_NumGlyph;
}TTFMAXP;

typedef struct {
	int16_t i_NumberOfContours;
	int16_t i_XMin;
	int16_t i_YMin;
	int16_t i_XMax;
	int16_t i_YMax;
} TTFGLYFH;

typedef struct {
	uint32_t i_GlyphStart;
	uint32_t i_GlyphStop;
	uint32_t i_GlyphSize;
	TTFGLYFH ttf_HEAD;
	uint16_t* i_EndPtsOfContours;
	uint16_t i_NumPoints;
	uint16_t i_InstructionLength;
	uint8_t* i_Instructions;
	uint8_t* i_Flags;
	int32_t* i_XCoords;
	int32_t* i_YCoords;
} TTFGLYF;

typedef struct {
	uint16_t i_PlatformID;
	uint16_t i_EncodingID;
	uint32_t i_SubtableOffset;
} TTFENCODINGRECORD;

typedef struct {
	uint16_t i_Version;
	uint16_t i_NumTables;
	TTFENCODINGRECORD* ttf_Record;
	uint32_t i_UnicodeSubtableOffset;
	uint16_t i_Format;
	uint16_t i_Length;
	uint16_t i_Language;
	uint16_t i_SegCount;
	uint16_t i_SearchRange;
	uint16_t i_EntrySelector;
	uint16_t i_RangeShift;
	uint16_t* i_EndCode;
	uint16_t i_ReservedPad;
	uint16_t* i_StartCode;
	int16_t* i_IdDelta;
	uint16_t* i_IdRangeOffset;
	uint16_t* i_GlyphIdArray;
} TTFCMAP;

#define TTF_FLAG_ONCURVEPOINT (uint8_t)(1)
#define TTF_FLAG_XSHORTVEC (uint8_t)(1<<1)
#define TTF_FLAG_YSHORTVEC (uint8_t)(1<<2)
#define TTF_FLAG_REPEAT (uint8_t)(1<<3)
#define TTF_FLAG_XSAME (uint8_t)(1<<4)
#define TTF_FLAG_YSAME (uint8_t)(1<<5)
#define TTF_FORMAT 4
#define TTF_UNICODE_ENCODINGID 1
#define TTF_UNICODE_PLATFORMID 3
#define ASCII_CHARS 128
#define ASCII_CHAR_START '!'
#define ASCII_CHAR_STOP '}'
#define MAXIMAL_TEXT_LENGTH 256

typedef class font_handler {
public:
	font_handler();
	font_handler(CODEC2D* o_pCodec, LPSTR c_pFontPath, int i_DivPerCurve = 4);

	uint8_t i_LastError;
	uint32_t i_Padding;
	uint32_t i_SpaceWidth;
	COLOR c_Color;

	void Write(V2 v_pAnchor, float f_Scale, const char* c_pformat, ...);
	void Dispose();

private:
	CODEC2D* o_pCodec;
	CHARMAP v_pFont[ASCII_CHARS];
	FILE* f_File;
	TTFHEAD ttf_HEAD;
	TTFMAXP ttf_MAXP;
	TTFCMAP ttf_CMAP;
	TTFGLYF* ttf_pGLYF;
	TTFHEADER ttf_Header;
	TTFTABLEDIR* ttf_Tables;
	uint32_t i_GlyphOffset;
	uint32_t i_LocaOffset;
	uint32_t i_CmapOffset;
	uint16_t c_ASCIIMapping[ASCII_CHARS];

	uint8_t Load(LPSTR s_Path);
	void Free();
}FONTHANDLER;


typedef class gui_handler {
public:
	gui_handler() {}
	gui_handler(CODEC2D* o_pCodec, FONTHANDLER* o_pFont);
private:
	CODEC2D* o_pCodec;
	FONTHANDLER* o_pFont;
} GUI;