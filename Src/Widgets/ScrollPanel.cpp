#include "ScrollPanel.h"

void ScrollPanel::CalculateScroll()
{
	//Calculate Scrollbar length based on the scroll panel size
	Vect2 XLen;
	Vect2 YLen;

	//XLen = 
}

void ScrollPanel::OnScroll(float YOffset)
{

}

void ScrollPanel::AddItem(Widget*)
{
}

void ScrollPanel::RenderObjects()
{
	PanelCanvas.DrawObject(Position);
}

void ScrollPanel::SetSize(const Vect2 Sz)
{
	Widget::SetSize(Sz);
	PanelCanvas.SetPrimitive(Sz);

	//Define scroll areas
	VScrollPos = Position + Vect2(Size.X - ScrollAreaWidth + BorderWidth, BorderWidth);
	HScrollPos = Position + Vect2(BorderWidth, Size.Y - ScrollAreaWidth + BorderWidth);

	//Define total length
	ScrollAreaSize = Size - ScrollAreaWidth - BorderWidth;

	//Calculate scroll size
	CalculateScroll();
}
