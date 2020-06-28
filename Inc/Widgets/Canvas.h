/*
	Main canvas widget.
	This widget covers the entire window and acts as the canvas on which other widgets can paint.

	Revision:
		Christopher Békési 2020-04-20

*/

#pragma once
#include "Widget.h"
#include "Texture.h"

class MAGICGUIAPI Canvas : public Widget
{
public:

	RenderObject Background;
	Vect2 Offset = Vect2(0.f, 0.f);			//-- Collision grid offset. Only used when applying scroll offsets to avoid moving the grid.

	//Canvas interface
	void SetColor(const Color);		//-- Background color
	void SetImage(Texture*);	//-- Background image
	virtual void SetMode(TYPE);	

	virtual void RenderObjects();

	Canvas()
	{
		Background.Owner = this;
	}

	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		Background.SetPrimitive(Sz);
	}

	//A Canvas will never collide
	virtual bool TestCollision(const Vect2&)
	{
		return false;
	}

	bool ShouldDraw(Widget* W)
	{
		return (Position + Size) > W->Position;
	}
};