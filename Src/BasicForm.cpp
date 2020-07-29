#include "BasicForm.h"


BasicForm::BasicForm(const Vect2 Size, const char* Title, MODE Mode, bool Borderless)
{
	//Handle errors later
	W = Window::CreateWindow(Size, Title, Mode, Borderless);
}

void BasicForm::AddItem(Widget* Wg)
{
	W->AddItem(*Wg);
}

inline Window* BasicForm::GetWindow()
{
	return W;
}

