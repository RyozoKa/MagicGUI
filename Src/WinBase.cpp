/*
	Windows specific initialization code

	Revision:
		2020-03-11 Christopher Békési
*/
#include "Window.h"
#include <Windows.h>


/*bool Window::InitializeDriver(const char* Driver)
{
	HINSTANCE hdll;
	hdll = LoadLibrary(Driver);
	if (!hdll)
		return false;

	typedef DriverInfo*(*GetDriverInfo)();
	GetDriverInfo GDI = (GetDriverInfo)GetProcAddress(hdll, "GetDriverInfo");
	if (!GDI)
		return false;

	if (Interface)
		Interface->Deinitialize();

	Info = GDI();
	Interface = Info->EntryPoint;
	Interface->Initialize();
	return true;
}
*/

