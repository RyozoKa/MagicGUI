#include "Canvas.h"

void Canvas::SetColor(Color C)
{
	Background.SetColor(C);
	bUpdate = true;
}

void Canvas::SetImage(Texture* T)
{
	Background.SetImage(T);
	bUpdate = true;
}

void Canvas::SetMode(TYPE T)
{
	Background.SetRenderMode(T);
	bUpdate = true;
}

void Canvas::RenderObjects()
{
	Background.DrawObject(Vect2(0.f, 0.f));
}

void Canvas::Attached()
{
	LocalGridSystem.InsertWidget(*this);
	GridSystem = &LocalGridSystem;
	if(Owner)
		Buffer.AbsPosition = Owner->GridSystem->OwnerCanvas->Buffer.AbsPosition + Position;
}

void Canvas::AddItem(Widget* W)
{
	Widget::AddItem(W);

	//Calculate total canvas size (outside clipping region)
	Vect2 Sz = W->Position + W->Size;
	Vect2 OldSize = CalculatedSize;
	if (Sz.X > CalculatedSize.X)
		CalculatedSize.X = Sz.X;
	if (Sz.Y > CalculatedSize.Y)
		CalculatedSize.Y = Sz.Y;

	if (OldSize != CalculatedSize)
	{
		Buffer.SetSize(CalculatedSize);
		Background.SetPrimitive(CalculatedSize);
		
	}
}

void Canvas::OnCursor(double X, double Y)
{
	Widget* W = LastHit;
	CursorPos.X = X - Position.X;
	CursorPos.Y = Y - Position.Y;
	if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
		W = LocalGridSystem.GetWidget(CursorPos);
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
	if (W)
		W->OnCursor(CursorPos.X, CursorPos.Y);	//-- Constant update
}

void Canvas::OnKeyPress(int Key, int Mod)
{
	if (Focused)
		Focused->OnKeyPressed(Key, Mod);
}

void Canvas::OnKeyRelease(int Key, int Mod)
{
}

void Canvas::OnMouseLeftClick(float X, float Y)
{
	if(LocalGridSystem.bUpdate)
	{
		Widget* W = LastHit;
		if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
			W = LocalGridSystem.GetWidget(CursorPos);
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
		LocalGridSystem.bUpdate = false;
	}
	if (LastHit)
		LastHit->OnMouseLeftClick(X, Y);
}

void Canvas::OnMouseRightClick(float X, float Y)
{
	if(LocalGridSystem.bUpdate)
	{
		Widget* W = LastHit;
		if (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0) //If not, find the new widget
			W = LocalGridSystem.GetWidget(CursorPos);
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
		LocalGridSystem.bUpdate = false;
	}
	if (LastHit)
		LastHit->OnMouseRightClick(X, Y);
}

void Canvas::OnMouseLeftReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeftReleased(X, Y);
}

void Canvas::OnMouseRightReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseRightReleased(X, Y);
}

void Canvas::OnScroll(float YOffset)
{
	if (LastHit)
		LastHit->OnScroll(YOffset);
}

void Canvas::OnMouseLeave(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeave(X, Y);
}

void Canvas::Draw()
{
	//Perform segmented drawing if needed. We don't care about rendering overlays because Controls aren't meant to be stacked on top of eachother unless they are children.
	if(CurrentState == RenderState::STATE_Normal)
	{
		if (bTransparent)
		{
			GridSystem->UpdateSegment(*this, Position, DrawSize);
			return;
		}
	}
	bool bScissor = false;
	//Set up eventual scissorbox
	//The only time we ever need to set up a second scissor box is for per-Widget segmented draws.
	//Account for cases where the DrawSize + Position (The left and bottom edge of the object) is less than the previous clipping box.
	if((DrawSize < Size))
	{
		bScissor = true;
		ScissorBox(Position, DrawSize);
	}
	//Draw the background regardless. visibility is based on texture drawing
	//if (!bHidden)
	RenderObjects();

	if(bScissor)
		DisableScissor();
	bUpdate = false;
}

void Canvas::Show()
{
	Buffer.bUpdate = true;
	bHidden = false;
	//Widget::Show();
}

void Canvas::ScrollX(float XOffset)
{
	Offset.X += XOffset;
	Buffer.bUpdate = true;
}

void Canvas::ScrollY(float YOffset)
{
	Offset.Y += YOffset;
	Buffer.bUpdate = true;
}

void Canvas::Tick(double dt)
{
	Buffer.Select();
		Widget::Tick(dt);
	Buffer.Restore();

	if (!bHidden && Buffer.bUpdate)
	{
		Frame.TexOffset.X = Offset.X;
		Frame.TexOffset.Y = -Offset.Y;
		//Frame.TexOffset.Negate();
		Frame.DrawObject(Position);
		Buffer.bUpdate = false;
	}
	
}

void Canvas::SegmentRender(Vect2 Pos, Vect2 Size)
{
	//Buffer.Select();
		Widget::SegmentRender(Pos, Size);
	//Buffer.Restore();
	
	//Don't draw here, let it draw in tick.
	//if (Buffer.bUpdate)
	//{
		//Frame.TexOffset.X = Offset.X;
		//Frame.TexOffset.Y = -Offset.Y;
	//	Frame.DrawObject(Position);
	//	Buffer.bUpdate = false;
	//}
}
