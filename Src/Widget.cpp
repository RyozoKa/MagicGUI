#include "Widget.h"
#include "Texture.h"
#include "Gridsubsystem.h"
#include "Window.h"
#include <cstdio>

unsigned int GZindex = 1;

void Widget::Tick(double DT)
{
	//We won't get here unless we have to update
	//Save current state
	Vect2 OldPos = Position;
	Vect2 OldSize = DrawSize;
	if (bAnimating)
	{
		bUpdate = true;
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
		GridSystem->MoveWidget(*this, Delta);
	}
	else if (bReturnAnim)
	{
		bUpdate = true;
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
		GridSystem->MoveWidget(*this, Delta);
	}

	//Sizing
	//Horizontal
	if(bExpandVert && VerticalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.X	= ((Size.X - VertMin) * (DT / VerticalDuration));
		if(DrawSize.X >= Size.X)
		{
			DrawSize.X = Size.X;
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
		GridSystem->ResizeWidget(*this, Delta);
		if(DrawSize.X <= VertMin)
		{
			DrawSize.X = Size.X;
			bCollapseVert = false;
			OnVerticalCollapsed.DelegateCallbacks(this);
		}
	}
	//Vertical
	if(bExpandHor && HorizontalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.Y += ((Size.Y - HorMin) * (DT / HorizontalDuration));
		GridSystem->ResizeWidget(*this, Delta);
		if(DrawSize.Y >= Size.Y)
		{
			DrawSize.Y = Size.Y;
			bExpandHor = false;
			OnHorizontalExpanded.DelegateCallbacks(this);
		}
	}
	else if(bCollapseHor && HorizontalDuration > 0.f)
	{
		bUpdate = true;
		Vect2 Delta;
		Delta.Y -= ((Size.Y - HorMin) * (DT / HorizontalDuration));
		GridSystem->ResizeWidget(*this, Delta);
		if(DrawSize.Y <= HorMin)
		{
			DrawSize.Y = Size.Y;
			bCollapseHor = false;
			OnHorizontalCollapsed.DelegateCallbacks(this);
		}
	}

	//Update old segment if necessary spanning over the old + new frame to avoid unnecessary calls.
	//This will take care of all Z indexing too
	if(OldPos != Position || OldSize > DrawSize)
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
	else if (bUpdate && GridSystem->OwnerCanvas->ShouldDraw(this))
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

void Widget::SetSize(const Vect2 Sz)
{
	Size = Sz;
	DrawSize = Sz;
	Update();	//Update all the children
}

void Widget::SetRotation(const Rotator Rot)
{
	Rotation = Rot;
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
	if (Position != Vect2(-1.f, -1.f) && Position.GetBlock() != Pos.GetBlock() && GridSystem)
	{
		GridSystem->RemoveWidget(*this);
		Position = Pos;
		GridSystem->InsertWidget(*this);
	}
	else
		Position = Pos;

	Keys[0].Pos = Pos;
	bUpdate = true;
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
	if (!bHidden)
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
