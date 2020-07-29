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
	DrawType |= TYPE::TYPE_TEXTURE;
	bUpdate = true;
}

void Image::SetColor(const Color Cf)
{
	ImgObj.SetColor(Cf);
	DrawType |= TYPE::TYPE_COLOR;
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
	bUpdate = true;
}

void Image::RenderObjects()
{
	ImgObj.DrawObject(Position);
}
