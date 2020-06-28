#pragma once

/*
	Basic image widget
	Revision:
		2020-03-24 Christopher Békési
*/
#include "Widget.h"
#include "Texture.h"

class MAGICGUIAPI Image : public Widget
{
public:	
	//Widget interface
	virtual void Tick(double);

	Image() : Widget()
	{
		bUpdate = true;
		ImgObj.Owner = this;
	}

	void SetImage(Texture*);
	void SetColor(const Color Cf);
	virtual void SetSize(const Vect2 Sz);
	void SetType(TYPE);
	virtual void SetPosition(const Vect2);

	virtual void RenderObjects();

	RenderObject ImgObj;
	TYPE DrawType;						//-- Keep trak of the current draw type
};
