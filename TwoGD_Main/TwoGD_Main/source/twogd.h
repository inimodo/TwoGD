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

// TwoGD Socket for C++ 
// Markus Kotar @ UCPSystems.com 2019/2020

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


extern HANDLE __WAY CreateExec(HINSTANCE h_Instance);
extern long __WAY WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int __WAY wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

/*
	ENTRYPOINT MUST BE:
	-> wWinMainCRTStartup

	FUNCTIONS:
	unsigned char __WAY win::main() { return 1; } //Gets Calledd once before Window Creation
	unsigned char __WAY win::update() { return 1; } // Gets Called every "WindowProc" Call
	void __WAY win::close() { } // Gets Called before window closing
*/

extern unsigned char __WAY gdmain();
extern unsigned char __WAY gdupdate();
extern void __WAY gdclose();

typedef unsigned char uchar;
typedef unsigned int u_int;
#define __STATUS uchar

#define GD_ALLOC_FAILED 0x1F
#define GD_TASK_OKAY 0x1A
#define GD_FILE_FAILED 0x2F
#define GD_OUTOFBOUND 0x2F

#define _TOCLIENTWIDTH(x) (win::i_Width-16)
#define _TOCLIENTHEIGHT(x) (win::i_Height-39)
#define _FROMCLIENTWIDTH(x) (x+16)
#define _FROMCLIENTHEIGHT(x) (x+39)
#define _TOINDEX(x,y) ((int)y * this->i_Pixels[0] + (int)x)
#define _TOROW(y) ((int)y * this->i_Pixels[0])
#define _PTOP(POI) GDPOINT(((POINT)POI).x,((POINT)POI).y)


extern void  SetScreenBuffer(DWORD* dw_ColorStream, int i_Width, int i_Height);
extern void  GetCursorPosition();

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

	extern int i_Width;
	extern int i_Height;

	extern HINSTANCE h_Instance;
	extern HWND hd_WindowHandle;
	extern MSG msg_WindowMessage;
	extern HDC hdc_WindowHdc;
	extern WNDCLASS w_WndClass;
	extern POINT p_CursorPos;
	extern void(*v_pMouseDown)();
}

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
	uchar c_Color[3];
	gd_color(uchar c_R, uchar c_G, uchar c_B);
	gd_color();
	DWORD GetAsHex();
} GDCOLOR;

typedef class gd_point {
public:
	float f_Pos[2];

	void Delta(gd_point p_Pos);
	double Distance(gd_point p_Pos);

	gd_point();
	gd_point(float f_X, float f_Y);
	gd_point(float f_Pos[2]);

}GDPOINT;

typedef struct gd_line {
	GDPOINT p_Point[2];
	GDCOLOR c_Color;
}GDLINE;



double Distance2(GDPOINT p_PosOne, GDPOINT p_PosTwo);

GDPOINT operator - (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2);
GDPOINT operator + (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2);
GDPOINT operator * (GDPOINT  &p_Pos1, int &i_Lenght);

typedef class canvas {
public:
	u_int i_Pixels[2];
	DWORD * d_pOutputStream;
	u_int i_OutputSize;

	__STATUS __WAY CleanBuffer();

	__STATUS __WAY Prepare(int i_Width, int i_Height);
	__STATUS __WAY Dispose();
}GDCANVAS;


typedef class codec : public GDCANVAS {
public:
	__STATUS __WAY SetPixel(GDPOINT * p_pPoint, GDCOLOR * c_pColor);
	__STATUS __WAY DrawLine(GDPOINT * p_pPointA, GDPOINT * p_pPointB, GDCOLOR * c_pColor);
	__STATUS __WAY DrawRect(GDPOINT * p_pPointA, GDPOINT * p_pPointB, GDCOLOR * c_pColor);
	__STATUS __WAY DrawHLine(GDPOINT * p_pPoint, u_int  i_Length, GDCOLOR * c_pColor);
	__STATUS __WAY DrawVLine(GDPOINT * p_pPoint, u_int  i_Length, GDCOLOR * c_pColor);
	__STATUS __WAY DrawBitmap(DWORD * d_pBuffer, GDPOINT * p_pPos, u_int  i_Pixels[2]);
}GDCODEC;

// vmf 
typedef class mapformat {
protected:
	FILE * f_Stream;
	__STATUS __WAY OpenStream(const char* c_StreamName);
	__STATUS __WAY CloseStream();

}GFMAPFORMAT;

typedef class vectormap : public GFMAPFORMAT {
public:
	GDPOINT p_Anchor;
	GDPOINT * p_pPoint;
	GDCOLOR * c_pColor;
	GDLINE * l_pLines;
	u_int i_Connections, i_Points,i_Colors;
private:
	__STATUS __WAY ReadHeader();
	__STATUS __WAY LoadFile();
public:
	__STATUS __WAY Read(const char* c_StreamName);

	__STATUS __WAY Prepare();
	__STATUS __WAY Dispose();
}GFVECTORMAP;

typedef class bitmap : public GDCODEC {


}GFBITMAP;