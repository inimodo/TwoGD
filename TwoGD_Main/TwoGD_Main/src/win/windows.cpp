#include "..\twogd.h"

win::GDWIN o_win;

HANDLE  CreateExec(HINSTANCE h_Instance);
long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);


DWORD  wWinProcess(LPVOID lv_Void)
{
	DWORD* dw_pFrame = 0;
	while(TRUE)
	{
		GetCursorPos(&o_win.v_CursorPos);
		ScreenToClient(o_win.hd_WindowHandle, &o_win.v_CursorPos);

		HWND hd_Active = GetForegroundWindow();
		o_win.b_HasFocus = hd_Active == o_win.hd_WindowHandle;

		dw_pFrame = gdUpdate(&o_win);
		if (dw_pFrame == NULL) break;

		HBITMAP bmv_Bitmap = CreateBitmap(o_win.i_Width, o_win.i_Height, 1, 8 * 4, (const void *)dw_pFrame);
		HDC hdc_TempHdc = CreateCompatibleDC(o_win.hdc_WindowHdc);
		SelectObject(hdc_TempHdc, bmv_Bitmap);
		BitBlt(o_win.hdc_WindowHdc, 0, 0, o_win.i_Width, o_win.i_Height, hdc_TempHdc, 0, 0, SRCCOPY);
		DeleteObject(bmv_Bitmap);
		DeleteDC(hdc_TempHdc);
	}
	gdClose();

	return 0; 
}
int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow)
{
	if (gdMain(&o_win) == NULL)
	{
		return NULL;
	}
	if (CreateExec(h_Instance) == NULL)
	{
		return NULL;
	}
	ShowWindow(o_win.hd_WindowHandle, i_CmdShow);
	UpdateWindow(o_win.hd_WindowHandle);

	o_win.hdc_WindowHdc = GetDC(o_win.hd_WindowHandle);

	HANDLE hwd_Thread = CreateThread(0,0, (LPTHREAD_START_ROUTINE)wWinProcess,0,0,NULL);

	while ((GetMessage(&o_win.msg_WindowMessage, NULL, 0, 0) > 0))
	{
		TranslateMessage(&o_win.msg_WindowMessage);
		DispatchMessage(&o_win.msg_WindowMessage);
	}

	printf("%d",CloseHandle(hwd_Thread));

	ReleaseDC(NULL, o_win.hdc_WindowHdc);
	return NULL;
}
HANDLE  CreateExec(HINSTANCE h_Instance) {
	o_win.w_WndClass.hbrBackground = (HBRUSH)(2);
	o_win.w_WndClass = { 0 };
	o_win.w_WndClass.lpfnWndProc = (WNDPROC)WindowProc;
	o_win.w_WndClass.hInstance = h_Instance;
	o_win.w_WndClass.lpszClassName = (LPCWSTR)win::c_WinClassName;

	RegisterClass(&o_win.w_WndClass);

	o_win.h_Instance = h_Instance;
	o_win.hd_WindowHandle = CreateWindowEx(
		win::dw_ExStyle,
		(LPCWSTR)win::c_WinClassName,
		(LPCWSTR)win::c_WinTitle,
		win::dw_Style,
		win::i_XPos,
		win::i_YPos,
		o_win.i_Width,
		o_win.i_Height,
		win::hd_WndParent,
		win::h_Menu,
		h_Instance,
		win::lv_Param
	);
	return o_win.hd_WindowHandle;
}
long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam)
{
	if (msg_Message == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT && o_win.b_HideCursor)
	{
		SetCursor(NULL);

		return TRUE;
	}

	if (msg_Message == WM_LBUTTONDOWN) {
		if (o_win.v_pMouseDown != NULL) {
			o_win.v_pMouseDown();
		}
		return 0;
	}

	if (msg_Message == WM_CLOSE) {
		DestroyWindow(hd_Handle);
		return 0;

	}
	if (msg_Message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	return (long)DefWindowProcW(hd_Handle, msg_Message, wParam, lParam);
}