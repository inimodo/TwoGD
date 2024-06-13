#include"twogd.h"

void 
BasicCameraController(GDCAMERA* gd_camera, float f_MoveSpeed, float f_ViewSpeed) 
{

	GDVEC3 p_UnitVec = GDVEC3(0,0,0);
	GDVEC3 p_Sel = GDVEC3(-1.0f, 1.0f, 0.0f);

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
	}
}

void 
DrawCrosshair(GD3DCODEC* o_pCodec, GDCOLOR c_Color, int i_size) 
{
	GDVEC2 p_X = GDVEC2(o_pCodec->gd_Camera->i_Dimensions[0]/2.0-i_size, o_pCodec->gd_Camera->i_Dimensions[1] / 2.0);
	GDVEC2 p_Y = GDVEC2(o_pCodec->gd_Camera->i_Dimensions[0]/2.0, o_pCodec->gd_Camera->i_Dimensions[1] / 2.0 - i_size);
	o_pCodec->DrawHLine(&p_X,i_size*2,&c_Color);
	o_pCodec->DrawVLine(&p_Y,i_size*2,&c_Color);
}