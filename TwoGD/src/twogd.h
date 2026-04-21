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

typedef struct o_win {
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
	UINT32 i_Width, i_Height;
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
}GDWIN;

extern HANDLE  CreateExec(HINSTANCE h_Instance);
extern long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

extern void gdCreateWinExec(GDWIN *);
extern unsigned char  gdMain(GDWIN *);
extern DWORD*  gdUpdate(GDWIN *);
extern void  gdClose();

typedef unsigned char UCHAR;
typedef unsigned int UINT32;

#define __REGISTER register
#define PTOV2(POI) V2((float)((POINT)POI).x,(float)((POINT)POI).y)
#define X 0
#define Y 1
#define Z 2

typedef class o_vec2 {
public:
	float f_Pos[Z];

	void Delta(o_vec2 v_Pos);
	double Distance(o_vec2 v_Pos);
	double Length();

	o_vec2();
	o_vec2(float f_X, float f_Y);
	o_vec2(float f_Pos[2]);

}V2;

V2 operator - (V2  &v_Pos1, V2  &v_Pos2);
V2 operator + (V2  &v_Pos1, V2  &v_Pos2);
V2 operator * (V2  &v_Pos1, int &i_Lenght);
V2 operator * (V2  &v_Pos1, float &f_Lenght);


typedef class o_vec3 {
public:
	float f_Pos[3];

	void NormalizeThis();
	o_vec3 NormalizeTo();
	
	void RotateThis(o_vec3 v_Rot);
	o_vec3 RotateTo(o_vec3 v_Rot);
	void CamRotateThis(o_vec3 v_Rot);
	void CamRotateThisOpt(o_vec3 v_Rot);
	void CamRotateThisStatic(o_vec3 v_Rot);
	o_vec3 CamRotateTo(o_vec3 v_Rot);
	void RotateAroundThis(o_vec3 v_UnitV, float f_phi);
	o_vec3 RotateAroundTo(o_vec3 v_UnitV, float f_phi);

	void DeltaThis(o_vec3 v_Pos);
	o_vec3 DeltaTo(o_vec3 v_Pos);

	o_vec3 AngleTo(o_vec3 v_Pos);
	o_vec3 Angle();

	double DistanceTo(o_vec3 v_Pos);
	float Length();
	float DotProduct(o_vec3 v_Pos);

	o_vec3();
	o_vec3(float f_X, float f_Y, float f_Z);
	o_vec3(float f_Pos[3]);

}V3;

V3 operator-(V3  &v_Pos1, V3  &v_Pos2);
V3 operator+(V3  &v_Pos1, V3  &v_Pos2);
V3 operator*(V3  &v_Pos1, V3  &v_Pos2);
V3 operator*(V3  &v_Pos1, V3  const&v_Pos2);
V3 operator*(V3  &v_Pos, float const&f_Lenght);
V3 operator*(V3  &v_Pos, float &f_Lenght);

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
	UINT32 i_Pixels[2];

	UINT32 i_OutputSize;
	DWORD * d_pOutputStream;
	UCHAR * d_pPixelFlags;
	UCHAR * d_pPrioFlags;


	UCHAR  CleanBuffer();

	UCHAR  Prepare(int i_Width, int i_Height);
	UCHAR  Dispose();
}CANVAS;

typedef struct o_color {
	UCHAR c_Color[3];
	o_color(UCHAR c_R, UCHAR c_G, UCHAR c_B);
	o_color();
	DWORD GetAsHex();
} COLOR;

static const COLOR co_Gray = COLOR(55, 55, 55);
static const COLOR co_White = COLOR(255, 255, 255);
static const COLOR co_Red = COLOR(0, 0, 255);
static const COLOR co_Green = COLOR(0, 255, 0);
static const COLOR co_Blue = COLOR(255, 0, 0);
static const COLOR co_Pink = COLOR(255, 0, 255);


typedef struct o_line {
	V2 v_Point[2];
	COLOR c_Color;
}LINE;

typedef struct o_face {
	V3 v_Point[3];
}FACE;


typedef class filer {
protected:
	FILE * f_Stream = NULL;
	UCHAR  OpenStream(LPSTR c_StreamName);
	UCHAR  CloseStream();

}FILER;


typedef class object : public FILER {
public:
	object();
	object(LPSTR c_StreamName);

	V3 v_Anchor;
	V3 * v_pPoint;
	COLOR c_pColor;
	FACE * o_pFace;
	UINT32 i_Faces, i_Points;

private:
	UCHAR  ReadHeader();
	UCHAR  LoadFile();
	UCHAR  Prepare();
public:
	UCHAR  Read(LPSTR c_StreamName);
	UCHAR  Dispose();
}OBJ3D;


typedef class vectormap : public FILER {
public:
	BOOL b_Loaded;
	V2 * v_pPoint;
	COLOR * c_pColor;
	LINE * l_pLines;
	UINT32 i_Connections, i_Points, i_Colors;
	UINT32 i_Width, i_Height;
	vectormap();
private:
	UCHAR  ReadHeader();
	UCHAR  LoadFile();
	UCHAR  Prepare();
public:
	UCHAR  Read(LPSTR c_StreamName);
	UCHAR  Dispose();
}VMAP;

typedef class layer {
public:
	layer();
	layer(LPSTR c_StreamName, COLOR c_Color);

	UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED;
	UINT32 i_PrioFlag =0;
	OBJ3D o_Obj;


	COLOR c_Color;
	BOOL b_Enabled = TRUE;
}LAYER;

typedef class cmap {
public:
	cmap();
	BOOL b_Loaded;
	LINE* l_pLines;
	UINT32 i_Count;
	UINT32 i_Width;
	UINT32	i_Height;
	int16_t i_XMin;
	int16_t i_YMin;
	int16_t i_XMax;
	int16_t i_YMax;

	UCHAR Dispose();
}CHARMAP;


typedef class codec2d {
public:
	codec2d() {}
	codec2d(CANVAS * o_pCanvas){
		o_Image = o_pCanvas;
	}
	UCHAR  SetPixel(V2 * v_pPoint, COLOR * c_pColor,UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawRect(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawHLine(V2 * v_pPoint, UINT32  i_Length, COLOR * c_pColor, UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawVLine(V2 * v_pPoint, UINT32  i_Length, COLOR * c_pColor, UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawCanvas(DWORD * d_pBuffer, V2 * v_pPos, UINT32  i_Pixels[2], UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawVMap(VMAP * o_VecMap, V2* v_pAnchor, float f_Scale , UCHAR i_PixelFlag , UCHAR i_PrioFlag );
	UCHAR  DrawChar(CHARMAP* o_VecMap, V2* v_pAnchor, COLOR* c_pColor, float f_Scale);

	BOOL b_AllowPixelOverwrite = TRUE;
protected:
	CANVAS * o_Image;

}CODEC2D;

typedef class camera {
public:
	camera();
	camera(
		FLOAT f_Frustum_Low,
		FLOAT f_Frustum_Far,
		UINT32 i_Dimensions_Width,
		UINT32 i_Dimensions_Height,
		FLOAT f_FOV,
		V3 i_Position,
		V3 i_Rotation
	);
	UCHAR(*s_Shader)(camera *o_Cam, V3 *v_Vertex, V2 *v_Point, COLOR *c_Color)= NULL;

	FLOAT f_Frustum[2];
	UINT32 i_Dimensions[2];
	FLOAT f_FOV;
	V3 i_Position;
	V3 i_Rotation;
	FLOAT f_CutoffAngles[2];

	BOOL b_UseCutoffAngles = TRUE;

	inline UCHAR  Translate(V3 * v_pPoint, V2 * v_pResult);
	UCHAR  Relate(V2 * v_pScreenPos, V3 * v_pAngle);
}CAM3D;


typedef class codec3d : public CODEC2D {
public:
	codec3d() {}
	codec3d(CANVAS * o_pCanvas, CAM3D * o_pCamera) {
		o_Image = o_pCanvas;
		o_Camera = o_pCamera;
	}
	UCHAR  DrawObject(OBJ3D * o_Object, COLOR * c_pColor, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED,UCHAR i_PrioFlag = 0);
	UCHAR  DrawEdge(V3 * v_pVertexA, V3 * v_pVertexB, COLOR * c_pColor, UCHAR i_PixelFlag= PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);

	CAM3D * o_Camera;
}CODEC3D;


typedef class o_console {
public:
	FILE *  Create(const wchar_t* c_ConsoleTitle);
	FILE*  Create();

	void  Clear();
	BOOL  Destroy();
	BOOL  Rename(const wchar_t* c_ConsoleTitle);

	void  SetColor(const int i_HexColor);
private:
	FILE *  AllocC();
}CONSOLE;



#define CHST_BASIC 0xA
#define CHST_AXIS  0xB
	
typedef class o_camctrlr {
public:
	o_camctrlr();
	o_camctrlr( CAM3D* o_camera, CODEC3D* o_pCodec);
	CAM3D* o_camera;
	CODEC3D* o_pCodec;

	BOOL b_MouseCtrl = TRUE;
	float f_MoveSpeed = 0.1f;
	float f_ViewSpeed = 0.05f;
	float f_MouseSensitivity = 2.0f;

	BOOL b_ShowCH = TRUE;
	COLOR c_CHColor = co_White;
	int i_CHStyle = CHST_AXIS;
	float f_CHSize = 0.2f;


	void UpdateCamCtrlr(GDWIN *o_win);
	void DrawCrosshair();
}CAMCTRLR;



typedef struct o_m3x3 {
public:
	o_m3x3();
	o_m3x3(V3 v_v1, V3 v_v2, V3 v_v3);
	o_m3x3(float f_UnitValue);

	V3 v_v1, v_v2, v_v3;

	void print();
} M3X3;

M3X3 M3X3RotX(float f_phi);
M3X3 M3X3RotY(float f_phi);
M3X3 M3X3RotZ(float f_phi);
M3X3 M3X3RotU(V3 v_UnitV,float f_phi);

M3X3 operator * (M3X3  &m_M, float &f_S);
M3X3 operator * (M3X3  &m_M1, M3X3 &m_M2);
M3X3 operator + (M3X3  &m_M1, M3X3 &m_M2);
V3 operator * (M3X3  &m_M, V3 &v_V);

M3X3 operator * (M3X3  const&m_M, float const&f_S);
M3X3 operator * (M3X3  const&m_M1, M3X3 const&m_M2);
M3X3 operator + (M3X3  const&m_M1, M3X3 const&m_M2);
V3 operator * (M3X3  const&m_M, V3 const&v_V);



typedef class o_world {
	UINT32 i_Length;
public:
	UINT32 Length() const { return i_Length; }
	LAYER * o_Layers = NULL;
	CODEC3D * o_Codec = NULL;

	UINT32 AppendLayer(LPSTR c_StreamName, COLOR c_Color);
	UCHAR GetObjInCrosshair(CAM3D * o_pCam, FLOAT f_CutoffAngleInRad, UINT32 * i_pLayer);
	UCHAR Render();
	UCHAR Dispose();

	o_world();
	o_world(CODEC3D * o_Codec);
}WORLD;


typedef class perlog {
protected:
	std::chrono::high_resolution_clock::time_point a_Start, a_Stop;
	int * i_Buffer;
	int i_BufferSize;
public:
	perlog(int i_BufferSize);
	perlog();

	void Start();
	void Stop();
	float GetDelta();
	UCHAR Dispose();
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
	uint16_t* i_EndPtsOfContours; // ttf_HEAD.i_NumberOfContours
	uint16_t i_NumPoints;
	uint16_t i_InstructionLength;
	uint8_t* i_Instructions; // i_InstructionLength
	uint8_t* i_Flags; // i_NumPoints
	int32_t* i_XCoords;// i_NumPoints
	int32_t* i_YCoords;// i_NumPoints
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
	font_handler() {}
	font_handler(CODEC2D* o_pCodec, LPSTR c_pFontPath, int i_DivPerCurve=4);
	void Write(V2 v_pAnchor, float f_Scale, const char* c_pformat, ...);
	UCHAR Dispose();

	UCHAR i_LastError;
	UINT32 i_Padding;
	UINT32 i_SpaceWidth;
	COLOR c_Color;
protected:
	UCHAR Load(LPSTR s_Path);
	void Free();

	CODEC2D* o_pCodec;
	CHARMAP v_pFont[ASCII_CHARS];

	FILE* f_File;

	TTFHEAD ttf_HEAD;
	TTFMAXP ttf_MAXP;
	TTFCMAP ttf_CMAP;
	TTFGLYF* ttf_pGLYF; // ttf_MAXP.i_NumGlyph
	TTFHEADER ttf_Header;
	TTFTABLEDIR* ttf_Tables; // ttf_Header.i_NumTables
	uint32_t i_GlyphOffset;
	uint32_t i_LocaOffset;
	uint32_t i_CmapOffset;
	uint16_t c_ASCIIMapping[ASCII_CHARS];
}FONTHANDLER;