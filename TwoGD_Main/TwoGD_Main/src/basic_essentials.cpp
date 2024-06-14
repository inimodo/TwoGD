#include"twogd.h"

void 
BasicCameraController(win::GDWIN *gd_win, GDCAMERA* gd_camera, float f_MoveSpeed, float f_ViewSpeed,BOOL b_MouseCtrl, float f_MouseSensitivity)
{

	GDVEC2 p_Middle = GDVEC2(gd_camera->i_Dimensions[0] / 2.0, gd_camera->i_Dimensions[1] / 2.0);
	if (gd_win->b_HasFocus && b_MouseCtrl)
	{
		gd_win->b_HideCursor = TRUE;

		POINT p_Point;
		p_Point.x = p_Middle.f_Pos[X];
		p_Point.y = p_Middle.f_Pos[Y];
		ClientToScreen(gd_win->hd_WindowHandle,&p_Point);
		SetCursorPos(p_Point.x, p_Point.y);

		GDVEC2 p_Delta = _PTOP(gd_win->p_CursorPos);
		float f_Ampl = 1.0f / p_Delta.Distance(p_Middle);
		p_Delta = p_Delta - p_Middle;

		gd_camera->i_Rotation.f_Pos[X] -= p_Delta.f_Pos[X]* f_MouseSensitivity * 0.001f;
		gd_camera->i_Rotation.f_Pos[Y] -= p_Delta.f_Pos[Y]* f_MouseSensitivity * 0.001f;


		GDVEC3 p_UnitVec = GDVEC3(0,0,0);

		if (GetAsyncKeyState('S') != 0) p_UnitVec.f_Pos[Z] += -f_MoveSpeed;
		if (GetAsyncKeyState('W') != 0) p_UnitVec.f_Pos[Z] += f_MoveSpeed;

		if (GetAsyncKeyState('D') != 0) p_UnitVec.f_Pos[X] += f_MoveSpeed;
		if (GetAsyncKeyState('A') != 0) p_UnitVec.f_Pos[X] += -f_MoveSpeed;

		p_UnitVec.RotateThis(gd_camera->i_Rotation);
		p_UnitVec.f_Pos[Y] = 0;
		gd_camera->i_Position = gd_camera->i_Position + p_UnitVec;
	
		if (GetAsyncKeyState(VK_SPACE) != 0)gd_camera->i_Position.f_Pos[1] += f_MoveSpeed;
		if (GetAsyncKeyState(VK_LSHIFT) != 0)gd_camera->i_Position.f_Pos[1] -= f_MoveSpeed;
	

		if (GetAsyncKeyState(VK_LEFT) != 0)gd_camera->i_Rotation.f_Pos[0] += f_ViewSpeed;
		if (GetAsyncKeyState(VK_RIGHT) != 0)gd_camera->i_Rotation.f_Pos[0] -= f_ViewSpeed;

		if (GetAsyncKeyState(VK_UP) != 0)gd_camera->i_Rotation.f_Pos[1] += f_ViewSpeed;
		if (GetAsyncKeyState(VK_DOWN) != 0)gd_camera->i_Rotation.f_Pos[1] -= f_ViewSpeed;

		if (GetAsyncKeyState(VK_ADD) != 0)gd_camera->f_FOV += 0.01;
		if (GetAsyncKeyState(VK_SUBTRACT) != 0)gd_camera->f_FOV -= 0.01;

		if (GetAsyncKeyState(VK_NUMPAD0) != 0) 
		{
			gd_camera->i_Rotation = p_UnitVec;
			gd_camera->i_Position = p_UnitVec;
			gd_camera->f_FOV = 2;
		}
	}
	else 
	{
		gd_win->b_HideCursor = FALSE;
	}

}

void 
DrawCrosshair(GD3DCODEC* o_pCodec, GDCOLOR c_Color, int i_Style, float f_size)
{
	GDVEC2 p_X = GDVEC2(o_pCodec->gd_Camera->i_Dimensions[0] / 2.0 - f_size, o_pCodec->gd_Camera->i_Dimensions[1] / 2.0);
	GDVEC2 p_Y = GDVEC2(o_pCodec->gd_Camera->i_Dimensions[0] / 2.0, o_pCodec->gd_Camera->i_Dimensions[1] / 2.0 - f_size);

	GDVEC3 p_Anchor = GDVEC3(0, 0, 5);
	GDVEC3 p_UnitX = GDVEC3(f_size, 0, 0);
	GDVEC3 p_UnitY = GDVEC3(0, f_size, 0);
	GDVEC3 p_UnitZ = GDVEC3(0, 0, f_size);

	switch (i_Style)
	{
	case CHST_BASIC:
		o_pCodec->DrawHLine(&p_X, f_size * 2, &c_Color);
		o_pCodec->DrawVLine(&p_Y, f_size * 2, &c_Color);

		break;
	case CHST_AXIS:

		p_Anchor.RotateThis(o_pCodec->gd_Camera->i_Rotation);

		p_Anchor = p_Anchor + o_pCodec->gd_Camera->i_Position;
		p_UnitX = p_UnitX + p_Anchor;
		p_UnitY = p_UnitY + p_Anchor;
		p_UnitZ = p_UnitZ + p_Anchor;


		o_pCodec->DrawEdge(&p_Anchor, &p_UnitX, &co_Red);
		o_pCodec->DrawEdge(&p_Anchor, &p_UnitY, &co_Green);
		o_pCodec->DrawEdge(&p_Anchor, &p_UnitZ, &co_Blue);
		break;

	}
 
}

/*
void
BasicCameraController(GDCAMERA* gd_camera, float f_MoveSpeed, float f_ViewSpeed)
{

GDVEC3 p_UnitVec = GDVEC3(0,0,0);
GDVEC3 p_Sel = GDVEC3(1.0f, 1.0f, 0.0f);

if (GetAsyncKeyState('S') != 0) p_UnitVec = GDVEC3(0, 0, -f_MoveSpeed);
if (GetAsyncKeyState('W') != 0) p_UnitVec = GDVEC3(0, 0, f_MoveSpeed);

if (GetAsyncKeyState('D') != 0) p_UnitVec = GDVEC3(f_MoveSpeed ,0 ,0);
if (GetAsyncKeyState('A') != 0) p_UnitVec = GDVEC3(-f_MoveSpeed ,0 ,0);

p_UnitVec.RotateThis(gd_camera->i_Rotation*p_Sel);
gd_camera->i_Position = gd_camera->i_Position + p_UnitVec;

if (GetAsyncKeyState(VK_SPACE) != 0)gd_camera->i_Position.f_Pos[1] += f_MoveSpeed;
if (GetAsyncKeyState(VK_LSHIFT) != 0)gd_camera->i_Position.f_Pos[1] -= f_MoveSpeed;


if (GetAsyncKeyState(VK_LEFT) != 0)gd_camera->i_Rotation.f_Pos[0] += f_ViewSpeed;
if (GetAsyncKeyState(VK_RIGHT) != 0)gd_camera->i_Rotation.f_Pos[0] -= f_ViewSpeed;

if (GetAsyncKeyState(VK_UP) != 0)gd_camera->i_Rotation.f_Pos[1] += f_ViewSpeed;
if (GetAsyncKeyState(VK_DOWN) != 0)gd_camera->i_Rotation.f_Pos[1] -= f_ViewSpeed;

if (GetAsyncKeyState(VK_ADD) != 0)gd_camera->f_FOV += 0.01;
if (GetAsyncKeyState(VK_SUBTRACT) != 0)gd_camera->f_FOV -= 0.01;

if (GetAsyncKeyState(VK_NUMPAD0) != 0)
{
gd_camera->i_Rotation = p_UnitVec;
gd_camera->i_Position = p_UnitVec;
gd_camera->f_FOV = 2;
}
}
*/