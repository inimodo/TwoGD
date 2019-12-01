#include "twogd.h"


FILE* __WAY gd_console::Create()
{
	return AllocC();
}
FILE* __WAY gd_console::Create(const wchar_t* c_ConsoleTitle)
{
	if (gd_console::AllocC() == NULL) {
		return NULL;
	}
	gd_console::Rename(c_ConsoleTitle);
	return NULL;
}
FILE * __WAY gd_console::AllocC()
{
	if (AllocConsole() == NULL) {
		return NULL;
	}
	return freopen("CON", "w", stdout);
}
BOOL __WAY gd_console::Rename(const wchar_t* c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}
BOOL __WAY gd_console::Destroy()
{
	return FreeConsole();
}