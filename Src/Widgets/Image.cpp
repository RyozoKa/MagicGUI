#include "Image.h"
#include <cstdio>
void Image::Tick(double DT)
{
	//if (!bUpdate)
	//	return;
	Widget::Tick(DT);
}

void Image::SetImage(Texture* Tx)
{
	ImgObj.SetImage(Tx);
	bUpdate = true;
	printf("Set image %p\n", Tx);
}

void Image::SetColor(const Color Cf)
{
	ImgObj.SetColor(Cf);
	bUpdate = true;
}

void Image::SetSize(const Vect2 Sz)
{
	ImgObj.SetPrimitive(Sz);
	Size = Sz;
	DrawSize = Sz;
	bUpdate = true;
}

void Image::SetType(TYPE T)
{
	ImgObj.SetRenderMode(T);
	DrawType = T;
}

void Image::SetPosition(const Vect2 Pos)
{
	Position = Pos;
	bUpdate = true;
}

void Image::RenderObjects()
{
	ImgObj.DrawObject(Position);
}
