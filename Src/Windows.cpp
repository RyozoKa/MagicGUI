/*
	Windows specific implementations

	Revision:
		Christopher Békési 2020-03-24
*/




#include "ContextMenu.h"
#include "Template.h"
#include <windows.h>
#include "Widget.h"

HDynamicArray<Callbacks<>> ContextMenu::CallbackRegistry;
bool bPopup = false;

void ErrorMessage(const char* Msg, const char* Title)
{
	MessageBoxA(NULL, Msg, Title, MB_TASKMODAL);
}

//OS Specific contex menu
#include "ContextMenu.h"



//Defined in win32_window.c GLFW
extern "C" void DelegateCallback(int index)
{
	if (index < ContextMenu::CurrentMenu->CallbackRegistry.Size())
		ContextMenu::CurrentMenu->CallbackRegistry[index].DelegateCallbacks();
}

typedef void(*Callback)(int);
extern Callback GetCallbackEntry;
int RegisterStuff()
{
	GetCallbackEntry = &DelegateCallback;
	return 0;
}
int IndexLevel = RegisterStuff();

void ContextMenu::AddString(const char* C, Callbacks<> CB)
{
	CallbackRegistry.Insert(CB);
	AppendMenuA((HMENU)Handle, MF_STRING, IndexLevel++, C);
}

void ContextMenu::AddSeparator()
{
	AppendMenuA((HMENU)Handle, MF_SEPARATOR, 0, NULL);
}

void ContextMenu::AddSubmenu(const char* C, ContextMenu CM)
{
	AppendMenuA((HMENU)Handle, MF_STRING | MF_POPUP, (UINT_PTR)CM.Handle, C);
}

ContextMenu::ContextMenu()
{
	Handle = (void*)CreatePopupMenu();
}

ContextMenu* ContextMenu::CurrentMenu = nullptr;

void Widget::ShowContextMenu()
{
	POINT Point;
	GetCursorPos(&Point);
	
	if (ContextMenuHandle)
	{
		//bPopup = true;
		if (!TrackPopupMenu((HMENU)ContextMenuHandle->Handle,
							TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							Point.x, Point.y, 0, GetActiveWindow(), NULL))
			printf("Last error: %i\n", GetLastError());
		else
			ContextMenu::CurrentMenu = ContextMenuHandle;
		//bPopup = false;
	}
}