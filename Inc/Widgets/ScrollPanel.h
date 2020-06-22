/*
	An X/Y scrollable panel containing children
	Revision:
		Christopher Békési 22-06-2020

*/

#pragma once

#include "Widget.h"

class MAGICGUIAPI ScrollPanel : public Widget
{

	//ScrollPanel
	Vect2 ScrollOffset;		//-- Current scroll position

	//Widget interface
	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnKeyPressed(int Key, int Mod);
	virtual void OnScroll(float YOffset);

};