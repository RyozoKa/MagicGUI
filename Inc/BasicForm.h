#pragma once

/*
	This is a basic form that contains a window's structure and widgets.

	Revision:
		Christopher Békési 2020 - 07 - 25
*/

#include "Template.h"
#include "Window.h"

class MAGICGUIAPI BasicForm
{
public:

	BasicForm(const Vect2 Size, const char* Title, MODE Mode, bool Borderless);

	void AddItem(class Widget*);
	class Window* GetWindow();

private:
	class Window* W;
};