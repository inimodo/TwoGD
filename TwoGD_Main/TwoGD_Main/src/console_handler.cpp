#include "twogd.h"


FILE*  gd_console::Create()
{
	return AllocC();
}
FILE*  gd_console::Create(const wchar_t* c_ConsoleTitle)
{
	if (gd_console::AllocC() == NULL) {
		return NULL;
	}
	gd_console::Rename(c_ConsoleTitle);
	return NULL;
}
FILE *  gd_console::AllocC()
{
	if (AllocConsole() == NULL) {
		return NULL;
	}
	return freopen("CON", "w", stdout);
}
BOOL  gd_console::Rename(const wchar_t* c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}
BOOL  gd_console::Destroy()
{
	return FreeConsole();
}