#include "ClippedCanvas.h"
#include "Window.h"

void ClippedCanvas::Tick(double dt)
{
	ScissorBox(Position, Size);
		Widget::Tick(dt);
	DisableScissor();

	if (Window->CursorPos < Position || Window->CursorPos > (Position + Size))
		return;

	Widget* W = LastHit;
	
	if(Window->CursorPos != CursorPos) 	
	{
		//Cursor moved, are we still inside the widget?
		CursorPos.X = Window->CursorPos.X;
		CursorPos.Y = Window->CursorPos.Y;
		if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
			W = GridSystem->GetWidget(CursorPos);
		if (W == this)
			W = nullptr;
		if (LastHit != W)
		{
			if (LastHit != nullptr)
				LastHit->OnMouseLeave(CursorPos.X, CursorPos.Y);
			if(W)
				W->OnMouseEnter(CursorPos.X, CursorPos.Y);
			LastHit = W;
		}
	}
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
	if (LastHit)
		LastHit->OnMouseEnter(X, Y);
}

void ClippedCanvas::OnMouseLeave(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeave(X, Y);
}

void ClippedCanvas::OnMouseLeftClick(float X, float Y)
{
	if(GridSystem->bUpdate)
	{
		Widget* W = LastHit;
		if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
			W = GridSystem->GetWidget(CursorPos);
		if (LastHit != W)
		{
			if (LastHit != nullptr)
				LastHit->OnMouseLeave(CursorPos.X, CursorPos.Y);
			if(W)
				W->OnMouseEnter(CursorPos.X, CursorPos.Y);
			LastHit = W;
		}
	}
	if (LastHit)
		LastHit->OnMouseLeftClick(X, Y);
}

void ClippedCanvas::OnMouseRightClick(float X, float Y)
{
	if(GridSystem->bUpdate)
	{
		Widget* W = LastHit;
		if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
			W = GridSystem->GetWidget(CursorPos);
		if (LastHit != W)
		{
			if (LastHit != nullptr)
				LastHit->OnMouseLeave(CursorPos.X, CursorPos.Y);
			if(W)
				W->OnMouseEnter(CursorPos.X, CursorPos.Y);
			LastHit = W;
		}
	}
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

void ClippedCanvas::AddItem(Widget* W)
{
	W->Move(Position);
	Widget::AddItem(W);
}

void ClippedCanvas::Attached()
{
	//Override previous grid subsystem
	GridSystem = new Gridsubsystem(this);
	GridSystem->InsertWidget(*this);
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
