#include "Canvas.h"

extern unsigned int GZindex;

void Canvas::SetColor(Color C)
{
	BackgroundImage.SetColor(C);
	bUpdate = true;
}

void Canvas::SetImage(Texture* T)
{
	BackgroundImage.SetImage(T);
	bUpdate = true;
}

void Canvas::SetMode(TYPE T)
{
	//Background.SetRenderMode(T);
	BackgroundImage.SetType(T);
	bUpdate = true;
}

void Canvas::RenderObjects()
{
	//Background.DrawObject(Vect2(0.f, 0.f));
}

void Canvas::Attached()
{

	//LocalGridSystem.InsertWidget(*this);
	GridSystem = &LocalGridSystem;
	AddItem(&BackgroundImage);
	
	if (Owner)
	{
		Buffer.AbsPosition = Owner->GridSystem->OwnerCanvas->Buffer.AbsPosition + Position;
		ParentCanvas = Owner->GridSystem->OwnerCanvas;
	}
}

void Canvas::AddItem(Widget* W)
{
	Items.push_back(W);
	W->GridSystem = &LocalGridSystem;
	W->Owner = this;
	W->Window = Window;
	W->ZIndex = GZindex++;
	LocalGridSystem.InsertWidget(*W);
	W->Attached();

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
		//Background.SetPrimitive(CalculatedSize);
		BackgroundImage.SetSize(CalculatedSize);
	}
}

void Canvas::OnCursor(double X, double Y)
{
	Widget* W = LastHit;
	CursorPos.X = X - Position.X;
	CursorPos.Y = Y - Position.Y;
	if (!bPressed && (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0)) //If not, find the new widget
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
		if (!bPressed && (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0)) //If not, find the new widget
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
	{
		LastHit->OnMouseLeftClick(X, Y);
		bPressed = true;
	}

}

void Canvas::OnMouseRightClick(float X, float Y)
{
	if(LocalGridSystem.bUpdate)
	{
		Widget* W = LastHit;
		if (!bPressed && (!LastHit || !LastHit->TestCollision(CursorPos) || LastHit->Items.size() > 0)) //If not, find the new widget
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
	{
		LastHit->OnMouseRightClick(X, Y);
		bPressed = true;
	}
}

void Canvas::OnMouseLeftReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseLeftReleased(X, Y);
	bPressed = false;
}

void Canvas::OnMouseRightReleased(float X, float Y)
{
	if (LastHit)
		LastHit->OnMouseRightReleased(X, Y);
	bPressed = false;
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
	LastHit = nullptr;
}

void Canvas::Draw()
{
	if (bHidden)
		return;
	//Draw the background regardless. visibility is based on texture drawing
	//if (!bHidden)
	/*if (CurrentState == RenderState::STATE_Normal)
	{
		RenderObjects();

		//if(bScissor)
		//	DisableScissor();
		bUpdate = false;
	}*/
	bool bScissor = false;
	//Set up eventual scissorbox
	//The only time we ever need to set up a second scissor box is for per-Widget segmented draws.
	//Account for cases where the DrawSize + Position (The left and bottom edge of the object) is less than the previous clipping box.
	if((DrawSize < Size))
	{
		bScissor = true;
		ScissorBox(Position, DrawSize);
	}

	Frame.TexOffset.X = Offset.X;
	Frame.TexOffset.Y = -Offset.Y;
	//Frame.TexOffset.Negate();
	Frame.DrawObject(Position);
	Buffer.bUpdate = false;
	bUpdate = false;
	if(bScissor)
		DisableScissor();
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

void Canvas::Tick(double DT)
{
	//Widget::Tick(DT);
	Vect2 OldPos = Position;
	Vect2 OldSize = DrawSize;
	Rotator OldRot = RotMatrix.Rot;
	if (bAnimating)
	{
		Buffer.bUpdate = true;
		Vect2 Destination = Keys[CurrentKey].Pos;
		Vect2 Source = Keys[CurrentKey - 1].Pos;
		Vect2 Delta = ( (Destination - Source) * (DT / Keys[CurrentKey].Duration));
		Vect2 AbsDelta = Delta;
		AbsDelta.ToAbs();
		if( (Destination - Position).ToAbs() < AbsDelta)
		{
			Delta = Destination - Position;
			if( (CurrentKey + 1) == NumKeys)
			{
				bAnimating = false;
				bUpdate = false;
				OnFinishedAnim.DelegateCallbacks(this);
			}
			else
				++CurrentKey;
		}
		Owner->GridSystem->MoveWidget(*this, Delta);
		//Currently rotations will cause minor collision glitches, but it's good enough for animations
		float DestinationRot = Keys[CurrentKey].Rot.Rot;
		float SourceRot = Keys[CurrentKey - 1].Rot.Rot;
		float DeltaRot = (DestinationRot - SourceRot) * (DT / Keys[CurrentKey].Duration);
		if( FAbs(RotMatrix.Rot.Rot - DestinationRot) < FAbs(DeltaRot))
		{
			DeltaRot = DestinationRot - RotMatrix.Rot.Rot;
			if( (CurrentKey + 1) == NumKeys)
			{
				bAnimating = false;
				bUpdate = false;
				OnFinishedAnim.DelegateCallbacks(this);
			}
			else
				++CurrentKey;
		}

		if (FAbs(DeltaRot) > 0.f)
			RotMatrix = Mat2(Rotator(DeltaRot + RotMatrix.Rot.Rot));
	}
	else if (bReturnAnim)
	{
		Buffer.bUpdate = true;
		Vect2 Destination = Keys[CurrentKey - 1].Pos;
		Vect2 Source = Keys[CurrentKey].Pos;
		Vect2 Delta = ( (Destination - Source) * (DT / Keys[CurrentKey].Duration));
		Vect2 AbsDelta = Delta;
		AbsDelta.ToAbs();
		if( (Destination - Position).ToAbs() < AbsDelta)
		{
			Delta = Destination - Position;
			if( CurrentKey == 1)
			{
				bReturnAnim = false;
				OnFinishedAnimReturn.DelegateCallbacks(this);
			}
			else
				--CurrentKey;
		}
		Owner->GridSystem->MoveWidget(*this, Delta);

		float DestinationRot = Keys[CurrentKey - 1].Rot.Rot;
		float SourceRot = Keys[CurrentKey].Rot.Rot;
		float DeltaRot = (DestinationRot - SourceRot) * (DT / Keys[CurrentKey].Duration);
		if( FAbs(RotMatrix.Rot.Rot - DestinationRot) < FAbs(DeltaRot))
		{
			DeltaRot = DestinationRot - RotMatrix.Rot.Rot;
			if( (CurrentKey + 1) == NumKeys)
			{
				bAnimating = false;
				bUpdate = false;
				OnFinishedAnim.DelegateCallbacks(this);
			}
			else
				++CurrentKey;
		}

		if (FAbs(DeltaRot) > 0.f)
			RotMatrix = Mat2(Rotator(DeltaRot + RotMatrix.Rot.Rot));
	}

	//Sizing
	//Horizontal
	if(bExpandVert && VerticalDuration > 0.f)
	{
		Buffer.bUpdate = true;
		Vect2 Delta;
		Delta.X	= ((Size.X - VertMin) * (DT / VerticalDuration));
		if(DrawSize.X + Delta.X >= Size.X)
		{
			//DrawSize.X = Size.X;
			Delta.X = Size.X - DrawSize.X;
			bExpandVert = false;
			OnVerticalExpanded.DelegateCallbacks(this);
		}
		Owner->GridSystem->ResizeWidget(*this, Delta);
	}
	else if(bCollapseVert && VerticalDuration > 0.f)
	{
		Buffer.bUpdate = true;
		Vect2 Delta;
		Delta.X -= ((Size.X - VertMin) * (DT / VerticalDuration));

		if(DrawSize.X + Delta.X <= VertMin)
		{
			Delta.X = VertMin - DrawSize.X;
			bCollapseVert = false;
			OnVerticalCollapsed.DelegateCallbacks(this);
		}
		Owner->GridSystem->ResizeWidget(*this, Delta);
	}
	//Vertical
	if(bExpandHor && HorizontalDuration > 0.f)
	{
		Buffer.bUpdate = true;
		Vect2 Delta;
		Delta.Y += ((Size.Y - HorMin) * (DT / HorizontalDuration));
		if(DrawSize.Y + Delta.Y >= Size.Y)
		{
			Delta.Y = Size.Y - DrawSize.Y;
			bExpandHor = false;
			OnHorizontalExpanded.DelegateCallbacks(this);
		}
		Owner->GridSystem->ResizeWidget(*this, Delta);

	}
	else if(bCollapseHor && HorizontalDuration > 0.f)
	{
		Buffer.bUpdate = true;
		Vect2 Delta;
		Delta.Y -= ((Size.Y - HorMin) * (DT / HorizontalDuration));
		if(DrawSize.Y + Delta.Y <= HorMin)
		{
			Delta.Y = HorMin - DrawSize.Y;
			bCollapseHor = false;
			OnHorizontalCollapsed.DelegateCallbacks(this);
		}
		Owner->GridSystem->ResizeWidget(*this, Delta);

	}

		//if (bUpdate /* && GridSystem->OwnerCanvas->ShouldDraw(this)*/)
		//	Draw();		//-- Avoid draw calls that are outside the canvas
	Buffer.Select();
		for (int i = 0; i < Items.size(); ++i)
			Items[i]->Tick(DT);
	Buffer.Restore();

	if(OldPos != Position || OldSize.X > DrawSize.X || OldSize.Y > DrawSize.Y)
	{
		//Calculate the largest rect
		Vect2 MinPos;
		MinPos.X = MIN(OldPos.X, Position.X) - 2.f;
		MinPos.Y = MIN(OldPos.Y, Position.Y) - 2.f;
		Vect2 FinalSize;
		FinalSize.X = MAX(OldPos.X + OldSize.X, Position.X + DrawSize.X) + 2.f;
		FinalSize.Y = MAX(OldPos.Y + OldSize.Y, Position.Y + DrawSize.Y) + 2.f;
		FinalSize -= MinPos;
		//if(Owner)
			Owner->GridSystem->UpdateSegment(*this, MinPos, FinalSize);
		//else
		//	GridSystem->UpdateSegment(*this, MinPos, FinalSize);
	}
	else if(RotMatrix.Rot.Rot != OldRot.Rot)
	{
		//Calculate largest bounding rect
		float MaxSize = MAX(Size.X, Size.Y);
		MaxSize = Vect2(MaxSize, MaxSize).VSize();
		Vect2 Origin = Position + (Size / 2);
		Vect2 Pos = Origin - (MaxSize / 2);
		Vect2 Sz = Vect2(MaxSize, MaxSize);

		//if(Owner)
			Owner->GridSystem->UpdateSegment(*this, Pos, Sz);
		//else
		//	GridSystem->UpdateSegment(*this, Pos, Sz);

	}
	else if (bUpdate || Buffer.bUpdate)
		Draw();
	
}

//Called from Parent canvas, used purely to draw the buffer itself. RenderSegment is done in BackgroundImage
void Canvas::SegmentRender(Vect2 Pos, Vect2 aSize)
{
	//Buffer.Select();
	//Widget::SegmentRender(Pos, aSize);
	//if (bUpdate)
	//	Draw();
	/*for(int i = 0; i < Items.size(); ++i)
	{
		if ( RectOverlap(Pos, Pos + Size, Items[i]->Position, Items[i]->Position + Items[i]->Size))
		{
			bool bUpdateState = Items[i]->bUpdate;
			Items[i]->bUpdate = true;
			Items[i]->SegmentRender(Pos, Size);
			if(!RectWithin(Items[i]->Position, Items[i]->Size, Pos, Size))
				Items[i]->bUpdate = bUpdateState;	//If we've only drawn this partially, restore previous update state
		}
	*/
	//Buffer.Restore();
	if (bUpdate)
		Draw();
	//Don't draw here, let it draw in tick.
	//if (Buffer.bUpdate)
	//{
		//Frame.TexOffset.X = Offset.X;
		//Frame.TexOffset.Y = -Offset.Y;
	//	Frame.DrawObject(Position);
	//	Buffer.bUpdate = false;
	//}
}
