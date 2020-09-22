
#include "TabPanel.h"
#include "Window.h"


extern unsigned int GZindex;

void TabPanel::SetSize(const Vect2 Sz)
{
	Widget::SetSize(Sz);
	TabCanvas.SetPrimitive(Sz);
	ImageStrip.SetPrimitive(Vect2(Sz.X, 32.f));
	for (int i = 0; i < Tabs.Size(); ++i)
		Tabs[i]->SetSize(Sz - Vect2(0.f, TabStripHeight));
}

void TabPanel::SetPosition(const Vect2 Pos)
{
	Widget::SetPosition(Pos);
	for (int i = 0; i < Tabs.Size(); ++i)
		Tabs[i]->TabButton.SetPosition(Position + Vect2(Tabs[i]->Index * 128.f, 0.f));

	for (int i = 0; i < Tabs.Size(); ++i)
		Tabs[i]->SetPosition(Pos + Vect2(1.f, TabStripHeight));
}

void TabPanel::RenderObjects()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	TabCanvas.DrawObject(Position);
	if (bShowFullStrip)
		ImageStrip.DrawObject(Position);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void TabPanel::SetTabVisibility(bool bVisible)
{
	if(bVisible && !bShowTabs)
	{
		TabStripHeight = DefaultTabStripHeight;
		SetSize(Size);
		bUpdate = true;
	}
	else if(!bVisible && bShowTabs)
	{
		TabStripHeight = 0.f;
		SetSize(Size);
		bUpdate = true;
	}
}

TabPage* TabPanel::GetCurrentTab()
{
	return Tabs[CurrentTab];
}

int TabPanel::GetCount()
{
	return Tabs.Size();
}

void TabPanel::OnTabButton(Widget* W)
{
	SelectTab(((Button*)W)->Index);
}

void TabPanel::Tick(double DT)
{
	//if (CurrentTab >= 0)
	//{
	//	Tabs[CurrentTab]->Tick(DT);
		//bUpdate = Tabs[CurrentTab]->bUpdate;
	//}
	Widget::Tick(DT);
	//if (bUpdate)
	//	Draw();
	//if(CurrentTab != -1)
	//	Tabs[CurrentTab]->Tick(DT);
	//for (int i = 0; i < Tabs.Size(); ++i)
	//	Tabs[i]->TabButton.Tick(DT);
}

void TabPanel::SegmentRender(Vect2 Pos, Vect2 Size)
{
	//if (CurrentTab >= 0)
	//{
	//	Tabs[CurrentTab]->SegmentRender(Pos, Size);
	//	bUpdate = Tabs[CurrentTab]->bUpdate;
	//}
	//if(CurrentTab != -1)
	//	Tabs[CurrentTab]->SegmentRender(Pos, Size);
	Widget::SegmentRender(Pos, Size);
	//for (int i = 0; i < Tabs.Size(); ++i)
	//	Tabs[i]->TabButton.SegmentRender(Pos, Size);
}

TabPage* TabPanel::AddTab()
{
	TabPage* Tab = new TabPage;
	Tab->TabButton.Index = Tab->Index = Tabs.Size();
	Tabs.Insert(Tab);
	Tab->SetSize(Size - Vect2(0.f, TabStripHeight));
	Tab->TabButton.OnClick += ClassDelegate((void*)&TabPanel::OnTabButton, this);
	Tab->TabButton.bToggleButton = true;
	Tab->TabButton.bClickLock = true;
	Tab->Owner = this;
	Tab->TabButton.SetSize(Vect2(128.f, TabStripHeight));
	Tab->TabButton.SetPosition(Position + Vect2(XOffset, 0.f));
	XOffset += 128.f;
	Tab->SetPosition(Position + Vect2(0.f, TabStripHeight));
	//Very important to call the Attached event
	Tab->C->Attached();
	Widget::AddItem(&Tab->TabButton);
	Widget::AddItem(Tab->C);
	if (CurrentTab < 0)
		SelectTab(0);
	return Tab;
}

void TabPanel::SelectTab(int Index)
{
	if (Tabs.Size() < Index + 1)
		return;

	//Don't perform a segmented render on tab switch.
	
	if (CurrentTab != -1)
	{
		Tabs[CurrentTab]->C->bHidden = true;
		Tabs[CurrentTab]->TabButton.bOn = false;
		Tabs[CurrentTab]->TabButton.SetState(Button::STATE_Normal);
	}

	CurrentTab = Index;
	//TabCanvas.SetImage(Tabs[CurrentTab]->C->Buffer.GetTexture());
	Tabs[CurrentTab]->C->Show();
	OnTabChanged.DelegateCallbacks(this);
	Tabs[CurrentTab]->TabButton.bOn = true;
	Tabs[CurrentTab]->TabButton.SetState(Button::STATE_Pressed);
}

//
//	TabPage
//

void TabPage::SetText(String S, String FontName, int FontHeight)
{
	TabButton.SetText(S, FontName, FontHeight);
	Owner->XOffset += (TabButton.Size.X - 128.f);
}

void TabPage::SetSize(const Vect2 TSize)
{
	C->SetSize(TSize - Vect2(2.f, 1.f));
}

void TabPage::SetPosition(const Vect2 Pos)
{
	C->SetPosition(Pos);
}

void TabPage::Tick(double DT)
{
	C->Tick(DT);
}

void TabPage::SegmentRender(Vect2 Pos, Vect2 Size)
{
	C->SegmentRender(Pos, Size);
}


void TabPage::AddItem(Widget* W)
{
	C->AddItem(W);
}
