#include "ScrollPanel.h"

void ScrollPanel::CalculateScroll()
{
	//Calculate Scrollbar length based on the scroll panel size
	Vect2 XLen;
	Vect2 YLen;

	XLen = { ScrollAreaSize.X * (CC.Size.X / CalculatedSize.X) , ScrollbarWidth };
	YLen = { ScrollbarWidth , ScrollAreaSize.Y * (CC.Size.Y / CalculatedSize.Y) };

	ScrollAreaLen.X = ScrollAreaSize.X - XLen.X;
	ScrollAreaLen.Y = ScrollAreaSize.Y - YLen.Y;

	VerticalScroll.SetSize(YLen);
	HorizontalScroll.SetSize(XLen);

	VerticalScroll.SetPosition(VScrollPos);
	HorizontalScroll.SetPosition(HScrollPos);

	HorizontalScroll.Start = HScrollPos;
	VerticalScroll.Start = VScrollPos;

	HorizontalScroll.SlideStop = HScrollPos.X + ScrollAreaLen.X;
	VerticalScroll.SlideStop = VScrollPos.Y + ScrollAreaLen.Y;
}

void ScrollPanel::OnMouseEnter(float X, float Y)
{
}

void ScrollPanel::OnMouseLeave(float X, float Y)
{
}

void ScrollPanel::OnMouseLeftClick(float X, float Y)
{
}

void ScrollPanel::OnMouseRightClick(float X, float Y)
{
}

void ScrollPanel::OnMouseLeftReleased(float X, float Y)
{
}

void ScrollPanel::OnMouseRightReleased(float X, float Y)
{
}

void ScrollPanel::OnKeyPressed(int Key, int Mod)
{
}


void ScrollPanel::OnScroll(float YOffset)
{
	/*if (YOffset > 0.f && ((ScrollOffset.X == 0.f && bHorizontalFocus) || (ScrollOffset.Y == 0.f && !bHorizontalFocus)))
		return;

	//Apply scale
	YOffset *= ScrollScale;

	//Apply clamping if needed
	if (!bHorizontalFocus)
	{
		if ((YOffset + ScrollOffset.Y) > 0.f)
			YOffset = -ScrollOffset.Y;
		else if (-((ScrollOffset.Y - CC.Size.Y) + YOffset) > CalculatedSize.Y)
			YOffset += (-((ScrollOffset.Y - CC.Size.Y) + YOffset) - CalculatedSize.Y);
		ScrollOffset.Y += YOffset;
		CC.Scroll({ 0.f, YOffset });
	}
	else
	{
		if ((YOffset + ScrollOffset.X) > 0.f)
			YOffset = -ScrollOffset.X;
		else if (-((ScrollOffset.X - CC.Size.X) + YOffset) > CalculatedSize.X)
			YOffset += (-((ScrollOffset.X - CC.Size.X) + YOffset) - CalculatedSize.X);
		ScrollOffset.X += YOffset;
		CC.Scroll({ YOffset, 0.f });
	}*/
}

void ScrollPanel::AddItem(Widget* W)
{
	CC.AddItem(W);
	Vect2 WPosRelative = W->Position - CC.Position;
	if ((WPosRelative + W->Size).X > (CalculatedSize + InnerPaddingWidth).X)
		CalculatedSize.X = (WPosRelative + W->Size).X + InnerPaddingWidth;
	if ((WPosRelative + W->Size).Y > (CalculatedSize + InnerPaddingWidth).Y)
		CalculatedSize.Y = (WPosRelative + W->Size).Y + InnerPaddingWidth;
	CalculatedSize.X = 500;
	CalculateScroll();
}

void ScrollPanel::RenderObjects()
{
	Background.DrawObject(Position);
}

void ScrollPanel::XScroll(float XOffset)
{
	//Refactor
	float Scale = XOffset / ScrollAreaLen.X;
	XOffset = (CalculatedSize.X - CC.Size.X) * Scale;

	if ((XOffset + ScrollOffset.X) > 0.f)
		XOffset = -ScrollOffset.X;
	else if (-((ScrollOffset.X - CC.Size.X) + XOffset) > CalculatedSize.X)
		XOffset += (-((ScrollOffset.X - CC.Size.X) + XOffset) - CalculatedSize.X);
	ScrollOffset.X += XOffset;
	CC.Offset.X = ScrollOffset.X;
	CC.Scroll({ XOffset, 0.f });
}

void ScrollPanel::YScroll(float YOffset)
{
	float Scale = YOffset / ScrollAreaLen.Y;
	YOffset = (CalculatedSize.Y - CC.Size.Y) * Scale;

	if ((YOffset + ScrollOffset.Y) > 0.f)
		YOffset = -ScrollOffset.Y;
	else if (-((ScrollOffset.Y - CC.Size.Y) + YOffset) > CalculatedSize.Y)
		YOffset += (-((ScrollOffset.Y - CC.Size.Y) + YOffset) - CalculatedSize.Y);
	ScrollOffset.Y += YOffset;
	CC.Offset.Y = ScrollOffset.Y;
	CC.Scroll({ 0.f, YOffset });
}

void ScrollPanel::SetSize(const Vect2 Sz)
{
	Widget::SetSize(Sz);
	CC.SetSize(Sz - ScrollAreaWidth);
	Background.SetPrimitive(Sz);

	if (CalculatedSize < CC.Size)
		CalculatedSize = CC.Size;

	//Define scroll areas
	VScrollPos = Position + Vect2(Size.X - ScrollAreaWidth + BorderWidth, BorderWidth);
	HScrollPos = Position + Vect2(BorderWidth, Size.Y - ScrollAreaWidth + BorderWidth);

	//Define total length
	ScrollAreaSize = Size - ScrollAreaWidth - BorderWidth - BorderWidth;

	//Calculate scroll size
	CalculateScroll();
}

void ScrollPanel::SetPosition(const Vect2 Pos)
{
	Widget::SetPosition(Pos);
	CC.SetPosition(Pos + 1.f);

	//Define scroll areas
	VScrollPos = Position + Vect2(Size.X - ScrollAreaWidth + BorderWidth, BorderWidth);
	HScrollPos = Position + Vect2(BorderWidth, Size.Y - ScrollAreaWidth + BorderWidth);
	
	//Calculate scroll size
	CalculateScroll();
}

void ScrollPanel::Attached()
{
	Widget::AddItem(&CC);
	Widget::AddItem(&VerticalScroll);
	Widget::AddItem(&HorizontalScroll);
}

void YScrollCallback(Widget* W, float YOffset)
{
	((ScrollPanel*)W->Owner)->YScroll(YOffset);
}
void XScrollCallback(Widget* W, float XOffset)
{
	((ScrollPanel*)W->Owner)->XScroll(XOffset);
}