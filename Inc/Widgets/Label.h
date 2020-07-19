#pragma once

/*
	Simple multi-line text label

	Revision:
		Christopher Békési 2020-07-18
*/

#include "Template.h"
#include "Image.h"

class MAGICGUIAPI Label : public Image
{
public:

	Label() : Image()
	{
		bTransparent = true;	//-- By default
		TextObj.Owner = this;
		TextObj.SetColor(Color(0, 0, 0));

		//This helps black textures blend
		TextObj.RedOffset = 1.f;
		TextObj.GreenOffset = 1.f;
		TextObj.BlueOffset = 1.f;
	}
	
	virtual void SetText(String S, String FontName, int FontHeight);
	virtual void SetTextColor(Color TextColor);
	virtual void RenderObjects();

private:

	RenderObject TextObj;
	class TextBuffer* TB = nullptr;
	String Text;
	String FontName;
	class Map8* Font = nullptr;
};