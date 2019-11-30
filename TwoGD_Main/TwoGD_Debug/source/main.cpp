#include"twogd.h"

GDCONSOLE gd_Console;

unsigned char __cdecl gdmain() {
	win::i_Width =300;
	win::i_Height = 300;
	gd_Console.Create();
	return TRUE;
}
unsigned char __cdecl gdupdate() {

	return TRUE;

}
void __cdecl gdclose() {

}