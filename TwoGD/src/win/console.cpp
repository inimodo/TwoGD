#include "..\twogd.h"

console::console() 
{
	if (AllocConsole() == NULL)
	{
		return;
	}
	f_Console = freopen("CONOUT$", "w", stdout);
}

void console::Clear()
{

}

BOOL  console::Rename(const wchar_t* c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}

void console::SetColor(int i_HexColor)
{
	SetConsoleTextAttribute(h_ConsoleHandle, i_HexColor);
}

BOOL  console::Destroy()
{
	return FreeConsole();
}