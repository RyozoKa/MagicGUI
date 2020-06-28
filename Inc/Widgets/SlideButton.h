#pragma once

#include "Button.h"
#include "CallbackChain.h"

class MAGICGUIAPI SlideButton : public Button
{
public:

	bool bHolding = false;		//-- This gets set when a button is sliding
	bool bVertical = false;		//-- If true, button slides vertically, otherwise horizontally
	float SlideStop;			//-- The max length of which this button should move
	Vect2 Start;
	Vect2 LastCursor;

	Callbacks<Widget*, float> OnSlide;

	SlideButton() : Button()
	{
	
	}

	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnScroll(float YOffset);

	virtual void Tick(double DT);
};