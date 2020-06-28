
#pragma once
#include "Canvas.h"
#include "Gridsubsystem.h"

/*
	Special canvas that only draw items within the size

	Revision
		Christopher Békési 23-06-2020

*/
class MAGICGUIAPI ClippedCanvas : public Canvas
{
public:

	Widget* LastHit = nullptr; //--Last focused widget within the canvas
	Vect2 CursorPos = Vect2(-1.f, -1.f); 

	//C++ really should consider overloading variables for top level functions.
	//Gridsubsystem CGridsystem;

	ClippedCanvas() : Canvas()
	{
		SetColor(Color(235, 240, 168));
		SetMode(TYPE::TYPE_COLOR);
	}

	~ClippedCanvas()
	{
		delete GridSystem;
	}
	virtual void Tick(double dt);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);

	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnKeyPressed(int Key, int Mod);
	virtual void AddItem(Widget*);
	virtual void Attached();
	void Scroll(Vect2 Offset);
	virtual void SetPosition(const Vect2 Pos);
	virtual void RenderObjects();

	virtual bool TestCollision(const Vect2& V)
	{
		return Widget::TestCollision(V);
	}
};