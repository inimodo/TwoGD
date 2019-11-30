#include "twogd.h"


FILE* __stdcall gd_console::Create()
{
	return AllocC();
}
FILE* __stdcall gd_console::Create(const wchar_t* c_ConsoleTitle)
{
	if (gd_console::AllocC() == NULL) {
		return NULL;
	}
	gd_console::Rename(c_ConsoleTitle);
	return NULL;
}
FILE * __stdcall gd_console::AllocC()
{
	if (AllocConsole() == NULL) {
		return NULL;
	}
	return freopen("CON", "w", stdout);
}
BOOL __stdcall gd_console::Rename(const wchar_t* c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}
BOOL __stdcall gd_console::Destroy()
{
	return FreeConsole();
}