#include "..\twogd.h"

o_camctrlr::o_camctrlr() {}
o_camctrlr::o_camctrlr(CAM3D* o_camera_, CODEC3D* o_pCodec_) 
{
	this->o_camera = o_camera_;
	this->o_pCodec = o_pCodec_;
}

void o_camctrlr::UpdateCamCtrlr(win::GDWIN * o_win)
{
	V2 v_Middle = V2(o_camera->i_Dimensions[0] / 2.0f, o_camera->i_Dimensions[1] / 2.0f);
	if (o_win->b_HasFocus && b_MouseCtrl)
	{
		o_win->b_HideCursor = TRUE;

		POINT v_Point;
		v_Point.x = (LONG)v_Middle.f_Pos[X];
		v_Point.y = (LONG)v_Middle.f_Pos[Y];
		ClientToScreen(o_win->hd_WindowHandle, &v_Point);
		SetCursorPos(v_Point.x, v_Point.y);

		V2 v_Delta = _PTOP(o_win->v_CursorPos);
		float f_Ampl = 1.0f / (float)v_Delta.Distance(v_Middle);
		v_Delta = v_Delta - v_Middle;

		o_camera->i_Rotation.f_Pos[X] -= v_Delta.f_Pos[X] * f_MouseSensitivity * 0.001f;
		o_camera->i_Rotation.f_Pos[Y] -= v_Delta.f_Pos[Y] * f_MouseSensitivity * 0.001f;

		V3 v_UnitVec = V3(0, 0, 0);

		if (GetAsyncKeyState('S') != 0) v_UnitVec.f_Pos[Z] += -f_MoveSpeed;
		if (GetAsyncKeyState('W') != 0) v_UnitVec.f_Pos[Z] += f_MoveSpeed;

		if (GetAsyncKeyState('D') != 0) v_UnitVec.f_Pos[X] += f_MoveSpeed;
		if (GetAsyncKeyState('A') != 0) v_UnitVec.f_Pos[X] += -f_MoveSpeed;

		v_UnitVec.RotateThis(o_camera->i_Rotation);
		v_UnitVec.f_Pos[Y] = 0;
		o_camera->i_Position = o_camera->i_Position + v_UnitVec;

		if (GetAsyncKeyState(VK_SPACE) != 0)o_camera->i_Position.f_Pos[1] += f_MoveSpeed;
		if (GetAsyncKeyState(VK_LSHIFT) != 0)o_camera->i_Position.f_Pos[1] -= f_MoveSpeed;


		if (GetAsyncKeyState(VK_LEFT) != 0)o_camera->i_Rotation.f_Pos[0] += f_ViewSpeed;
		if (GetAsyncKeyState(VK_RIGHT) != 0)o_camera->i_Rotation.f_Pos[0] -= f_ViewSpeed;

		if (GetAsyncKeyState(VK_UP) != 0)o_camera->i_Rotation.f_Pos[1] += f_ViewSpeed;
		if (GetAsyncKeyState(VK_DOWN) != 0)o_camera->i_Rotation.f_Pos[1] -= f_ViewSpeed;

		if (GetAsyncKeyState(VK_ADD) != 0)o_camera->f_FOV += 0.01f;
		if (GetAsyncKeyState(VK_SUBTRACT) != 0)o_camera->f_FOV -= 0.01f;

		if (GetAsyncKeyState(VK_NUMPAD0) != 0)
		{
			o_camera->i_Rotation = v_UnitVec;
			o_camera->i_Position = v_UnitVec;
			o_camera->f_FOV = 2;
		}
	}
	else
	{
		o_win->b_HideCursor = FALSE;
	}
}

void o_camctrlr::DrawCrosshair()
{
	V2 v_X = V2(o_pCodec->o_Camera->i_Dimensions[0] / 2.0f - f_CHSize, o_pCodec->o_Camera->i_Dimensions[1] / 2.0f);
	V2 v_Y = V2(o_pCodec->o_Camera->i_Dimensions[0] / 2.0f, o_pCodec->o_Camera->i_Dimensions[1] / 2.0f - f_CHSize);

	V3 v_Anchor = V3(0, 0, 5);
	V3 v_UnitX = V3(f_CHSize, 0, 0);
	V3 v_UnitY = V3(0, f_CHSize, 0);
	V3 v_UnitZ = V3(0, 0, f_CHSize);

	switch (i_CHStyle)
	{
	case CHST_BASIC:
		o_pCodec->DrawHLine(&v_X, (UINT32)(f_CHSize * 2.0f), &c_CHColor, PF_OVERWRITE_FORBIDDEN);
		o_pCodec->DrawVLine(&v_Y, (UINT32)(f_CHSize * 2.0f), &c_CHColor, PF_OVERWRITE_FORBIDDEN);

		break;
	case CHST_AXIS:

		v_Anchor.RotateThis(o_pCodec->o_Camera->i_Rotation);

		v_Anchor = v_Anchor + o_pCodec->o_Camera->i_Position;
		v_UnitX = v_UnitX + v_Anchor;
		v_UnitY = v_UnitY + v_Anchor;
		v_UnitZ = v_UnitZ + v_Anchor;

		o_pCodec->DrawEdge(&v_Anchor, &v_UnitX, (COLOR*)&co_Red,PF_OVERWRITE_FORBIDDEN);
		o_pCodec->DrawEdge(&v_Anchor, &v_UnitY, (COLOR*)&co_Green, PF_OVERWRITE_FORBIDDEN);
		o_pCodec->DrawEdge(&v_Anchor, &v_UnitZ, (COLOR*)&co_Blue, PF_OVERWRITE_FORBIDDEN);
		break;
	}
}
