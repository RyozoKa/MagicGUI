#include "Widget.h"
#include "Texture.h"
#include "Gridsubsystem.h"
#include "Window.h"
#include <cstdio>

unsigned int GZindex = 0;

void Widget::Tick(double DT)
{
	//We won't get here unless we have to update
	//Save current state
	Vect2 OldPos = Position;
	Vect2 OldSize = DrawSize;
	Rotator OldRot = RotMatrix.Rot;
	if (bAnimating)
	{
		bUpdate = true;
		Vect2 Destination = Keys[CurrentKey].Pos;
		Vect2 Source = Keys[CurrentKey - 1].Pos;
		if (Destination != Source)
		{
			Vect2 Delta = ((Destination - Source) * (DT / Keys[CurrentKey].Duration));
			Vect2 AbsDelta = Delta;
			AbsDelta.ToAbs();
			if ((Destination - Position).ToAbs() < AbsDelta)
			{
				Delta = Destination - Position;
				if ((CurrentKey + 1) == NumKeys)
				{
					bAnimating = false;
					bUpdate = false;
					OnFinishedAnim.DelegateCallbacks(this);
				}
				else
					++CurrentKey;
			}
			GridSystem->MoveWidget(*this, Delta);
		}
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
		bUpdate = true;
		Vect2 Destination = Keys[CurrentKey - 1].Pos;
		Vect2 Source = Keys[CurrentKey].Pos;
		if (Destination != Source)
		{
			Vect2 Delta = ((Destination - Source) * (DT / Keys[CurrentKey].Duration));
			Vect2 AbsDelta = Delta;
			AbsDelta.ToAbs();
			if ((Destination - Position).ToAbs() < AbsDelta)
			{
				Delta = Destination - Position;
				if (CurrentKey == 1)
				{
					bReturnAnim = false;
					bUpdate = false;
					OnFinishedAnimReturn.DelegateCallbacks(this);
				}
				else
					--CurrentKey;
			}
			GridSystem->MoveWidget(*this, Delta);
		}
		float DestinationRot = Keys[CurrentKey - 1].Rot.Rot;
		float SourceRot = Keys[CurrentKey].Rot.Rot;
		float DeltaRot = (DestinationRot - SourceRot) * (DT / Keys[CurrentKey].Duration);
		if( FAbs(RotMatrix.Rot.Rot - DestinationRot) < FAbs(DeltaRot))
		{
			DeltaRot = DestinationRot - RotMatrix.Rot.Rot;
			if( CurrentKey == 1)
			{
				bReturnAnim = false;
				bUpdate = false;
				OnFinishedAnimReturn.DelegateCallbacks(this);
			}
			else
				--CurrentKey;
		}

		if (FAbs(DeltaRot) > 0.f)
			RotMatrix = Mat2(Rotator(DeltaRot + RotMatrix.Rot.Rot));
	}

	//Sizing
	//Horizontal
	if(bExpandVert && VerticalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.X	= ((Size.X - VertMin) * (DT / VerticalDuration));
		if(DrawSize.X + Delta.X >= Size.X)
		{
			//DrawSize.X = Size.X;
			Delta.X = Size.X - DrawSize.X;
			bExpandVert = false;
			OnVerticalExpanded.DelegateCallbacks(this);
		}
		GridSystem->ResizeWidget(*this, Delta);
	}
	else if(bCollapseVert && VerticalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.X -= ((Size.X - VertMin) * (DT / VerticalDuration));
		if(DrawSize.X + Delta.X <= VertMin)
		{
			Delta.X = VertMin - DrawSize.X;
			bCollapseVert = false;
			OnVerticalCollapsed.DelegateCallbacks(this);
		}
		GridSystem->ResizeWidget(*this, Delta);
	}
	//Vertical
	if(bExpandHor && HorizontalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.Y += ((Size.Y - HorMin) * (DT / HorizontalDuration));
		if(DrawSize.Y + Delta.Y >= Size.Y)
		{
			Delta.Y = Size.Y - DrawSize.Y;
			bExpandHor = false;
			OnHorizontalExpanded.DelegateCallbacks(this);
		}
		GridSystem->ResizeWidget(*this, Delta);
	}
	else if(bCollapseHor && HorizontalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.Y -= ((Size.Y - HorMin) * (DT / HorizontalDuration));
		if(DrawSize.Y + Delta.Y <= HorMin)
		{
			Delta.Y = HorMin - DrawSize.Y;
			bCollapseHor = false;
			OnHorizontalCollapsed.DelegateCallbacks(this);
		}
		GridSystem->ResizeWidget(*this, Delta);
	}

	//Update old segment if necessary spanning over the old + new frame to avoid unnecessary calls.
	//This will take care of all Z indexing too
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
		GridSystem->UpdateSegment(*this, MinPos, FinalSize);
	}
	else if(RotMatrix.Rot.Rot != OldRot.Rot)
	{
		//Calculate largest bounding rect
		float MaxSize = MAX(Size.X, Size.Y);
		MaxSize = Vect2(MaxSize, MaxSize).VSize();
		Vect2 Origin = Position + (Size / 2);
		Vect2 Pos = Origin - (MaxSize / 2);
		Vect2 Sz = Vect2(MaxSize, MaxSize);
		GridSystem->UpdateSegment(*this, Pos, Sz);

	}
	else if (bUpdate /* && GridSystem->OwnerCanvas->ShouldDraw(this)*/)
		Draw();		//-- Avoid draw calls that are outside the canvas

	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Tick(DT);
}


void Widget::AddKeypoint(Keypoint Point)
{
	Keys[NumKeys++] = Point;
}

bool Widget::TestCollision(const Vect2& Pos)
{
	if (bHidden || !bEnabled || !bCollide)
		return false;
	switch (CollisionType)
	{
		case ECollision_Radi:
		{
			//Radius collision test
			//Find the center of the collision circle.
			Vect2 Center = Position + (DrawSize / 2.f);
			Vect2 Distance = Center - Pos;
			if (Distance.VSize() <= Radius)
				return true;
			else
				return false;
		}
		break;
		case ECollision_Rect:
		{
			Vect2 End = Position + DrawSize;
			//Simple rectangular collision primitive
			if (Pos.X >= Position.X && Pos.Y >= Position.Y && Pos.X <= End.X && Pos.Y <= End.Y)
				return true;
			else
				return false;
		}
		break;
	}
	return false;
}

void Widget::Initialize()
{
	if (bStartHorCollapsed)
		DrawSize.Y = HorMin;
	if (bStartVertCollapsed)
		DrawSize.X = VertMin;

	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Initialize();
}

void Widget::AddItem(Widget* W)
{
	Items.push_back(W);
	W->GridSystem = GridSystem;
	W->Owner = this;
	W->Window = Window;
	W->ZIndex = GZindex++;
	GridSystem->InsertWidget(*W);
	W->Attached();
}

void Widget::SetExpansion(int E)
{
	Exp = E;
}

void Widget::SetSize(const Vect2 Sz)
{
	//Owner might still be null if not yet attached.
	if (Owner && DrawSize != Vect2(-1.f, -1.f) && DrawSize.GetBlock() != Sz.GetBlock() && Owner->GridSystem)
	{
		Owner->GridSystem->RemoveWidget(*this);
		
		//We need to respect the DrawSize configurations!!!
		if(DrawSize.X == Size.X)
			DrawSize.X = Sz.X;
		if(DrawSize.Y == Size.Y)
			DrawSize.Y = Sz.Y;
		Size = Sz;

		Owner->GridSystem->InsertWidget(*this);
	}
	else
	{
		if(DrawSize.X == Size.X)
			DrawSize.X = Sz.X;
		if(DrawSize.Y == Size.Y)
			DrawSize.Y = Sz.Y;
		Size = Sz;
	}

	OnDrawChanged.DelegateCallbacks(Position, Sz);
	Update();	//Update all the children
}

void Widget::SetRotation(const Rotator Rot)
{
	Keys[0].Rot = Rotation = Rot;
	RotMatrix = Mat2(Rot);
}

void Widget::SetRadius(const float F)
{
	Radius = F;
	CollisionType = ECollision_Radi;
}

void Widget::SetPosition(const Vect2 Pos)
{
	//if(bAnimating)
	//	return;

	//Move widget if necessary
	if (Pos < 0.f)
		return;
	if (Owner && Position.GetBlock() != Pos.GetBlock() && Owner->GridSystem)
	{
		Owner->GridSystem->RemoveWidget(*this);
		Position = Pos;
		Owner->GridSystem->InsertWidget(*this);
	}
	else
		Position = Pos;
	OnDrawChanged.DelegateCallbacks(Pos, Size);
	Keys[0].Pos = Pos;
	Keys[0].Rot.Rot = 0.f;
	Update();
}

void Widget::Move(const Vect2 Offset)
{
	Position += Offset;
	bUpdate = true;
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Move(Offset);
}


void Widget::Draw()
{
	if (bHidden)
		return;
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
	//Draw objects if visible
		RenderObjects();

	if(bScissor)
		DisableScissor();
	bUpdate = false;
}

void Widget::Update()
{
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Update();

	bUpdate = true;
	//printf("Updated\n");
}
void Widget::Show()
{
	bHidden = false;
	bUpdate = true;
}

void Widget::Hide()
{
	bHidden = true;
	SegmentRender(Position, Size);
}

void Widget::SegmentRender(Vect2 Pos, Vect2 Size)
	{
		if (bUpdate)
			Draw();
		for(int i = 0; i < Items.size(); ++i)
		{
			if (/*Items[i]->bUpdate &&*/ RectOverlap(Pos, Pos + Size, Items[i]->Position, Items[i]->Position + Items[i]->Size))
			{
				bool bUpdateState = Items[i]->bUpdate;
				Items[i]->bUpdate = true;
				Items[i]->SegmentRender(Pos, Size);
				if(!RectWithin(Items[i]->Position, Items[i]->Size, Pos, Size))
					Items[i]->bUpdate = bUpdateState;	//If we've only drawn this partially, restore previous update state
			}
		}
	}
//These don't do anything by default.
void Widget::OnMouseEnter(float X, float Y)
{}

void Widget::OnMouseLeave(float X, float Y)
{}

void Widget::OnMouseLeftClick(float X, float Y)
{}

void Widget::OnMouseRightClick(float X, float Y)
{}

void Widget::OnMouseLeftReleased(float X, float Y)
{}

void Widget::OnMouseRightReleased(float X, float Y)
{}

void Widget::OnKeyPressed(int Key, int Mod)
{}

void Widget::OnScroll(float YOffset)
{}

void Widget::OnCursor(double X, double Y)
{
}

void Widget::Attached()
{
}

void Widget::OnWindowResize(Vect2 Delta)
{
	Vect2 ADelta = Delta;
	//Take care of top left first
	Vect2 FinalDelta;
	Vect2 FinalPos;
	bool bUpdated = false;
	if(Exp & EXPAND_UP)
	{
		//SetPosition(Position - Vect2(0.f, (Delta.Y / 2)));
		FinalPos += Vect2(0.f, (Delta.Y / 2));
		Delta.Y /= 2;
		FinalDelta += Vect2(0.f, Delta.Y);
		//SetSize(Size + Vect2(0.f, Delta.Y));
		bUpdated = true;

	}
	if(Exp & EXPAND_LEFT)
	{
		//SetPosition(Position - Vect2((Delta.X / 2), 0.f));
		FinalPos += Vect2((Delta.X / 2), 0.f);
		Delta.X /= 2;
		FinalDelta += Vect2(Delta.X, 0.f);
		//SetSize(Size + Vect2(Delta.X, 0.f));
		bUpdated = true;
	}
	if(Exp & EXPAND_DOWN)
	{
		FinalDelta += Vect2(0.f, Delta.Y);
		//SetSize(Size + Vect2(0.f, Delta.Y));

		bUpdated = true;
		//Update();
	}
	if (Exp & EXPAND_RIGHT)
	{
		FinalDelta += Vect2(Delta.X, 0.f);
		//SetSize(Size + Vect2(Delta.X, 0.f));
		bUpdated = true;
	}

	if(!FinalDelta.IsEmpty())
		SetSize(Size + FinalDelta);

	if (!FinalPos.IsEmpty())
		SetPosition(Position - FinalPos);

	if (bUpdated)
	{
		Update();
		//OnResize.DelegateCallbacks(ADelta);
		OnDrawChanged.DelegateCallbacks(Position, Size);
	}

	for (int i = 0; i < Items.size(); ++i)
	{
		Items[i]->OnWindowResize(ADelta);
	}
}
