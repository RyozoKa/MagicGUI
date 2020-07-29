#pragma once

#include "Image.h"
#include "ThemeManager.h"
#include "Label.h"

class MAGICGUIAPI Button : public Image
{
public:

	virtual void Tick(double dt)
	{
		Image::Tick(dt);
	}
	Button() : Image()
	{
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_DOWN), STATE_Pressed);
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_NORMAL), STATE_Normal);
		SetStateBackground(ThemeManager::GetTextureFor(C_BUTTON_HOVER), STATE_Hover);
		DrawType = TYPE::TYPE_TEXTURE;
		SetState(STATE_Normal);
		SetSize(Vect2(128.f, 32.f));
	}

	enum ButtonState
	{
		STATE_Normal,
		STATE_Pressed,
		STATE_Hover
	} CurrentState = STATE_Normal;

	class Texture* States[3] = { nullptr };
	Color ColoredStates[3];

	//Text
	Label Text;
	Vect2 TextOffset;
	virtual void SetText(String S, String FontName, int FontHeight);

	//Misc
	unsigned int Index = 0;

	//Control
	bool bToggleButton = false;		//-- Is toggle button?
	bool bClickLock = false;		//-- lock button when switched on
	bool bOn = false;				//-- Button state

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

	virtual void SetPosition(const Vect2 Pos);
	virtual void Attached()
	{
		AddItem(&Text);
	}
	
	virtual void SetSize(const Vect2 Sz)
	{
		Image::SetSize(Sz);
		TextOffset = (Size - Text.Size) / 2;
		Text.SetPosition(Position + TextOffset);
	}

	void SetState(ButtonState);

protected:

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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Image::Draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

};