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

	if (!VScrollPos.IsEmpty())
	{
		Up.SetPosition(VScrollPos - Vect2(0.f, ButtonSize.Y + BorderWidth));
		Left.SetPosition(HScrollPos - Vect2(ButtonSize.Y + BorderWidth, 0.f));
	}
	if (ScrollAreaSize > 0.f)
	{
		Down.SetPosition(VScrollPos + Vect2(0.f, ScrollAreaSize.Y + BorderWidth));
		Right.SetPosition(HScrollPos + Vect2(ScrollAreaSize.X + BorderWidth, 0.f));
	}
}

void ScrollPanel::OnMouseEnter(float X, float Y)
{
}

void ScrollPanel::OnMouseLeave(float X, float Y)
{
}

void ScrollPanel::OnMouseLeftClick(float X, float Y)
{
	//For Horizontal scroll bar
	//Is within the scroll area

	if(X >= HScrollPos.X && Y >= HScrollPos.Y &&
	   X <= (HScrollPos.X + ScrollAreaSize.X) && 
	   Y <= (HScrollPos.Y + ScrollAreaWidth) )
	{
		if (X > (HorizontalScroll.Position.X + HorizontalScroll.Size.X))
			HorizontalScroll.OnScroll(-2);
		else
			HorizontalScroll.OnScroll(2);
	}
	else if(X >= VScrollPos.X && Y >= VScrollPos.Y &&
	   X <= (VScrollPos.X + ScrollAreaWidth) && 
	   Y <= (VScrollPos.Y + ScrollAreaSize.Y) )
	{
		if (Y > (VerticalScroll.Position.Y + VerticalScroll.Size.Y))
			VerticalScroll.OnScroll(-2);
		else
			VerticalScroll.OnScroll(2);
	}
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
	Vect2 WPosRelative = W->Position;
	if ((WPosRelative + W->Size).X > (CalculatedSize + InnerPaddingWidth).X)
		CalculatedSize.X = (WPosRelative + W->Size).X + InnerPaddingWidth;
	if ((WPosRelative + W->Size).Y > (CalculatedSize + InnerPaddingWidth).Y)
		CalculatedSize.Y = (WPosRelative + W->Size).Y + InnerPaddingWidth;

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
	CC.ScrollX(-XOffset);
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

	CC.ScrollY(-YOffset);
}

void ScrollPanel::SetSize(const Vect2 Sz)
{
	Widget::SetSize(Sz);
	CC.SetSize(Sz - ScrollAreaWidth);
	Background.SetPrimitive(Sz);

	if (CalculatedSize < CC.Size)
		CalculatedSize = CC.Size;

	//Define scroll areas
	VScrollPos = Position + Vect2(Size.X - ScrollAreaWidth + BorderWidth, BorderWidth + ButtonSize.Y + BorderWidth);
	HScrollPos = Position + Vect2(BorderWidth + ButtonSize.X + BorderWidth, Size.Y - ScrollAreaWidth + BorderWidth);

	//Define total length
	ScrollAreaSize = Size - ScrollAreaWidth - BorderWidth - BorderWidth - (ButtonSize * 2) - BorderWidth - BorderWidth;

	//Calculate scroll size
	CalculateScroll();
}

void ScrollPanel::SetPosition(const Vect2 Pos)
{
	Widget::SetPosition(Pos);
	CC.SetPosition(Pos + 1.f);

	//Define scroll areas
	VScrollPos = Position + Vect2(Size.X - ScrollAreaWidth + BorderWidth, BorderWidth + ButtonSize.Y + BorderWidth);
	HScrollPos = Position + Vect2(BorderWidth + ButtonSize.X + BorderWidth, Size.Y - ScrollAreaWidth + BorderWidth);
	
	//Calculate scroll size
	CalculateScroll();
}

void ScrollPanel::Attached()
{
	Widget::AddItem(&CC);
	Widget::AddItem(&VerticalScroll);
	Widget::AddItem(&HorizontalScroll);
	Widget::AddItem(&Up);
	Widget::AddItem(&Down);
	Widget::AddItem(&Left);
	Widget::AddItem(&Right);
}

void YScrollCallback(Widget* W, float YOffset)
{
	((ScrollPanel*)W->Owner)->YScroll(YOffset);
}
void XScrollCallback(Widget* W, float XOffset)
{
	((ScrollPanel*)W->Owner)->XScroll(XOffset);
}

void OnClickDown(Widget* W)
{
	((ScrollPanel*)W->Owner)->VerticalScroll.OnScroll(-2);
}

void OnClickUp(Widget* W)
{
	((ScrollPanel*)W->Owner)->VerticalScroll.OnScroll(2);
}

void OnClickLeft(Widget* W)
{
	((ScrollPanel*)W->Owner)->HorizontalScroll.OnScroll(2);
}

void OnClickRight(Widget* W)
{
	((ScrollPanel*)W->Owner)->HorizontalScroll.OnScroll(-2);
}