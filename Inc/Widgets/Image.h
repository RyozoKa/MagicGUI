#pragma once

/*
	Basic image widget
	Revision:
		2020-03-24 Christopher B�k�si
*/
#include "Widget.h"
#include "Texture.h"

class MAGICGUIAPI Image : public Widget
{
public:	
	//Widget interface
	virtual void Tick(double);

	Image()
	{
		bUpdate = true;
		ImgObj.Owner = this;
	}

	//Image interface
	void SetImage(Texture*);
	void SetColor(Color& Cf);
	void SetSize(Vect2& Sz);
	void SetType(TYPE);
	void SetPosition(Vect2&);

	virtual void RenderObjects();

	RenderObject ImgObj;
	TYPE DrawType;						//-- Keep trak of the current draw type
};