
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

	virtual void Attached();

	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		Background.SetPrimitive(Sz);
	}

	virtual void RenderObjects();
};