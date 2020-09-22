/*
	A tabbed panel with support for multiple inner panels
	Revision:
		Christopher Békési 2020-06-29
*/

#pragma once

#include "Button.h"
#include "HDynamicArray.h"
#include "RenderBuffer.h"
#include "Canvas.h"
#include "CallbackChain.h"

class MAGICGUIAPI TabPage
{
public:

	void AddItem(Widget* W);

	TabPage()
	{
		C->SetColor(Color(252, 255, 219));
		C->bHidden = true;
	}

	Button TabButton;
	bool bUpdate = true;
	int Index;
	class TabPanel* Owner;
	Canvas* C = new Canvas;

	void SetText(String S, String FontName, int FontHeight);

private:

	void SetSize(const Vect2 Size);
	void SetPosition(const Vect2 Pos);
	void Tick(double DT);
	void SegmentRender(Vect2 Pos, Vect2 Size);
	
	friend class TabPanel;
};

class MAGICGUIAPI TabPanel : public Widget
{
public:
	TabPanel()
	{
		bUpdate = true;
		ImageStrip.Owner = this;
		TabCanvas.Owner = this;
		TabCanvas.SetColor(Color(79, 79, 78));
		TabCanvas.SetRenderMode(TYPE::TYPE_COLOR);
		ImageStrip.SetImage(ThemeManager::GetTextureFor(C_BUTTON_NORMAL));
	}

	virtual TabPage* AddTab();
	virtual void SelectTab(int Index);

	virtual void Tick(double DT);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);
	virtual void SetSize(const Vect2 Sz);
	virtual void SetPosition(const Vect2 Pos);
	virtual void RenderObjects();
	virtual void SetTabVisibility(bool);
	virtual TabPage* GetCurrentTab();

	int GetCount();

	//Click callback
	void OnTabButton(Widget*);

	const float DefaultTabStripHeight = 32.f;
	float TabStripHeight = DefaultTabStripHeight;
	bool bShowTabs = true;
	bool bShowFullStrip = false;
	//Events
	Callbacks<Widget*> OnTabChanged;
	float XOffset = 0;

private:
	
	HDynamicArray<TabPage*> Tabs;
	RenderObject TabCanvas;		//-- Actual background
	RenderObject ImageStrip;	//-- Menu bar background masking
	int CurrentTab = -1;
	

};