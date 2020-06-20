#include "ThemeManager.h"
#include <stdio.h>
#include "HString.h"
#include <cstdio>

Texture* ThemeManager::TextureMap[C_ENDOF];
const char* EnumMap[C_ENDOF] = {};

Texture* ThemeManager::GetTextureFor(Controls C)
{
	return TextureMap[C];
}

bool ThemeManager::LoadTheme(const char* ThemeFile)
{
	FILE* F;
	F = fopen(ThemeFile, "r+b");
	char* C;
	String Line;
	unsigned int LineCount = 0;
	while( (C = getc(F)) != EOF )
	{
		while (C != '\n')
			Line += C;
		++LineCount;
		if (!LoadTextureFromLine(Line))
		{
			printf("Error: Failed to load theme file %s\n Syntax error ( %i ): %s\n", ThemeFile, LineCount, Line.Tochar());
			return false;
		}
	}
	return true;
}

bool ThemeManager::LoadTextureFromLine(const String& Str)
{
	if (Str.IsEmpty() || Str.BeginsWith("//"))
		return true;	//Valid but not useful

	int Index = Str.Find("=");
	String ID;
	if (!Index)
		ID = Str.Substr(0, Index);
	else
		return false;
	if( (Controls Cs = StringToID(ID)) != C_INVALID)
	{
		String Result = Str.Substr(Index);
		Result.Trim();
		Result.ChopStart(1);
		Result.Chop(1);
		printf("File for ID %s is : %s\n", ID.Tochar(), Result.Tochar());
	}
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
