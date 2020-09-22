
#include "Button.h"

void Button::SetText(String S, String FontName, int FontHeight)
{
	Text.SetText(S, FontName, FontHeight);
	if (Text.Size + 10.f > Size)
	{
		Vect2 NewSize = Size;
		if (Text.Size.X + 20.f > Size.X)
			NewSize.X = Text.Size.X + 20.f;
		if (Text.Size.Y + 10.f > Size.Y)
			NewSize.Y = Text.Size.Y + 10.f;
		SetSize(NewSize);
	}
		
	//Calculate text offset
	TextOffset = (Size - Text.Size) / 2;
	Text.SetPosition(Position + TextOffset);
}

void Button::SetStateBackground(Texture* T, ButtonState S)
{
	States[S] = T;
	if (S == CurrentState)
	{
		if(T)
			SetImage(T);
		else
		{
			DrawType &= ~TYPE::TYPE_TEXTURE;
			ImgObj.RenderMode &= ~TYPE::TYPE_TEXTURE;
		}

	}
}

void Button::SetStateColor(Color C, ButtonState S)
{
	ColoredStates[S] = C;
	if (S == CurrentState && C.Alpha > 0)
		SetColor(C);
}

void Button::SetState(ButtonState S)
{
	if (CurrentState == S)
		return;
	CurrentState = S;
	SetType(TYPE::TYPE_NONE);
	if( States[S]  )
		SetImage( States[S] );
	if (ColoredStates[S].Alpha > 0)
		SetColor( ColoredStates[S] );
	Text.bUpdate = true;
}

void Button::OnMouseEnter(float X, float Y)
{
	SetState(ButtonState::STATE_Hover);
}

void Button::OnMouseLeave(float X, float Y)
{
	if(bToggleButton && bOn)
		SetState(ButtonState::STATE_Pressed);
	else
		SetState(ButtonState::STATE_Normal);
}

void Button::OnMouseLeftClick(float X, float Y)
{
	if(!bClickLock || !bOn)
		bOn = !bOn;
	SetState(ButtonState::STATE_Pressed);
	OnClick.DelegateCallbacks(this);
}

void Button::OnMouseRightClick(float X, float Y)
{
	SetState(ButtonState::STATE_Pressed);
	OnRightClick.DelegateCallbacks(this);
}

void Button::OnMouseLeftReleased(float X, float Y)
{
	if(bToggleButton)
	{
		if(!bOn)
			SetState(ButtonState::STATE_Hover);
	}
	else
		SetState(ButtonState::STATE_Hover);
	if(!bClickLock || bOn)
		OnRelease.DelegateCallbacks(this);
}

void Button::OnMouseRightReleased(float X, float Y)
{
	SetState(ButtonState::STATE_Hover);
	OnRightRelease.DelegateCallbacks(this);
}

void Button::OnKeyPressed(int Key, int Mod)
{
}

void Button::OnScroll(float YOffset)
{
}

void Button::SetColor(Color C)
{
	Image::SetColor(C);
}

void Button::SetImage(Texture* T)
{
	Image::SetImage(T);
}

void Button::SetPosition(const Vect2 Pos)
{
	Text.SetPosition(Position + TextOffset);
	Image::SetPosition(Pos);
}
