#pragma once

#include "Image.h"

class MAGICGUIAPI Button : public Image
{
public:

	Button() : Image()
	{
		
	}

	enum ButtonState
	{
		STATE_Normal,
		STATE_Pressed,
		STATE_Hover
	} CurrentState = STATE_Normal;

	class Texture* States[3];
	Color ColoredStates[3];
	
	ButtonState GetCurrentState()
	{
		return CurrentState; 
	}

	void SetStateBackground(Texture*, ButtonState);
	void SetStateColor(Color&, ButtonState);

private:
	
	void SetState(ButtonState);

	//Widget interface
	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnKeyPressed(int Key, int Mod);
	virtual void OnScroll(float YOffset);

};