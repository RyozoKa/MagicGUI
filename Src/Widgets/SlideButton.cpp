
#include "SlideButton.h"
#include "Window.h"

void SlideButton::OnMouseEnter(float X, float Y)
{
	Button::OnMouseEnter(X, Y);
}

void SlideButton::OnMouseLeave(float X, float Y)
{
	if(!bHolding)
		Button::OnMouseLeave(X, Y);
	//bHolding = false;
}

void SlideButton::OnMouseLeftClick(float X, float Y)
{
	Button::OnMouseLeftClick(X, Y);
	bHolding = true;
	LastCursor = Window->CursorPos;
}

void SlideButton::OnMouseLeftReleased(float X, float Y)
{
	Button::OnMouseLeftReleased(X, Y);
	SetState(ButtonState::STATE_Normal);
	bHolding = false;
}

void SlideButton::OnScroll(float YOffset)
{
	//if(bHolding) 
	YOffset = -YOffset * 10.f;
	if(bVertical)
	{
		if ( (Position.Y + YOffset) > SlideStop)
			YOffset = SlideStop - Position.Y;

		else if( (Position.Y + YOffset) < Start.Y)
			YOffset = -(Position.Y - Start.Y);

		if (YOffset == 0.f)
				return;
		GridSystem->MoveWidget(*this, { 0.f, YOffset });
		bUpdate = true;
		//GridSystem->UpdateSegment(*this, Start, { Size.X, SlideStop - Start.Y + Size.Y} );
	}
	else
	{
		if ( (Position.X + YOffset) > SlideStop)
			YOffset = SlideStop - Position.X;

		else if( (Position.X + YOffset) < Start.X)
			YOffset = -(Position.X - Start.X);

		if (YOffset == 0.f)
				return;
		GridSystem->MoveWidget(*this, { YOffset, 0.f });
		bUpdate = true;
		//GridSystem->UpdateSegment(*this, Start, { SlideStop - Start.X + Size.X, Size.Y } );
	}
	OnSlide.DelegateCallbacks(this, -YOffset);
}

void SlideButton::Tick(double DT)
{
	
	if(bHolding && LastCursor != Window->CursorPos) 
	{
		float Offset;
		if(bVertical)
		{
			Offset = Window->CursorPos.Y - LastCursor.Y;
			if ( (Position.Y + Offset) > SlideStop)
				Offset = SlideStop - Position.Y;

			else if( (Position.Y + Offset) < Start.Y)
				Offset = -(Position.Y - Start.Y);

			if (Offset == 0.f)
				return;
			GridSystem->MoveWidget(*this, { 0.f, Offset });
			bUpdate = true;
			//GridSystem->UpdateSegment(*this, Start, { Size.X, SlideStop - Start.Y + Size.Y} );
		}
		else
		{
			Offset = Window->CursorPos.X - LastCursor.X;
			if ( (Position.X + Offset) > SlideStop)
				Offset = SlideStop - Position.X;

			else if( (Position.X + Offset) < Start.X)
				Offset = -(Position.X - Start.X);

			if (Offset == 0.f)
					return;
			GridSystem->MoveWidget(*this, { Offset, 0.f });
			bUpdate = true;
			//GridSystem->UpdateSegment(*this, Start, { SlideStop - Start.X + Size.X, Size.Y } );
		}
		
		LastCursor = Window->CursorPos;
		OnSlide.DelegateCallbacks(this, -Offset);
	}

	if (bUpdate)
	{
		if (bVertical)
			GridSystem->UpdateSegment(*this, Start, { Size.X, SlideStop - Start.Y + Size.Y} );
		else
			GridSystem->UpdateSegment(*this, Start, { SlideStop - Start.X + Size.X, Size.Y } );
	}
	Button::Tick(DT);
}