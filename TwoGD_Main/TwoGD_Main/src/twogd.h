// TwoGD Win32Socket for C++ 
// Markus Kotar 2024

#pragma once

#define _ALLOW_REGISTER_USE
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

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
#define DEGTORAD(X) (float)(X*2*M_PI/360.0)
#define RADTODEG(X) (float)(X/(2*M_PI)*360.0)

typedef unsigned char UCHAR;
typedef unsigned int UINT32;

#define VEC2D TRUE
#ifdef VEC2D

	typedef class gd_vec2 {
	public:
		float f_Pos[2];

		void Delta(gd_vec2 p_Pos);
		double Distance(gd_vec2 p_Pos);

		gd_vec2();
		gd_vec2(float f_X, float f_Y);
		gd_vec2(float f_Pos[2]);

	}GDVEC2;

	GDVEC2 operator - (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2);
	GDVEC2 operator + (GDVEC2  &p_Pos1, GDVEC2  &p_Pos2);
	GDVEC2 operator * (GDVEC2  &p_Pos1, int &i_Lenght);

#endif // VEC2D

#define VEC3D TRUE
#ifdef VEC3D

	typedef class gd_vec3 {
	public:
		float f_Pos[3];

		void NormalizeThis();
		gd_vec3 NormalizeTo();
	
		void RotateThis(gd_vec3 p_Rot);
		gd_vec3 RotateTo(gd_vec3 p_Rot);
		void RotateAroundThis(gd_vec3 p_UnitV, float f_phi);
		gd_vec3 RotateAroundTo(gd_vec3 p_UnitV, float f_phi);

		void DeltaThis(gd_vec3 p_Pos);
		gd_vec3 DeltaTo(gd_vec3 p_Pos);

		gd_vec3 AngleTo(gd_vec3 p_Pos);
		gd_vec3 Angle();

		double DistanceTo(gd_vec3 p_Pos);
		float Length();
		float DotProduct(gd_vec3 p_Pos);

		gd_vec3();
		gd_vec3(float f_X, float f_Y, float f_Z);
		gd_vec3(float f_Pos[3]);



	}GDVEC3;

	GDVEC3 operator-(GDVEC3  &p_Pos1, GDVEC3  &p_Pos2);
	GDVEC3 operator+(GDVEC3  &p_Pos1, GDVEC3  &p_Pos2);
	GDVEC3 operator*(GDVEC3  &p_Pos1, GDVEC3  &p_Pos2);
	GDVEC3 operator*(GDVEC3  &p_Pos, float const&f_Lenght);
	GDVEC3 operator*(GDVEC3  &p_Pos, float &f_Lenght);

#endif // VEC3D


#define TYPES_OTHERS TRUE
#ifdef TYPES_OTHERS

	typedef class canvas {
	public:
		UINT32 i_Pixels[2];
		DWORD * d_pOutputStream;
		UINT32 i_OutputSize;

		UCHAR  CleanBuffer();

		UCHAR  Prepare(int i_Width, int i_Height);
		UCHAR  Dispose();
	}GDCANVAS;

	typedef struct gd_color {
		UCHAR c_Color[3];
		gd_color(UCHAR c_R, UCHAR c_G, UCHAR c_B);
		gd_color();
		DWORD GetAsHex();
	} GDCOLOR;

#endif // TYPES_OTHERS

typedef struct gd_line {
	GDVEC2 p_Point[2];
	GDCOLOR c_Color;
}GDLINE;

typedef struct gd_face {
	GDVEC3 p_Point[3];
}GDFACE;

#define FILE_HANDLER TRUE
#ifdef FILE_HANDLER

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

#endif // FILE_HANDLER

#define CODEC2D TRUE
#ifdef CODEC2D

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

#endif // CODEC2D


#define CODEC3D TRUE
#ifdef CODEC3D

	typedef class camera {
	public:
		FLOAT f_Frustum[2];
		UINT32 i_Dimensions[2];
		FLOAT f_FOV;
		GDVEC3 i_Position, i_Rotation;

		UCHAR  Translate(GDVEC3 * p_pPoint, GDVEC2 * p_pResult);
		UCHAR  Relate(GDVEC2 * p_pScreenPos, GDVEC3 * p_pAngle);

	}GDCAMERA;


	typedef class codec3d : public GD2DCODEC {
	public:
		codec3d() {}
		codec3d(GDCANVAS * gd_pCanvas, GDCAMERA * gd_pCamera) {
			gd_Image = gd_pCanvas;
			gd_Camera = gd_pCamera;
		}
		UCHAR  DrawObject(GFOBJECT * gd_Object, GDCOLOR * c_pColor);
		UCHAR  DrawEdge(GDVEC3 * p_pVertexA, GDVEC3 * p_pVertexB, GDCOLOR * c_pColor);

		GDCAMERA * gd_Camera;
	}GD3DCODEC;

#endif


#define CONSOLE_HANDLER TRUE
#ifdef CONSOLE_HANDLER

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

#endif // CONSOLE_HANDLER

#define BASIC_ESSENTIALS TRUE
#ifdef BASIC_ESSENTIALS

	extern void BasicCameraController(GDCAMERA* gd_camera, float f_MoveSpeed, float f_ViewSpeed);
	extern void DrawCrosshair(GD3DCODEC* o_pCodec, GDCOLOR c_Color, int i_size);

#endif // BASIC_ESSENTIALS

#define MATRIX_OP
#ifdef MATRIX_OP

	typedef struct gd_m3x3 {
	public:
		gd_m3x3();
		gd_m3x3(GDVEC3 p_v1, GDVEC3 p_v2, GDVEC3 p_v3);
		gd_m3x3(float f_UnitValue);

		GDVEC3 p_v1, p_v2, p_v3;

		void print();
	} GDM3X3;

	GDM3X3 M3X3RotX(float f_phi);
	GDM3X3 M3X3RotY(float f_phi);
	GDM3X3 M3X3RotZ(float f_phi);
	GDM3X3 M3X3RotU(GDVEC3 p_UnitV,float f_phi);

	GDM3X3 operator * (GDM3X3  &m_M, float &f_S);
	GDM3X3 operator * (GDM3X3  &m_M1, GDM3X3 &m_M2);
	GDM3X3 operator + (GDM3X3  &m_M1, GDM3X3 &m_M2);
	GDVEC3 operator * (GDM3X3  &m_M, GDVEC3 &p_V);

	GDM3X3 operator * (GDM3X3  const&m_M, float const&f_S);
	GDM3X3 operator * (GDM3X3  const&m_M1, GDM3X3 const&m_M2);
	GDM3X3 operator + (GDM3X3  const&m_M1, GDM3X3 const&m_M2);
	GDVEC3 operator * (GDM3X3  const&m_M, GDVEC3 const&p_V);

#endif // MATRIX_OP


double Distance2(GDVEC2 p_PosOne, GDVEC2 p_PosTwo);
double Distance3(GDVEC3 p_PosOne, GDVEC3 p_PosTwo);


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
		GDVEC2 p_CursorPos;
		void(*v_pMouseDown)();
	}GDWIN;

}

extern unsigned char  gdMain(win::GDWIN *);
extern DWORD*  gdUpdate(win::GDWIN *);
extern void  gdClose();