#include "..\twogd.h"

WIN o_Win;

HANDLE  CreateExec(HINSTANCE h_Instance);
long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);


DWORD  wWinProcess(LPVOID lv_Void)
{
	DWORD* dw_pFrame = 0;
	while (TRUE)
	{
		GetCursorPos(&o_Win.v_CursorPos);
		ScreenToClient(o_Win.hd_WindowHandle, &o_Win.v_CursorPos);

		HWND hd_Active = GetForegroundWindow();
		o_Win.b_HasFocus = hd_Active == o_Win.hd_WindowHandle;

		dw_pFrame = gdUpdate(&o_Win);
		if (dw_pFrame == NULL) break;

		HBITMAP bmv_Bitmap = CreateBitmap(o_Win.i_Width, o_Win.i_Height, 1, 8 * 4, (const void*)dw_pFrame);
		HDC hdc_TempHdc = CreateCompatibleDC(o_Win.hdc_WindowHdc);
		SelectObject(hdc_TempHdc, bmv_Bitmap);
		BitBlt(o_Win.hdc_WindowHdc, 0, 0, o_Win.i_Width, o_Win.i_Height, hdc_TempHdc, 0, 0, SRCCOPY);
		DeleteObject(bmv_Bitmap);
		DeleteDC(hdc_TempHdc);
	}
	gdClose();

	return 0;
}

int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow)
{
	gdCreateWinExec(&o_Win);

	if (CreateExec(h_Instance) == NULL)
	{
		return NULL;
	}
	ShowWindow(o_Win.hd_WindowHandle, o_Win.i_CmdShow);
	UpdateWindow(o_Win.hd_WindowHandle);

	if (gdMain(&o_Win) == NULL)
	{
		return NULL;
	}

	o_Win.hdc_WindowHdc = GetDC(o_Win.hd_WindowHandle);

	HANDLE hwd_Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)wWinProcess, 0, 0, NULL);

	while ((GetMessage(&o_Win.msg_WindowMessage, NULL, 0, 0) > 0))
	{
		TranslateMessage(&o_Win.msg_WindowMessage);
		DispatchMessage(&o_Win.msg_WindowMessage);
	}

	CloseHandle(hwd_Thread);
	ReleaseDC(NULL, o_Win.hdc_WindowHdc);
	return NULL;
}

HANDLE  CreateExec(HINSTANCE h_Instance) {
	o_Win.w_WndClass.hbrBackground = (HBRUSH)(2);
	o_Win.w_WndClass = { 0 };
	o_Win.w_WndClass.lpfnWndProc = (WNDPROC)WindowProc;
	o_Win.w_WndClass.hInstance = h_Instance;
	o_Win.w_WndClass.lpszClassName = (LPCWSTR)o_Win.c_WinClassName;

	RegisterClass(&o_Win.w_WndClass);

	o_Win.h_Instance = h_Instance;
	o_Win.hd_WindowHandle = CreateWindowEx(
		o_Win.dw_ExStyle,
		(LPCWSTR)o_Win.c_WinClassName,
		(LPCWSTR)o_Win.c_WinTitle,
		o_Win.dw_Style,
		o_Win.i_XPos,
		o_Win.i_YPos,
		o_Win.i_Width,
		o_Win.i_Height,
		o_Win.hd_WndParent,
		o_Win.h_Menu,
		h_Instance,
		o_Win.lv_Param
	);
	return o_Win.hd_WindowHandle;
}

long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam)
{
	if (msg_Message == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT && o_Win.b_HideCursor)
	{
		SetCursor(NULL);
		return TRUE;
	}

	switch (msg_Message)
	{
	case WM_MOUSEWHEEL:
		if (o_Win.v_pMouseScroll != NULL)
		{
			POINT p_ScrollPoint;
			p_ScrollPoint.x = GET_X_LPARAM(lParam);
			p_ScrollPoint.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hd_Handle, &p_ScrollPoint);
			o_Win.v_pMouseScroll(HIWORD(wParam) == 120.0f, p_ScrollPoint);
		}
		return 0;

	case WM_LBUTTONDOWN:
		if (o_Win.v_pLeftMouseDown != NULL)
		{
			POINT p_ClickPoint;
			p_ClickPoint.x = GET_X_LPARAM(lParam);
			p_ClickPoint.y = GET_Y_LPARAM(lParam);
			o_Win.v_pLeftMouseDown(p_ClickPoint);
		}
		return 0;

	case WM_LBUTTONUP:
		if (o_Win.v_pLeftMouseUp != NULL)
		{
			POINT p_ClickPoint;
			p_ClickPoint.x = GET_X_LPARAM(lParam);
			p_ClickPoint.y = GET_Y_LPARAM(lParam);
			o_Win.v_pLeftMouseUp(p_ClickPoint);
		}
		return 0;

	case WM_RBUTTONDOWN:
		if (o_Win.v_pRightMouseDown != NULL)
		{
			POINT p_ClickPoint;
			p_ClickPoint.x = GET_X_LPARAM(lParam);
			p_ClickPoint.y = GET_Y_LPARAM(lParam);
			o_Win.v_pRightMouseDown(p_ClickPoint);
		}
		return 0;

	case WM_RBUTTONUP:
		if (o_Win.v_pRightMouseUp != NULL)
		{
			POINT p_ClickPoint;
			p_ClickPoint.x = GET_X_LPARAM(lParam);
			p_ClickPoint.y = GET_Y_LPARAM(lParam);
			o_Win.v_pRightMouseUp(p_ClickPoint);
		}
		return 0;

	case WM_CLOSE:
		DestroyWindow(hd_Handle);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (long)DefWindowProcW(hd_Handle, msg_Message, wParam, lParam);
}