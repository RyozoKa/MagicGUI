
#include "Button.h"

void Button::SetStateBackground(Texture* T, ButtonState S)
{
	States[S] = T;
	//DrawType |= TYPE::TYPE_TEXTURE;
}

void Button::SetStateColor(Color C, ButtonState S)
{
	ColoredStates[S] = C;
	//DrawType |= TYPE::TYPE_COLOR;
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
}

void Button::OnMouseLeave(float X, float Y)
{
	SetState(ButtonState::STATE_Normal);
}

void Button::OnMouseLeftClick(float X, float Y)
{
	SetState(ButtonState::STATE_Pressed);
}

void Button::OnMouseRightClick(float X, float Y)
{
	SetState(ButtonState::STATE_Pressed);
}

void Button::OnMouseLeftReleased(float X, float Y)
{
	SetState(ButtonState::STATE_Hover);
	OnClick.DelegateCallbacks();
}

void Button::OnMouseRightReleased(float X, float Y)
{
	SetState(ButtonState::STATE_Hover);
	OnRightClick.DelegateCallbacks();
}

void Button::OnKeyPressed(int Key, int Mod)
{
}

void Button::OnScroll(float YOffset)
{
}
