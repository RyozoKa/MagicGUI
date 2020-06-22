#include "Widget.h"
#include "Texture.h"
#include "Gridsubsystem.h"
#include "Window.h"
#include <cstdio>

static unsigned int GZindex = 1;

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
		Vect2 MinPos = Vect2::Min(OldPos, Position);
		Vect2 FinalSize = (Position - OldPos).ToAbs() + Vect2::Max(OldSize, DrawSize);
		GridSystem->UpdateSegment(*this, MinPos, FinalSize);
	}
	else if (bUpdate)
		Draw();

	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Tick(DT);
}


void Widget::AddKeypoint(Keypoint Point)
{
	Keys[NumKeys++] = Point;
}

bool Widget::TestCollision(const Vect2& Pos)
{
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
	Position = Pos;
	Keys[0].Pos = Pos;
	bUpdate = true;
}


void Widget::Draw()
{
	//Perform segmented drawing if needed
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
		Vect2 FinalPos = Position;
		Vect2 FinalSize = DrawSize;
		if(bScissorState)
		{ 
			FinalPos = Vect2::Max(FinalPos, ScissorPos);
			FinalSize = Vect2::Min(Position + DrawSize, ScissorPos + ScissorSize);
			FinalSize -= FinalPos;
			FinalSize.X -= 0.01;	//Compensate for weird rounding error in the scissor box.
		}
		if (FinalSize <= 0.f)
			return;	//Don't draw nulls. if you pass a negative delta to glScissor it will revert to the previous box, causing artifacts
		glScissor(FinalPos.X , (Window->WindowSize.Y - FinalPos.Y ) - FinalSize.Y, FinalSize.X, FinalSize.Y);
		glEnable(GL_SCISSOR_TEST);
	}
	//Draw objects
	RenderObjects();

	if(bScissor)
	{
		if (!bScissorState)	//No previous scissor
			glDisable(GL_SCISSOR_TEST);
		else
			glScissor(ScissorPos.X, (Window->WindowSize.Y - ScissorPos.Y) - ScissorSize.Y, ScissorSize.X, ScissorSize.Y);
	}
	bUpdate = false;
}

void Widget::Update()
{
	for (int i = 0; i < Items.size(); ++i)
		Items[i]->Update();

	bUpdate = true;
	//printf("Updated\n");
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