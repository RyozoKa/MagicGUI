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

void ClippedCanvas::OnMouseEnter(float X, float Y)
{
	Widget* W = LastHit;
	
	if(CursorPos.X != X || CursorPos.Y != Y) 	
	{
		//Cursor moved, are we still inside the widget?
		CursorPos.X = X;
		CursorPos.Y = Y;
		if (!LastHit || !LastHit->TestCollision(CursorPos)) //If not, find the new widget
			W = GridSystem->GetWidget(CursorPos);
		if (LastHit != W)
		{
			if (LastHit != nullptr)
				LastHit->OnMouseLeave(X, Y);
			W->OnMouseEnter(X, Y);
			LastHit = W;
		}
		else if (W->Items.size() > 0)
			W->OnMouseEnter(X, Y);
	}
}

void ClippedCanvas::OnMouseLeave(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeave(X, Y);
}

void ClippedCanvas::OnMouseLeftClick(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeftClick(X, Y);
}

void ClippedCanvas::OnMouseRightClick(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseRightClick(X, Y);
}

void ClippedCanvas::OnMouseLeftReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeftReleased(X, Y);
}

void ClippedCanvas::OnMouseRightReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseRightReleased(X, Y);
}

void ClippedCanvas::OnKeyPressed(int Key, int Mod)
{
	if (LastHit)
		LastHit->OnKeyPressed(Key, Mod);
}

void ClippedCanvas::Attached()
{
	//Override previous grid subsystem
	SetColor(Color(235, 240, 168));
	SetMode(TYPE::TYPE_COLOR);
	GridSystem = new Gridsubsystem(this);
	GridSystem->InsertWidget(*this);
}

void ClippedCanvas::RenderObjects()
{
	//In case we have to do something here.
	Background.DrawObject(Position);
}
