#include "Label.h"
#include "TextRenderer.h"
#include "Font.h"

void Label::SetText(String S, String FontName, int Height)
{
	if (TB)
		TB->Detach();
	if(S.IsEmpty())
	{
		ImgObj.SetRenderMode(TYPE::TYPE_NONE);
		bUpdate = true;
		return;
	}
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
	TextObj.DrawObject(Position);
}
