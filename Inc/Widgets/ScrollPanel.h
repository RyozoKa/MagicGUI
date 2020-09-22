/*
	An X/Y scrollable panel containing children
	Revision:
		Christopher Békési 22-06-2020

*/

#pragma once

#include "SlideButton.h"
#include "Canvas.h"

MAGICGUIAPI void YScrollCallback(Widget* W, float YOffset);
MAGICGUIAPI void XScrollCallback(Widget* W, float XOffset);

MAGICGUIAPI void OnClickDown(Widget* W);
MAGICGUIAPI void OnClickUp(Widget* W);
MAGICGUIAPI void OnClickLeft(Widget* W);
MAGICGUIAPI void OnClickRight(Widget* W);

class MAGICGUIAPI ScrollPanel : public Widget
{
	public:
	//ScrollPanel
	Vect2 ScrollOffset;		//-- Current scroll position

	Vect2 CalculatedSize;	//-- Total calculated size based on the lowest sitting widget

	Vect2 VScrollPos;		//-- Vertical starting position
	Vect2 HScrollPos;		//-- Horizontal starting position

	Vect2 ScrollAreaSize;	//-- Scroll area length. This is the total length of the scroll area

	Vect2 ScrollAreaLen;	//-- Relative area length with scroll bar size taken into account

	float ScrollbarWidth = 12;
	float ScrollAreaWidth = 20;
	Vect2 ButtonSize = {ScrollbarWidth, ScrollbarWidth};
	float BorderWidth = (ScrollAreaWidth - ScrollbarWidth) / 2;

	float InnerPaddingWidth = 0.f;	//-- Padding thickness around Canvas edge	
	const float ScrollScale = 10.f;
	SlideButton VerticalScroll;
	SlideButton HorizontalScroll;
	
	Button Up;
	Button Down;
	Button Left;
	Button Right;

	RenderObject Background;
	Canvas &CC = *new Canvas;


	ScrollPanel() : Widget()
	{
		CC.Owner = this;
		SetExpansion(Expansion::EXPAND_DOWN | Expansion::EXPAND_RIGHT);
		Background.Owner = this;
		Background.SetColor(Color(79, 79, 78));
		CC.SetColor({ 252, 255, 219, 255 });
		//VerticalScroll.bHidden = true;
		//HorizontalScroll.bHidden = true;
		VerticalScroll.SetStateColor(Color(163, 163, 163), Button::ButtonState::STATE_Normal);
		VerticalScroll.SetStateColor(Color(214, 214, 214), Button::ButtonState::STATE_Hover);
		VerticalScroll.SetStateColor(Color(255, 255, 255), Button::ButtonState::STATE_Pressed);
		HorizontalScroll.SetStateColor(Color(163, 163, 163), Button::ButtonState::STATE_Normal);
		HorizontalScroll.SetStateColor(Color(214, 214, 214), Button::ButtonState::STATE_Hover);
		HorizontalScroll.SetStateColor(Color(255, 255, 255), Button::ButtonState::STATE_Pressed);
		HorizontalScroll.SetType(TYPE::TYPE_COLOR);
		VerticalScroll.SetType(TYPE::TYPE_COLOR);
		VerticalScroll.bVertical = true;

		HorizontalScroll.OnSlide += &XScrollCallback;
		VerticalScroll.OnSlide += &YScrollCallback;

		Up.OnClick += &OnClickUp;
		Down.OnClick += &OnClickDown;
		Left.OnClick += OnClickLeft;
		Right.OnClick += OnClickRight;

		Up.SetSize(ButtonSize);
		Down.SetSize(ButtonSize);
		Left.SetSize(ButtonSize);
		Right.SetSize(ButtonSize);

		CC.OnCanvasResize += ClassDelegate((void*)&ScrollPanel::OnCanvasChanged, this);
	}

	//ScrollPanel
	void CalculateScroll();
	//void ScrollHorizontal(float XOffset);
	//void ScrollVertical(float YOffset);

	//Widget interface
	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnKeyPressed(int Key, int Mod);
	virtual void OnScroll(float YOffset);
	virtual void AddItem(Widget*);
	virtual void RenderObjects();

	void XScroll(float XOffset);
	void YScroll(float YOffset);

	virtual void SetSize(const Vect2 Sz);
	virtual void SetPosition(const Vect2 Pos);

	virtual void Attached();
	virtual void OnCanvasChanged(Canvas* C);	//-- Linked canvas has changed in size

};