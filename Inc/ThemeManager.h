/*
	This is a basic theme delegation manager that allows a user to
	change default themes.

	TODO: Implement a system for vectorized and dynamic textures
	TODO: Implement a default dialog interface to handle theme selection	
	Revision:
		Christopher Békési 2020-06-19
*/

#include "Template.h"

#define ADDCONTROL(Ctl) EnumMap[Ctl] = #Ctl;


//Controls that can use the theme manager
enum Controls
{
	C_BUTTON,

	C_ENDOF,
	C_INVALID
};

extern const char* EnumMap[C_ENDOF];


constexpr int InitArray()
{
	ADDCONTROL(C_BUTTON)

	return 1;
}

const int iss = InitArray();

class MAGICGUIAPI ThemeManager
{
public:
	static class Texture* GetTextureFor(Controls);
	static bool LoadTheme(const char* ThemeFile);

private:

	static bool LoadTextureFromLine(const String& Str);
	static Controls StringToID(String& S);

	static class Texture* TextureMap[C_ENDOF];
};