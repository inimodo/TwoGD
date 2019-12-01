#include "twogd.h"

HINSTANCE win::h_Instance;
HWND win::hd_WindowHandle;
MSG win::msg_WindowMessage;
HDC win::hdc_WindowHdc;
WNDCLASS win::w_WndClass;
POINT win::p_CursorPos;
void(*win::v_pMouseDown)();
int win::i_Height;
int win::i_Width;


HANDLE __WAY CreateExec(HINSTANCE h_Instance);
long __WAY WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam);


DWORD __WAY wWinProcess(LPVOID lp_Void)
{
	while (gdupdate() != NULL)
	{

	}

	return 0; 
}
int __WAY wWinMain(HINSTANCE h_Instance, HINSTANCE, PWSTR c_pCmdLine, int i_CmdShow)
{
	if (gdmain() == NULL)
	{
		return NULL;
	}
	if (CreateExec(h_Instance) == NULL)
	{
		return NULL;
	}
	ShowWindow(win::hd_WindowHandle, i_CmdShow);
	UpdateWindow(win::hd_WindowHandle);

	win::hdc_WindowHdc = GetDC(win::hd_WindowHandle);

	HANDLE hwd_Thread = CreateThread(0,0, wWinProcess,0,0,NULL);

	while ((GetMessage(&win::msg_WindowMessage, NULL, 0, 0) > 0))
	{
		TranslateMessage(&win::msg_WindowMessage);
		DispatchMessage(&win::msg_WindowMessage);
	}
	CloseHandle(hwd_Thread);
	gdclose();

	ReleaseDC(NULL, win::hdc_WindowHdc);
	return NULL;
}
HANDLE __WAY CreateExec(HINSTANCE h_Instance) {
	win::w_WndClass.hbrBackground = (HBRUSH)(2);
	win::w_WndClass = { 0 };
	win::w_WndClass.lpfnWndProc = (WNDPROC)WindowProc;
	win::w_WndClass.hInstance = h_Instance;
	win::w_WndClass.lpszClassName = (LPCSTR)win::c_WinClassName;

	RegisterClass(&win::w_WndClass);

	win::h_Instance = h_Instance;
	win::hd_WindowHandle = CreateWindowEx(
		win::dw_ExStyle,
		(LPCSTR)win::c_WinClassName,
		(LPCSTR)win::c_WinTitle,
		win::dw_Style,
		win::i_XPos,
		win::i_YPos,
		win::i_Width,
		win::i_Height,
		win::hd_WndParent,
		win::h_Menu,
		h_Instance,
		win::lp_Param
	);
	return win::hd_WindowHandle;
}
long __WAY WindowProc(HWND hd_Handle, UINT msg_Message, WPARAM wParam, LPARAM lParam)
{
	if (msg_Message == WM_LBUTTONDOWN) {
		if (win::v_pMouseDown != NULL) {
			win::v_pMouseDown();
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

	return DefWindowProcW(hd_Handle, msg_Message, wParam, lParam);
}
void  SetScreenBuffer(DWORD * dw_ColorStream, int i_dWidth, int di_Height) {
	HBITMAP bmp_Bitmap = CreateBitmap(i_dWidth, di_Height, 1, 8 * 4, (const void *)dw_ColorStream);
	HDC hdc_TempHdc = CreateCompatibleDC(win::hdc_WindowHdc);
	SelectObject(hdc_TempHdc, bmp_Bitmap);
	BitBlt(win::hdc_WindowHdc, 0, 0, i_dWidth, di_Height, hdc_TempHdc, 0, 0, SRCCOPY);
	DeleteObject(bmp_Bitmap);
	DeleteDC(hdc_TempHdc);
}
void GetCursorPosition() {
	GetCursorPos(&win::p_CursorPos);
	ScreenToClient(win::hd_WindowHandle, &win::p_CursorPos);
}