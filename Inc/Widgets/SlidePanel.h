#pragma once

/*
	A widget that expands horizontally or vertically with buttons and icons
	Revision:
		Christopher Békési
*/

#include "Canvas.h"
#include "Button.h"

class MAGICGUIAPI SlidePanel : public Widget
{
public:
	Canvas* C;
	Button* ToggleButton;
	virtual void AddItem(Widget*);

	SlidePanel() : Widget()
	{
		C = new Canvas;
		ToggleButton = new Button;
		//C->Owner = this;

		C->VertMin = 64.f;
		C->VerticalDuration = 0.3f;
		C->bStartVertCollapsed = true;
		ToggleButton->SetSize(Vect2(64.f, 64.f));
		ToggleButton->SetPosition(Vect2(0.f, 0.f));
		ToggleButton->SetStateBackground(ThemeManager::GetTextureFor(C_EXPAND_ICON_CLICK), Button::STATE_Pressed);
		ToggleButton->SetStateBackground(ThemeManager::GetTextureFor(C_EXPAND_ICON), Button::STATE_Normal);
		ToggleButton->SetStateBackground(ThemeManager::GetTextureFor(C_EXPAND_ICON_HOVER), Button::STATE_Hover);
		//ToggleButton->SetType(TYPE::TYPE_TEXTURE);
		//ToggleButton->SetStateColor(Color(167, 171, 168), Button::STATE_Pressed);
		//ToggleButton->SetStateColor(Color(0, 0, 0), Button::STATE_Normal);
		//ToggleButton->SetStateColor(Color(255, 255, 255), Button::STATE_Hover);
		ToggleButton->OnClick += ClassDelegate((void*)&SlidePanel::Toggle, this);
		ToggleButton->SetRotation(Rotator(0.f));
		ToggleButton->bTransparent = true;
		Keypoint KP;
		KP.Pos = ToggleButton->Position;
		KP.Rot.Rot = 180.f;
		KP.Duration = 0.3f;

		ToggleButton->AddKeypoint(KP);
		//ToggleButton->SetText("What", "Fonts/arial.ttf", 14);
		//ToggleButton->Text.SetTextColor(Color(255, 0, 0));
		C->SetColor(ThemeManager::CanvasWhite);
		
	}

	virtual bool TestCollision(const Vect2& Pos)
	{
		return false;
	}

	~SlidePanel()
	{
		delete C;
		delete ToggleButton;
	}

	virtual void Attached()
	{
		Widget::AddItem(C);
		C->AddItem(ToggleButton);
	}

	virtual void SetPosition(const Vect2 Pos)
	{
		Widget::SetPosition(Pos);
		C->SetPosition(Pos);
	}

	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		C->SetSize(Sz);
	}

	virtual void Toggle();

	virtual void RenderObjects()
	{
	}

	virtual void Tick(double DT)
	{
		Widget::Tick(DT);
		DrawSize = C->DrawSize;
	}
};