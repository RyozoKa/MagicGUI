#pragma once

#include "Template.h"

extern int IndexLevel;

class MAGICGUIAPI ContextMenu
{
public:
	void* Handle = nullptr;
	static HDynamicArray<Callbacks<>> CallbackRegistry;
	
	
	ContextMenu();

	virtual void AddString(const char*, Callbacks<> CB);
	virtual void AddSeparator();
	virtual void AddSubmenu(const char*, ContextMenu CM);

	static ContextMenu* CurrentMenu;

};