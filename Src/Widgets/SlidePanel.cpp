#include "SlidePanel.h"

void SlidePanel::AddItem(Widget* W)
{
	C->AddItem(W);
}


void SlidePanel::Toggle()
{
	if(C->DrawSize.X == C->Size.X)
	{
		C->VerticalCollapse();
		ToggleButton->ReturnAnim();
	}
	else
	{
		C->VerticalExpand();
		ToggleButton->Animate();
	}
}