/*
	Base class for all windows

	Revision:
	2020-03-11 Christopher Békési
*/

#include "Texture.h"
#include "Widget.h"
#include "WindowCanvas.h"
#include "Gridsubsystem.h"

enum MODE
{
	MD_FULLSCREEN,
	MD_WINDOWED,
	MD_MAXIMIZED,
	MD_MINIMIZED
};

class MAGICGUIAPI Window
{
public:
	//Static functions
	static Window* CreateWindow(const Vect2 Size, const char* Title, MODE Mode, bool Borderless);

	void SetPosition(const Vect2 Pos);
	void SetSize(const Vect2 Size);
	void SetForeground(Color Col);
	void SetForeground(Texture* Tx);
	void SetDrawstyle(TYPE Mode); 
	void Close();
	void Minimize();
	void Maximize();
	void Restore();
	void SetTitle(const char* Title);
	//Toggle Maximized/Restore
	void ToggleWindow();
	void ToggleFullscreen();
	void Hide();
	void Show();
	void Focus();

	//	I/O interface
	void OnCursor(double X, double Y);
	void OnKeyPress(int Key, int Mod);
	void OnKeyRelease(int Key, int Mod);
	void OnMouseClick(int Key);
	void OnMouseRelease(int Key);
	void OnScroll(double XOffset, double YOffset);

	virtual void Tick(double);

	void AddItem(Widget& Item);

	void IsClosing();

	static Window* OpenWindows[255];
	static unsigned int WndIdx;
	static class GLFWwindow* Context;	//--	Using a single context
	Vect2 CursorPos;
	class GLFWwindow* WindowHandle;


	
	Vect2 WindowSize;					//--	Current Window size, relative to Viewport matrix
	MODE CurrentMode;					//--	Window mode
	bool Borderless;					//--	Borderless?
	unsigned int ShaderProg;			//--	Shader program for this window.
	unsigned int WndIndex;				//--	Index into the static registry. Used to avoid on demand lookup

	WindowCanvas C;							//--	Main Canvas for this window

	

	Window(const Vect2 Size)
	{
		//Initialize canvas widget here
		C.Window = this;
		C.ZIndex = 0;	//Canvas will always be 0 indexed, to avoid collision detection
		WindowSize = Size;
		C.SetSize(Size);
		C.Attached();
	}

	
};

/*
	These are global GLFW functions that handles
	Human Interface Devices
*/

void CursorCallback(GLFWwindow* Wnd, double X, double Y);
void KeyCallback(GLFWwindow* Wnd, int Key, int Scancode, int Action, int Mods);
void MouseClickCallback(GLFWwindow* Wnd, int Button, int Action, int Mods);
void OnScrollCallback(GLFWwindow* Wnd, double XOffset, double YOffset);