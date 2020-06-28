#pragma once

#include "Image.h"
#include "ThemeManager.h"

class MAGICGUIAPI Button : public Image
{
public:

	Button() : Image()
	{
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_DOWN), STATE_Pressed);
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_NORMAL), STATE_Normal);
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_HOVER), STATE_Hover);
		DrawType = TYPE::TYPE_TEXTURE;
		SetState(STATE_Normal);
	}

	enum ButtonState
	{
		STATE_Invalid,
		STATE_Normal,
		STATE_Pressed,
		STATE_Hover
	} CurrentState = STATE_Invalid;

	class Texture* States[3];
	Color ColoredStates[3];

	//Callback events
	Callbacks<Widget*> OnClick;
	Callbacks<Widget*> OnRightClick;
	Callbacks<Widget*> OnRelease;
	Callbacks<Widget*> OnRightRelease;

	ButtonState GetCurrentState()
	{
		return CurrentState; 
	}

	void SetStateBackground(Texture*, ButtonState);
	void SetStateColor(Color, ButtonState);


protected:
	
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

	//These should be private. To set button colors, user the SetState functions above.
	virtual void SetColor(Color C);
	virtual void SetImage(Texture* T);
	virtual void Draw()
	{
		Image::Draw();
	}

};