
#include "Button.h"

void Button::SetStateBackground(Texture* T, ButtonState S)
{
	States[S] = T;
	if (T && S == CurrentState)
		SetImage(T);
}

void Button::SetStateColor(Color C, ButtonState S)
{
	ColoredStates[S] = C;
	if (S == CurrentState)
		SetColor(C);
}

void Button::SetState(ButtonState S)
{
	if (CurrentState == S)
		return;
	CurrentState = S;
	if( States[S] && (DrawType & TYPE::TYPE_TEXTURE) )
		SetImage( States[S] );
	if (DrawType & TYPE::TYPE_COLOR)
		SetColor( ColoredStates[S] );
}

void Button::OnMouseEnter(float X, float Y)
{
	SetState(ButtonState::STATE_Hover);
	printf("Entered\n");
}

void Button::OnMouseLeave(float X, float Y)
{
	SetState(ButtonState::STATE_Normal);
	printf("Left\n");
}

void Button::OnMouseLeftClick(float X, float Y)
{
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
	SetState(ButtonState::STATE_Hover);
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
