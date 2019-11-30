
// TwoGD Socket for C++ 
// Markus Kotar @ UCPSystems.com 2019

#pragma once
#define _ALLOW_REGISTER_USE

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

#ifndef UNICODE
#define UNICODE
#endif 

#define _CRT_SECURE_NO_WARNINGS

#ifndef _ACIT_
#define _ACIT_
#endif

#include <iostream>
#include <xmmintrin.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>

#define TOCLIENTWIDTH(x) (win::i_Width-16)
#define TOCLIENTHEIGHT(x) (win::i_Height-39)

#define FROMCLIENTWIDTH(x) (x+16)
#define FROMCLIENTHEIGHT(x) (x+39)

extern HANDLE __stdcall CreateExec(HINSTANCE h_Instance);
extern long __stdcall WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int __stdcall wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

/*
	ENTRYPOINT MUST BE:
	-> wWinMainCRTStartup

	FUNCTIONS:
	unsigned char __fastcall win::main() { return 1; } //Gets Calledd once before Window Creation
	unsigned char __fastcall win::update() { return 1; } // Gets Called every "WindowProc" Call
	void __cdecl win::close() { } // Gets Called before window closing
*/

extern unsigned char __cdecl gdmain();
extern unsigned char __cdecl gdupdate();
extern void __cdecl gdclose();

#ifdef _ALLOW_REGISTER_USE
#define __REGISTER register
#else 
#define __REGISTER 
#endif

#define GD_ALLOC_FAILED 0x1F
#define GD_TASK_OKAY 0x1A
#define GD_FILE_FAILED 0x2F
#define GD_OUTOFBOUND 0x2F

#define _VALUE uchar
#define _STATUS uchar

typedef unsigned char uchar;
typedef unsigned int u_int;

#define _TOINDEX(x,y) ((int)y * this->i_Pixels[0] + (int)x)
#define _TOROW(y) ((int)y * this->i_Pixels[0])

#define _PTOP(POI) GDPOINT(((POINT)POI).x,((POINT)POI).y)

#define __WAY __stdcall

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
	FILE * __stdcall Create(const wchar_t* c_ConsoleTitle);
	FILE* __stdcall Create();

	void __stdcall Clear();
	BOOL __stdcall Destroy();
	BOOL __stdcall Rename(const wchar_t* c_ConsoleTitle);

	void __stdcall SetColor(const int i_HexColor);
private:
	FILE * __stdcall AllocC();
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

double Distance2(GDPOINT p_PosOne, GDPOINT p_PosTwo);

GDPOINT operator - (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2);
GDPOINT operator + (GDPOINT  &p_Pos1, GDPOINT  &p_Pos2);
GDPOINT operator * (GDPOINT  &p_Pos1, int &i_Lenght);

typedef class display {
public:
	u_int i_Pixels[2];
	DWORD * d_pOutputStream;
	u_int i_OutputSize;


	_STATUS __stdcall SetPixel(GDPOINT * p_pPoint, GDCOLOR * c_pColor);
	_STATUS __stdcall DrawLine(GDPOINT * p_pPointA, GDPOINT * p_pPointB, GDCOLOR * c_pColor);
	_STATUS __stdcall DrawRect(GDPOINT * p_pPointA, GDPOINT * p_pPointB, GDCOLOR * c_pColor);
	_STATUS __stdcall DrawHLine(GDPOINT * p_pPoint, u_int  i_Length, GDCOLOR * c_pColor);
	_STATUS __stdcall DrawVLine(GDPOINT * p_pPoint, u_int  i_Length, GDCOLOR * c_pColor);

	void CleanBuffer();

	_STATUS Prepare(int i_Width, int i_Height);
	_STATUS Dispose();

}GDDISPLAY;

// vmf / bmf
typedef class mapformat {
	
}GFMAPFORMAT;