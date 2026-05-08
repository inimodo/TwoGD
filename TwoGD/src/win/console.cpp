#include "..\twogd.h"

console::console() 
{
	if (AllocConsole() == NULL)
	{
		return;
	}
	f_Console = freopen("CONOUT$", "w", stdout);
}

console::console(LPCWSTR c_ConsoleTitle)
{
	if (AllocConsole() == NULL)
	{
		return;
	}
	f_Console = freopen("CONOUT$", "w", stdout);
	SetConsoleTitleW(c_ConsoleTitle);
}

BOOL  console::Rename(LPCWSTR c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}

void console::SetColor(int i_HexColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i_HexColor);
}

BOOL  console::Destroy()
{
	return FreeConsole();
}