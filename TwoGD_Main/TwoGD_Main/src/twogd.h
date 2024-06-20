// TwoGD Win32Socket for C++ 
// Markus Kotar 2024

#pragma once

#define _ALLOW_REGISTER_USE
//#define _ALLOW_PIXEL_OVERWRITE

#ifndef UNICODE
#define UNICODE
#endif 

#ifdef _ALLOW_REGISTER_USE
#define __REGISTER register
#else 
#define __REGISTER 
#endif

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
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
#define GD_OUTOFBOUND 0x2E

#define _TOCLIENTWIDTH(x) (win::i_Width-16)
#define _TOCLIENTHEIGHT(x) (win::i_Height-39)
#define _FROMCLIENTWIDTH(x) (x+16)
#define _FROMCLIENTHEIGHT(x) (x+39)
#define _TOINDEX(x,y) ((int)y * o_Image->i_Pixels[0] + (int)x)
#define _TOROW(y) ((int)y * o_Image->i_Pixels[0])
#define _PTOP(POI) V2((float)((POINT)POI).x,(float)((POINT)POI).y)
#define DEGTORAD(X) (float)(X*2*M_PI/360.0)
#define RADTODEG(X) (float)(X/(2*M_PI)*360.0)


#ifdef X 
#undef X
#endif   

#ifdef Y
#undef Y
#endif 

#ifdef Z 
#undef Z
#endif 

#define X 0
#define Y 1
#define Z 2

namespace win {
	const static DWORD dw_ExStyle = 0;
	const static wchar_t* c_WinClassName = L"WINCLASSEWS";
	const static wchar_t* c_WinTitle = L"";
	const static DWORD  dw_Style = (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX);
	const static int i_XPos = CW_USEDEFAULT;
	const static int i_YPos = CW_USEDEFAULT;
	const static HWND hd_WndParent = NULL;
	const static HMENU h_Menu = NULL;
	const static  LPVOID lv_Param = NULL;

	typedef struct o_win {
		UINT32 i_Width, i_Height;
		HINSTANCE h_Instance;
		HWND hd_WindowHandle;
		MSG msg_WindowMessage;
		HDC hdc_WindowHdc;
		WNDCLASS w_WndClass;
		POINT v_CursorPos;
		void(*v_pMouseDown)();
		BOOL b_HasFocus;
		BOOL b_HideCursor = FALSE;
	}GDWIN;

}

extern HANDLE  CreateExec(HINSTANCE h_Instance);
extern long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);
extern int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow);

extern unsigned char  gdMain(win::GDWIN *);
extern DWORD*  gdUpdate(win::GDWIN *);
extern void  gdClose();

typedef unsigned char UCHAR;
typedef unsigned int UINT32;


#define VEC2D TRUE
#ifdef VEC2D

	typedef class o_vec2 {
	public:
		float f_Pos[Z];

		void Delta(o_vec2 v_Pos);
		double Distance(o_vec2 v_Pos);

		o_vec2();
		o_vec2(float f_X, float f_Y);
		o_vec2(float f_Pos[2]);

	}V2;

	V2 operator - (V2  &v_Pos1, V2  &v_Pos2);
	V2 operator + (V2  &v_Pos1, V2  &v_Pos2);
	V2 operator * (V2  &v_Pos1, int &i_Lenght);
	V2 operator * (V2  &v_Pos1, float &f_Lenght);

#endif // VEC2D

#define VEC3D TRUE
#ifdef VEC3D

	typedef class o_vec3 {
	public:
		float f_Pos[3];

		void NormalizeThis();
		o_vec3 NormalizeTo();
	
		void RotateThis(o_vec3 v_Rot);
		o_vec3 RotateTo(o_vec3 v_Rot);
		void CamRotateThis(o_vec3 v_Rot);
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

#endif // VEC3D


#define DRAWING TRUE
#ifdef DRAWING

	#define PF_OVERWRITE_ALLOWED   0xA
	#define PF_OVERWRITE_FORBIDDEN 0xF

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


#endif // DRAWING

typedef struct o_line {
	V2 v_Point[2];
	COLOR c_Color;
}LINE;

typedef struct o_face {
	V3 v_Point[3];
}FACE;

#define FILE_HANDLER TRUE
#ifdef FILE_HANDLER

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
		V2 v_Anchor;
		V2 * v_pPoint;
		COLOR * c_pColor;
		LINE * l_pLines;
		UINT32 i_Connections, i_Points, i_Colors;

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

#endif // FILE_HANDLER

#define CODEC_2D TRUE
#ifdef CODEC_2D

	typedef class codec2d {
	public:
		codec2d() {}
		codec2d(CANVAS * o_pCanvas){
			o_Image = o_pCanvas;
		}
		UCHAR  SetPixel(V2 * v_pPoint, COLOR * c_pColor,UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawLine(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawRect(V2 * v_pPointA, V2 * v_pPointB, COLOR * c_pColor, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawHLine(V2 * v_pPoint, UINT32  i_Length, COLOR * c_pColor, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawVLine(V2 * v_pPoint, UINT32  i_Length, COLOR * c_pColor, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawCanvas(DWORD * d_pBuffer, V2 * v_pPos, UINT32  i_Pixels[2], UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);
		UCHAR  DrawVMap(VMAP * o_VecMap, UCHAR i_PixelFlag = PF_OVERWRITE_ALLOWED, UCHAR i_PrioFlag = 0);

		BOOL b_AllowPixelOverwrite = TRUE;
	protected:
		CANVAS * o_Image;

	}CODEC2D;

#endif // CODEC_2D

#define CODEC_3D TRUE
#ifdef CODEC_3D

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

#endif


#define CONSOLE_HANDLER TRUE
#ifdef CONSOLE_HANDLER

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

#endif // CONSOLE_HANDLER

#define CAM_CTRLR TRUE
#ifdef CAM_CTRLR

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


		void UpdateCamCtrlr(win::GDWIN *o_win);
		void DrawCrosshair();
	}CAMCTRLR;



#endif // CAM_CTRLR

#define MATRIX_OP
#ifdef MATRIX_OP

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

#endif // MATRIX_OP

#define WORLD_CTRLR
#ifdef WORLD_CTRLR

	typedef class o_world {
		UINT32 i_Length;
	public:
		UINT32 Length() const { return i_Length; }
		LAYER * o_Layers = NULL;
		CODEC3D * o_Codec = NULL;

		UINT32 AppendLayer(LPSTR c_StreamName, COLOR c_Color);
		UCHAR RayTrace(CAM3D * o_pCam, FLOAT f_CutoffAngleInRad, UINT32 * i_pLayer);
		UCHAR Render();
		UCHAR  Dispose();

		o_world();
		o_world(CODEC3D * o_Codec);
	}WORLD;

#endif // WOLRD_CTRLR


double Distance2(V2 v_PosOne, V2 v_PosTwo);
double Distance3(V3 v_PosOne, V3 v_PosTwo);
