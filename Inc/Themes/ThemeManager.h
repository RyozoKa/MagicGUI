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
	C_BUTTON_DOWN,
	C_BUTTON_HOVER,
	C_BUTTON_NORMAL,
	C_EXPAND_ICON,
	C_EXPAND_ICON_HOVER,
	C_EXPAND_ICON_CLICK,

	C_ENDOF,
	C_INVALID
};

extern MAGICGUIAPI const char* EnumMap[C_ENDOF];

class MAGICGUIAPI ThemeManager
{
public:

	//Standard defined system colors
	static const Color CanvasWhite;

	static class Texture* GetTextureFor(Controls);
	static bool LoadTheme(const char* ThemeFile);

	static void Init()
	{
		
		//Initialize default theme
		//TODO: Determine where to read default theme file from later
		LoadTheme("./Themes/Default.thm");
	}

private:

	static bool LoadTextureFromLine(const String& Str);
	static Controls StringToID(String& S);

	static class Texture* TextureMap[C_ENDOF];
};

//This is a bit ugly
inline int InitArray()
{
	ADDCONTROL(C_BUTTON_DOWN)
	ADDCONTROL(C_BUTTON_HOVER)
	ADDCONTROL(C_BUTTON_NORMAL)
	ADDCONTROL(C_EXPAND_ICON)
	ADDCONTROL(C_EXPAND_ICON_HOVER)
	ADDCONTROL(C_EXPAND_ICON_CLICK)

	Application::SubsystemModules += &ThemeManager::Init;
	return 1;
}

