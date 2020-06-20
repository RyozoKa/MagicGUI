/*
	Windows specific implementations

	Revision:
		Christopher Békési 2020-03-24
*/





#include "Template.h"
#include <windows.h>

void ErrorMessage(const char* Msg, const char* Title)
{
	MessageBoxA(NULL, Msg, Title, MB_TASKMODAL);
}