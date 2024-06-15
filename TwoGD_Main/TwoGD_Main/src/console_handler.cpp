#include "twogd.h"


FILE*  o_console::Create()
{
	return AllocC();
}
FILE*  o_console::Create(const wchar_t* c_ConsoleTitle)
{
	if (o_console::AllocC() == NULL) {
		return NULL;
	}
	o_console::Rename(c_ConsoleTitle);
	return NULL;
}
FILE *  o_console::AllocC()
{
	if (AllocConsole() == NULL) {
		return NULL;
	}
	return freopen("CON", "w", stdout);
}
BOOL  o_console::Rename(const wchar_t* c_ConsoleTitle)
{
	return SetConsoleTitleW(c_ConsoleTitle);

}
BOOL  o_console::Destroy()
{
	return FreeConsole();
}