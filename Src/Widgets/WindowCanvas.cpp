#include "WindowCanvas.h"

extern unsigned int GZindex;

void WindowCanvas::Tick(double dt)
{
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Tick(dt);
}

void WindowCanvas::SegmentRender(Vect2 Pos, Vect2 Size)
{
	Widget::SegmentRender(Pos, Size);
}

void WindowCanvas::AddItem(Widget* W)
{
	Items.push_back(W);
	W->GridSystem = &LocalGridSystem;
	W->Owner = this;
	W->Window = Window;
	W->ZIndex = GZindex++;
	LocalGridSystem.InsertWidget(*W);
	W->Attached();
}
