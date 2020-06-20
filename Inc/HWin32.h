/* OS specific includes and definitions
	Revision:
		Christopher Békési - initial build
*/

//Forward declarations
#pragma once


//#include <Windows.h>
#include <ctime>

#pragma warning(disable: 4996)
#pragma warning(disable: 4521)
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#pragma warning(disable: 4267)
#pragma warning(disable: 4995)

//Library loading

#define ldlib(c) LoadLibraryA(c)
#define ldfunc(handle,funcname) GetProcAddress((HMODULE)handle,funcname)
#define LIBEXT	".dll"
#define DLLPROTECT //Nothing on windows
#define DIRJOIN '\\'
#define DIRJOINSTR "\\"
#define ODIRJOINSTR "/"
#define ODIRJOIN '/'