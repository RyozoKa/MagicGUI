
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

	//C++ really should consider overloading variables for top level functions.
	//Gridsubsystem CGridsystem;

	ClippedCanvas() : Canvas()
	{
		SetColor(Color(235, 240, 168));
		SetMode(TYPE::TYPE_COLOR);
	}

	~ClippedCanvas()
	{
		//delete GridSystem;
	}
	virtual void Tick(double dt);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);

	virtual void AddItem(Widget*);
	void Scroll(Vect2 Offset);
	virtual void SetPosition(const Vect2 Pos);
	virtual void RenderObjects();
};