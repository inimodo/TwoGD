#pragma once

#define _ALLOW_REGISTER_USE
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <xmmintrin.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

// TwoGD Win32Socket for C++ 
// Markus Kotar @ www.ucpsystems.com 2019/2020


/*
ENTRYPOINT
-> wWinMainCRTStartup

FUNCTIONS
unsigned char __WAY gdmain(win::GDWIN *) { return TRUE; } //Called once before Window Creation
unsigned char __WAY gdupdate(win::GDWIN *) { return 1; } // Called every "WindowProc" Call
void __WAY gdclose() { } // Gets Called before window closing
*/


#ifndef UNICODE
#define UNICODE
#endif 


#ifndef _ACIT_
#define _ACIT_
#endif

#define __WAY __stdcall

#ifdef _ALLOW_REGISTER_USE
#define __REGISTER register
#else 
#define __REGISTER 
#endif


typedef unsigned char UCHAR;
typedef unsigned int UINT32;
#define __STATUS UCHAR

#define GD_ALLOC_FAILED 0x1F
#define GD_TASK_OKAY 0x1A
#define GD_FILE_FAILED 0x2F
#define GD_OUTOFBOUND 0x2F

#define _TOCLIENTWIDTH(x) (win::i_Width-16)
#define _TOCLIENTHEIGHT(x) (win::i_Height-39)
#define _FROMCLIENTWIDTH(x) (x+16)
#define _FROMCLIENTHEIGHT(x) (x+39)

#define _PTOP(POI) GDVEC2(((POINT)POI).x,((POINT)POI).y)

extern HANDLE __WAY CreateExec(HINSTANCE h_Instance);
extern long __WAY WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int __WAY wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

namespace win {
	const static DWORD dw_ExStyle = 0;
	const static wchar_t* c_WinClassName = L"WINCLASSEWS";
	const static wchar_t* c_WinTitle = L"Test";
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

extern unsigned char __WAY gdmain(win::GDWIN *);
extern unsigned char __WAY gdupdate(win::GDWIN *);
extern void __WAY gdclose();

typedef class gd_console {
public:
	FILE * __WAY Create(const wchar_t* c_ConsoleTitle);
	FILE* __WAY Create();

	void __WAY Clear();
	BOOL __WAY Destroy();
	BOOL __WAY Rename(const wchar_t* c_ConsoleTitle);

	void __WAY SetColor(const int i_HexColor);
private:
	FILE * __WAY AllocC();
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
GDVEC3 operator * (GDVEC3  &p_Pos1, int &i_Lenght);


extern void  SetScreenBuffer(DWORD* dw_ColorStream, int i_Width, int i_Height);
extern void  GetCursorPosition();

typedef class canvas {
public:
	UINT32 i_Pixels[2];
	DWORD * d_pOutputStream;
	UINT32 i_OutputSize;

	__STATUS __WAY CleanBuffer();

	__STATUS __WAY Prepare(int i_Width, int i_Height);
	__STATUS __WAY Dispose();
}GDCANVAS;

typedef class filer {
protected:
	FILE * f_Stream;
	__STATUS __WAY OpenStream(LPSTR c_StreamName);
	__STATUS __WAY CloseStream();

}GDFILER;

typedef class object : public GDFILER {
public:
	GDVEC3 p_Anchor;
	GDVEC3 * p_pPoint;
	GDCOLOR c_pColor;
	GDFACE * o_pFace;
	UINT32 i_Faces, i_Points;
private:
	__STATUS __WAY ReadHeader();
	__STATUS __WAY LoadFile();
	__STATUS __WAY Prepare();
public:
	__STATUS __WAY Read(LPSTR c_StreamName);
	__STATUS __WAY Dispose();
}GFOBJECT;




typedef class vectormap : public GDFILER {
public:
	GDVEC2 p_Anchor;
	GDVEC2 * p_pPoint;
	GDCOLOR * c_pColor;
	GDLINE * l_pLines;
	UINT32 i_Connections, i_Points, i_Colors;
private:
	__STATUS __WAY ReadHeader();
	__STATUS __WAY LoadFile();
	__STATUS __WAY Prepare();
public:
	__STATUS __WAY Read(LPSTR c_StreamName);
	__STATUS __WAY Dispose();
}GFVECTORMAP;

typedef class camera {
public:
	UINT32 i_Frustum[2];
	UINT32 i_Dimensions[2];
	UINT32 i_FOV;
	GDVEC3 i_Position, i_Rotation;

	__STATUS __WAY Translate(GDVEC3 * p_pPoint, GDVEC2 * p_pResult);
}GDCAMERA;

typedef class codec2d {
public:
	codec2d(){}
	codec2d(GDCANVAS * gd_pCanvas){
		gd_Image = gd_pCanvas;
	}
	__STATUS __WAY SetPixel(GDVEC2 * p_pPoint, GDCOLOR * c_pColor);
	__STATUS __WAY DrawLine(GDVEC2 * p_pPointA, GDVEC2 * p_pPointB, GDCOLOR * c_pColor);
	__STATUS __WAY DrawRect(GDVEC2 * p_pPointA, GDVEC2 * p_pPointB, GDCOLOR * c_pColor);
	__STATUS __WAY DrawHLine(GDVEC2 * p_pPoint, UINT32  i_Length, GDCOLOR * c_pColor);
	__STATUS __WAY DrawVLine(GDVEC2 * p_pPoint, UINT32  i_Length, GDCOLOR * c_pColor);
	__STATUS __WAY DrawCanvas(DWORD * d_pBuffer, GDVEC2 * p_pPos, UINT32  i_Pixels[2]);
	__STATUS __WAY DrawVMap(GFVECTORMAP * gd_VecMap);
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
	__STATUS __WAY SetVoxel(GDVEC3 * p_pPoint, GDCOLOR * c_pColor);
	__STATUS __WAY DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor);


private:
	GDCAMERA * gd_Camera;
}GD3DCODEC;


