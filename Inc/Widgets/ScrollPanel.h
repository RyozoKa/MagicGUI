/*
	An X/Y scrollable panel containing children
	Revision:
		Christopher Békési 22-06-2020

*/

#pragma once

#include "Button.h"

class MAGICGUIAPI ScrollPanel : public Widget
{

	//ScrollPanel
	Vect2 ScrollOffset;		//-- Current scroll position

	Vect2 CalculatedSize;	//-- Total calculated size based on the lowest sitting widget

	Vect2 VScrollPos;		//-- Vertical starting position
	Vect2 HScrollPos;		//-- Horizontal starting position

	Vect2 ScrollAreaSize;	//-- Scroll area length. This is the total length of the scroll area

	Vect2 ScrollAreaLen;	//-- Relative area length with scroll bar size taken into account

	float ScrollbarWidth = 16;
	float ScrollAreaWidth = 20;
	float BorderWidth = (ScrollAreaWidth - ScrollbarWidth) / 2;
	RenderObject PanelCanvas;
	Button VerticalScroll;
	Button HorizontalScroll;
	bool bHorizontalFocus = false;

	ScrollPanel()
	{
		VerticalScroll.bHidden = true;
		HorizontalScroll.bHidden = true;
		VerticalScroll.SetStateColor(Color(122, 122, 122), Button::ButtonState::STATE_Normal);
		VerticalScroll.SetStateColor(Color(64, 64, 64), Button::ButtonState::STATE_Hover);
		VerticalScroll.SetStateColor(Color(64, 64, 64), Button::ButtonState::STATE_Pressed);
		HorizontalScroll.SetStateColor(Color(122, 122, 122), Button::ButtonState::STATE_Normal);
		HorizontalScroll.SetStateColor(Color(64, 64, 64), Button::ButtonState::STATE_Hover);
		HorizontalScroll.SetStateColor(Color(64, 64, 64), Button::ButtonState::STATE_Pressed);
		HorizontalScroll.SetType(TYPE::TYPE_COLOR);
		AddItem(&VerticalScroll);
		AddItem(&HorizontalScroll);
	}

	//ScrollPanel
	void CalculateScroll();
	void ScrollHorizontal(float XOffset);
	void ScrollVertical(float YOffset);


	//Widget interface
	//virtual void OnMouseEnter(float X, float Y);
	//virtual void OnMouseLeave(float X, float Y);
	//virtual void OnMouseLeftClick(float X, float Y);
	//virtual void OnMouseRightClick(float X, float Y);
	//virtual void OnMouseLeftReleased(float X, float Y);
	//virtual void OnMouseRightReleased(float X, float Y);
	//virtual void OnKeyPressed(int Key, int Mod);
	virtual void OnScroll(float YOffset);
	virtual void AddItem(Widget*);
	virtual void RenderObjects();

	virtual void SetSize(const Vect2 Sz);

};