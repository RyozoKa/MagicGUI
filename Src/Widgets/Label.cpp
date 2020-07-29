#include "Label.h"
#include "TextRenderer.h"
#include "Font.h"

void Label::SetText(String S, String FontName, int Height)
{
	if (TB)
		TB->Detach();
	Text = S;
	if(S.IsEmpty())
	{
		bHidden = true;	//Don't even process it
		bUpdate = true;
		return;
	}
	bHidden = false;
	Font = Font::LoadTrueTypeFont8(FontName.Tochar(), Height);
	TB = TextBuffer::GetDrawableUTF8Text(S, Font);
	TextObj.SetPrimitive(TB->Size);
	TextObj.SetImage(TB->GetTexture());
	//TextObj.SetRenderMode(TYPE::TYPE_TEXTURE);

	if(DrawSize < TB->Size)
		SetSize(TB->Size);
}

void Label::SetTextColor(Color TextColor)
{
	TextObj.SetColor(TextColor);
}


void Label::RenderObjects()
{

	ImgObj.DrawObject(Position);
	TextObj.DrawObject(Position);
}
