// TwoGD Win32Socket for C++ 
// Markus Kotar 2024

/*
ENTRYPOINT
-> wWinMainCRTStartup

FUNCTIONS
unsigned char  gdmain(win::GDWIN *) { return TRUE; } //Called once before Window Creation
unsigned char  gdupdate(win::GDWIN *) { return 1; } // Called every "WindowProc" Call
void  gdclose() { } // Gets Called before window closing
*/

#pragma once

#define _ALLOW_REGISTER_USE
#define _CRT_SECURE_NO_WARNINGS

#ifndef UNICODE
#define UNICODE
#endif 

#ifdef _ALLOW_REGISTER_USE
#define __REGISTER register
#else 
#define __REGISTER 
#endif


#include <iostream>
#include <xmmintrin.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

#define GD_ALLOC_FAILED 0x1F
#define GD_TASK_OKAY 0x1A
#define GD_FILE_FAILED 0x2F
#define GD_OUTOFBOUND 0x2F

#define _TOCLIENTWIDTH(x) (win::i_Width-16)
#define _TOCLIENTHEIGHT(x) (win::i_Height-39)
#define _FROMCLIENTWIDTH(x) (x+16)
#define _FROMCLIENTHEIGHT(x) (x+39)
#define _TOINDEX(x,y) ((int)y * gd_Image->i_Pixels[0] + (int)x)
#define _TOROW(y) ((int)y * gd_Image->i_Pixels[0])
#define _PTOP(POI) GDVEC2(((POINT)POI).x,((POINT)POI).y)

typedef unsigned char UCHAR;
typedef unsigned int UINT32;

extern HANDLE  CreateExec(HINSTANCE h_Instance);
extern long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

namespace win {
	const static DWORD dw_ExStyle = 0;
	const static wchar_t* c_WinClassName = L"WINCLASSEWS";
	const static wchar_t* c_WinTitle = L"";
	const static DWORD  dw_Style = (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX);
	const static int i_XPos = CW_USEDEFAULT;
	const static int i_YPos = CW_USEDEFAULT;
	const static HWND hd_WndParent = NULL;
	const static HMENU h_Menu = NULL;
	const static  LPVOID lp_Param = NULL;

	typedef struct gd_win {
		UINT32 i_Width, i_Height;
		HINSTANCE h_Instance;
		HWND hd_WindowHandle;
		MSG msg_WindowMessage;
		HDC hdc_WindowHdc;
		WNDCLASS w_WndClass;
		POINT p_CursorPos;
		void(*v_pMouseDown)();
	}GDWIN;

}

extern unsigned char  gdMain(win::GDWIN *);
extern unsigned char  gdUpdate(win::GDWIN *);
extern void  gdClose();

typedef class gd_console {
public:
	FILE *  Create(const wchar_t* c_ConsoleTitle);
	FILE*  Create();

	void  Clear();
	BOOL  Destroy();
	BOOL  Rename(const wchar_t* c_ConsoleTitle);

	void  SetColor(const int i_HexColor);
private:
	FILE *  AllocC();
}GDCONSOLE;

typedef struct gd_color {
	UCHAR c_Color[3];
	gd_color(UCHAR c_R, UCHAR c_G, UCHAR c_B);
	gd_color();
	DWORD GetAsHex();
} GDCOLOR;

typedef class gd_vec2 {
public:
	float f_Pos[2];

	void Delta(gd_vec2 p_Pos);
	double Distance(gd_vec2 p_Pos);

	gd_vec2();
	gd_vec2(float f_X, float f_Y);
	gd_vec2(float f_Pos[2]);

}GDVEC2;

typedef class gd_vec3 {
public:
	float f_Pos[3];

	void Delta(gd_vec3 p_Pos);
	double Distance(gd_vec3 p_Pos);
	float Length();

	gd_vec3();
	gd_vec3(float f_X, float f_Y, float f_Z);
	gd_vec3(float f_Pos[3]);

}GDVEC3;

typedef struct gd_line {
	GDVEC2 p_Point[2];
	GDCOLOR c_Color;
}GDLINE;

typedef struct gd_face {
	GDVEC3 p_Point[3];
}GDFACE;

double Distance2(GDVEC2 p_PosOne, GDVEC2 p_PosTwo);
double Distance3(GDVEC3 p_PosOne, GDVEC3 p_PosTwo);

GDVEC2 operator - (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2);
GDVEC2 operator + (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2);
GDVEC2 operator * (GDVEC2  &p_Pos1, int &i_Lenght);

GDVEC3 operator - (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2);
GDVEC3 operator + (GDVEC3  &p_Pos1, GDVEC3  &p_Pos2);
GDVEC3 operator * (GDVEC3  &p_Pos1, float &i_Lenght);


extern void  SetScreenBuffer(DWORD* dw_ColorStream, int i_Width, int i_Height);
extern void  GetCursorPosition();

typedef class canvas {
public:
	UINT32 i_Pixels[2];
	DWORD * d_pOutputStream;
	UINT32 i_OutputSize;

	UCHAR  CleanBuffer();

	UCHAR  Prepare(int i_Width, int i_Height);
	UCHAR  Dispose();
}GDCANVAS;

typedef class filer {
protected:
	FILE * f_Stream;
	UCHAR  OpenStream(LPSTR c_StreamName);
	UCHAR  CloseStream();

}GDFILER;

typedef class object : public GDFILER {
public:
	GDVEC3 p_Anchor;
	GDVEC3 * p_pPoint;
	GDCOLOR c_pColor;
	GDFACE * o_pFace;
	UINT32 i_Faces, i_Points;
private:
	UCHAR  ReadHeader();
	UCHAR  LoadFile();
	UCHAR  Prepare();
public:
	UCHAR  Read(LPSTR c_StreamName);
	UCHAR  Dispose();
}GFOBJECT;




typedef class vectormap : public GDFILER {
public:
	GDVEC2 p_Anchor;
	GDVEC2 * p_pPoint;
	GDCOLOR * c_pColor;
	GDLINE * l_pLines;
	UINT32 i_Connections, i_Points, i_Colors;
private:
	UCHAR  ReadHeader();
	UCHAR  LoadFile();
	UCHAR  Prepare();
public:
	UCHAR  Read(LPSTR c_StreamName);
	UCHAR  Dispose();
}GFVECTORMAP;

typedef class camera {
public:
	UINT32 i_Frustum[2];
	UINT32 i_Dimensions[2];
	UINT32 i_FOV;
	GDVEC3 i_Position, i_Rotation;

	UCHAR  Translate(GDVEC3 * p_pPoint, GDVEC2 * p_pResult);
}GDCAMERA;

typedef class codec2d {
public:
	codec2d(){}
	codec2d(GDCANVAS * gd_pCanvas){
		gd_Image = gd_pCanvas;
	}
	UCHAR  SetPixel(GDVEC2 * p_pPoint, GDCOLOR * c_pColor);
	UCHAR  DrawLine(GDVEC2 * p_pPointA, GDVEC2 * p_pPointB, GDCOLOR * c_pColor);
	UCHAR  DrawRect(GDVEC2 * p_pPointA, GDVEC2 * p_pPointB, GDCOLOR * c_pColor);
	UCHAR  DrawHLine(GDVEC2 * p_pPoint, UINT32  i_Length, GDCOLOR * c_pColor);
	UCHAR  DrawVLine(GDVEC2 * p_pPoint, UINT32  i_Length, GDCOLOR * c_pColor);
	UCHAR  DrawCanvas(DWORD * d_pBuffer, GDVEC2 * p_pPos, UINT32  i_Pixels[2]);
	UCHAR  DrawVMap(GFVECTORMAP * gd_VecMap);
protected:
	GDCANVAS * gd_Image;
}GD2DCODEC;

typedef class codec3d : public GD2DCODEC {
public:
	codec3d() {}
	codec3d(GDCANVAS * gd_pCanvas, GDCAMERA * gd_pCamera) {
		gd_Image = gd_pCanvas;
		gd_Camera = gd_pCamera;
	}
	UCHAR  DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor);
	UCHAR  DrawEdge(GDVEC3 * p_pVertexA, GDVEC3 * p_pVertexB, GDCOLOR * c_pColor);


private:
	GDCAMERA * gd_Camera;
}GD3DCODEC;


