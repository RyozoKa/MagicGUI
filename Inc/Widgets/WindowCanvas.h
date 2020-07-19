#pragma once

#include "Canvas.h"

class MAGICGUIAPI WindowCanvas : public Canvas
{
public:

	WindowCanvas() : Canvas()
	{
	}

	//Override
	virtual void Tick(double dt);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);

	virtual void AddItem(Widget*);

	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		Background.SetPrimitive(Sz);
		Buffer.Size = Sz;
	}

	virtual void OnScroll(float YOffset)
	{
		if (LastHit)
			LastHit->OnScroll(YOffset);
	}
};