#include "ClippedCanvas.h"
#include "Window.h"

void ClippedCanvas::Tick(double dt)
{
	ScissorBox(Position, Size);
		Widget::Tick(dt);
	DisableScissor();
}

void ClippedCanvas::SegmentRender(Vect2 Pos, Vect2 ASize)
{
	if (RectOverlap(Position, Position + Size, Pos, Pos + ASize))
	{
		ScissorBox(Position, Size);
			Widget::SegmentRender(Pos, ASize);
		DisableScissor();
	}
}

void ClippedCanvas::AddItem(Widget* W)
{
	W->Move(Position);
	Widget::AddItem(W);
}

void ClippedCanvas::Scroll(Vect2 Offset)
{
	bUpdate = true;
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Move(Offset);
}

void ClippedCanvas::SetPosition(const Vect2 Pos)
{
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Move(Pos - Position);
	Widget::SetPosition(Pos);
}

void ClippedCanvas::RenderObjects()
{
	//In case we have to do something here.
	Background.DrawObject(Position);
}
