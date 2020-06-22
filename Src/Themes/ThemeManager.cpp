#include "ThemeManager.h"
#include <stdio.h>
#include "HString.h"
#include <cstdio>
#include "Texture.h"

Texture* ThemeManager::TextureMap[C_ENDOF];
const char* EnumMap[C_ENDOF] = {};

Texture* ThemeManager::GetTextureFor(Controls C)
{
	return TextureMap[C];
}

bool ThemeManager::LoadTheme(const char* ThemeFile)
{
	FILE* F;
	F = fopen(ThemeFile, "r");
	if(!F)
	{
		printf("Error: Failed to open theme file %s\n", ThemeFile);
		return false;
	}
	char C;
	String Line;
	unsigned int LineCount = 0;
	do
	{
		C = getc(F);
		if(C != '\n' && C != EOF)
			Line += C;
		else
		{
			++LineCount;
			if (!LoadTextureFromLine(Line))
			{
				printf("Error: Failed to load theme file %s\n Syntax error ( %i ): %s\n", ThemeFile, LineCount, Line.Tochar());
				return false;
			}
			Line.Clear();
		}
	} while (C != EOF);
	fclose(F);
	return true;
}

bool ThemeManager::LoadTextureFromLine(const String& Str)
{
	if (Str.IsEmpty() || Str.BeginsWith("//"))
		return true;	//Valid but not useful

	int Index = Str.Find("=");
	String ID;
	if (Index >= 0)
		ID = Str.Substr(0, Index);
	else
		return false;
	Controls Cs;
	ID.Trim();
	if( (Cs = StringToID(ID)) != C_INVALID)
	{
		String Result = Str.Substr(Index + 1);
		Result.Trim();
		Result.ChopStart(1);
		Result.Chop(1);
		Texture* Tx = Texture::CreateTexture(Result.Tochar());
		if (!Tx)
		{
			printf("Failed to load texture %s\n", Result.Tochar());
			return false;
		}
		TextureMap[Cs] = Tx;
		return true;
	}
	return false;
}

Controls ThemeManager::StringToID(String& S)
{
	for(int i = 0; i < C_ENDOF; ++i)
	{
		if (EnumMap[i] && S == EnumMap[i])
			return (Controls)i;
	}
	return C_INVALID;
}
