#include "twogd.h"


win::GDWIN gd_win;

HANDLE  CreateExec(HINSTANCE h_Instance);
long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);


DWORD  wWinProcess(LPVOID lp_Void)
{
	DWORD* dw_pFrame = 0;
	while(TRUE)
	{
		POINT p_CursorP;
		GetCursorPos(&p_CursorP);
		ScreenToClient(gd_win.hd_WindowHandle, &p_CursorP);
		gd_win.p_CursorPos.f_Pos[0] = p_CursorP.x;
		gd_win.p_CursorPos.f_Pos[1] = p_CursorP.y;

		dw_pFrame = gdUpdate(&gd_win);
		if (dw_pFrame == NULL) break;

		HBITMAP bmp_Bitmap = CreateBitmap(gd_win.i_Width, gd_win.i_Height, 1, 8 * 4, (const void *)dw_pFrame);
		HDC hdc_TempHdc = CreateCompatibleDC(gd_win.hdc_WindowHdc);
		SelectObject(hdc_TempHdc, bmp_Bitmap);
		BitBlt(gd_win.hdc_WindowHdc, 0, 0, gd_win.i_Width, gd_win.i_Height, hdc_TempHdc, 0, 0, SRCCOPY);
		DeleteObject(bmp_Bitmap);
		DeleteDC(hdc_TempHdc);

	}
	gdClose();

	return 0; 
}
int  wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow)
{
	if (gdMain(&gd_win) == NULL)
	{
		return NULL;
	}
	if (CreateExec(h_Instance) == NULL)
	{
		return NULL;
	}
	ShowWindow(gd_win.hd_WindowHandle, i_CmdShow);
	UpdateWindow(gd_win.hd_WindowHandle);

	gd_win.hdc_WindowHdc = GetDC(gd_win.hd_WindowHandle);

	HANDLE hwd_Thread = CreateThread(0,0, (LPTHREAD_START_ROUTINE)wWinProcess,0,0,NULL);

	while ((GetMessage(&gd_win.msg_WindowMessage, NULL, 0, 0) > 0))
	{
		TranslateMessage(&gd_win.msg_WindowMessage);
		DispatchMessage(&gd_win.msg_WindowMessage);
	}

	printf("%d",CloseHandle(hwd_Thread));

	ReleaseDC(NULL, gd_win.hdc_WindowHdc);
	return NULL;
}
HANDLE  CreateExec(HINSTANCE h_Instance) {
	gd_win.w_WndClass.hbrBackground = (HBRUSH)(2);
	gd_win.w_WndClass = { 0 };
	gd_win.w_WndClass.lpfnWndProc = (WNDPROC)WindowProc;
	gd_win.w_WndClass.hInstance = h_Instance;
	gd_win.w_WndClass.lpszClassName = (LPCWSTR)win::c_WinClassName;

	RegisterClass(&gd_win.w_WndClass);

	gd_win.h_Instance = h_Instance;
	gd_win.hd_WindowHandle = CreateWindowEx(
		win::dw_ExStyle,
		(LPCWSTR)win::c_WinClassName,
		(LPCWSTR)win::c_WinTitle,
		win::dw_Style,
		win::i_XPos,
		win::i_YPos,
		gd_win.i_Width,
		gd_win.i_Height,
		win::hd_WndParent,
		win::h_Menu,
		h_Instance,
		win::lp_Param
	);
	return gd_win.hd_WindowHandle;
}
long  WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam)
{
	if (msg_Message == WM_LBUTTONDOWN) {
		if (gd_win.v_pMouseDown != NULL) {
			gd_win.v_pMouseDown();
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