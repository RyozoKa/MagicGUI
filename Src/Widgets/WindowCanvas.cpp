#include "WindowCanvas.h"

void WindowCanvas::Tick(double dt)
{
	Widget::Tick(dt);
}

void WindowCanvas::SegmentRender(Vect2 Pos, Vect2 Size)
{
	Widget::SegmentRender(Pos, Size);
}

void WindowCanvas::AddItem(Widget* W)
{
	Widget::AddItem(W);
}
