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
	virtual void Tick(double Delta);

	RenderObject Background;

	//Canvas interface
	void SetColor(Color&);		//-- Background color
	void SetImage(Texture*);	//-- Background image
	virtual void SetMode(TYPE);	

	virtual void RenderObjects();

	Canvas()
	{
		Background.Owner = this;
	}

	virtual void SetSize(const Vect2& Sz)
	{
		Widget::SetSize(Sz);
		Background.SetPrimitive(Sz);
	}

	//A Canvas will never collide
	virtual bool TestCollision(const Vect2&)
	{
		return false;
	}

	//Canvas interface
	//Used by segmented rendering
	void SegmentRender()
	{
		if (bUpdate)
			Draw();
		for(int i = 0; i < Items.size(); ++i)
		{
			if (Items[i]->bUpdate)
				Items[i]->Draw();
		}
	}
};