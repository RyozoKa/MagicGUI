/*
	Main canvas widget.
	This widget covers the entire window and acts as the canvas on which other widgets can paint.

	Revision:
		Christopher Békési 2020-04-20

*/

#pragma once
#include "Widget.h"
#include "Texture.h"
#include "Gridsubsystem.h"
#include "RenderBuffer.h"
class MAGICGUIAPI Canvas : public Widget
{
public:

	RenderObject Background;
	Vect2 Offset = Vect2(0.f, 0.f);			//-- Collision grid offset. Only used when applying scroll offsets to avoid moving the grid.
	Vect2 CalculatedSize;
	//I/O Related internals
	Gridsubsystem LocalGridSystem;
	Vect2 CursorPos;
	Widget* LastHit = nullptr;
	Widget* Focused = nullptr;

	RenderBuffer Buffer;		//-- Actual framebuffer
	RenderObject Frame;			//-- Frame on which this buffer will be drawn

	//Canvas interface
	void SetColor(const Color);		//-- Background color
	void SetImage(Texture*);		//-- Background image
	virtual void SetMode(TYPE);	

	virtual void RenderObjects();

	Canvas() : LocalGridSystem(this) , Buffer(&Frame)
	{
		Background.Owner = this;
		Frame.Owner = this;
		Buffer.Owner = this;
	}
	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		Background.SetPrimitive(Sz);
		Frame.SetPrimitive(Sz);
		if(Buffer.Size < Sz)
			Buffer.SetSize(Sz);
		Frame.SetImage(Buffer.GetTexture());
		if (Sz.X > CalculatedSize.X)
			CalculatedSize.X = Sz.X;
		if (Sz.Y > CalculatedSize.Y)
			CalculatedSize.Y = Sz.Y;
	}

	virtual void SetPosition(const Vect2 Pos)
	{
		Widget::SetPosition(Pos);
		if(Owner && GridSystem)
			Buffer.AbsPosition = Owner->GridSystem->OwnerCanvas->Buffer.AbsPosition + Position;
	}
	//A Canvas will never collide
	/*virtual bool TestCollision(const Vect2&)
	{
		return false;
	}*/
	bool ShouldDraw(Widget* W)
	{
		return (Position + Size) > W->Position;
	}

	virtual void Attached();

	virtual void AddItem(Widget*);

	virtual void OnCursor(double X, double Y);
	virtual void OnKeyPress(int Key, int Mod);
	virtual void OnKeyRelease(int Key, int Mod);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnScroll(float YOffset);
	//We need this in order to register leaves when a widget is by the edge.
	virtual void OnMouseLeave(float X, float Y);

	virtual void Draw();

	virtual void Show();

	void ScrollX(float XOffset);
	void ScrollY(float YOffset);

	virtual void Tick(double dt);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);
};