#include "Canvas.h"

void Canvas::SetColor(Color C)
{
	Background.SetColor(C);
	bUpdate = true;
}

void Canvas::SetImage(Texture* T)
{
	Background.SetImage(T);
	bUpdate = true;
}

void Canvas::SetMode(TYPE T)
{
	Background.SetRenderMode(T);
	bUpdate = true;
}

void Canvas::RenderObjects()
{
	Background.DrawObject(Position);
}
