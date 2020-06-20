#include "Window.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Widget.h"
#include <cstring>
#include <cstdio>

Window* Window::OpenWindows[255];
unsigned int Window::WndIdx = 0;
GLFWwindow* Window::Context = nullptr;

void Window::Tick(double dt)
{
	C.Tick(dt);
}

void Window::AddItem(Widget& Item)
{
	C.AddItem(&Item);
	GridSystem.InsertWidget(Item);
}

void Window::IsClosing()
{
	//Exclude this window from the list
	--WndIdx;
	Window::OpenWindows[WndIndex] = nullptr;
	delete this;
}

//This doesn't get called on hardcoded resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (window->Wnd)
	{
		window->Wnd->SetSize({ (float) width, (float) height });
	}
}

void window_close_callback(GLFWwindow* window)
{
	window->Wnd->IsClosing();
}

Window* Window::CreateWindow(const Vect2 Size, const char* Title, MODE Mode, bool Borderless)
{
	GLFWwindow* Wd;

	if (!Context)
	{
		bool Res = glfwInit();
		if (!Res)
		{
			return NULL;
			std::cout << "Failed to initialize glfw. Aborting\n";
			exit(-1);
		}
	}
		if(Borderless)
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		if (!Context)
		{
			
			Application::primary = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(Application::primary);
			Application::ScreenRes.X = mode->width;
			Application::ScreenRes.Y = mode->height;
			Application::RefreshRate = mode->refreshRate;
			int X, Y;
			glfwGetMonitorPhysicalSize(Application::primary, &X, &Y);
			Application::PhysicalSize.X = X;
			Application::PhysicalSize.Y = Y;

			

			Context = Wd = glfwCreateWindow(Size.X, Size.Y, Title, Mode == MD_FULLSCREEN ? Application::primary : nullptr, nullptr);
			glfwMakeContextCurrent(Wd);
			volatile int res = glewInit();
			
			//Shader must be initialized after glewInit
			Shader.InitializeShader("Shaders/Vert.shader", "Shaders/Frag.shader");

			for(auto it = Texture::_HashTable.begin(); it != Texture::_HashTable.end(); ++it)
			{
				it->second->DriverStartup();
			}
			Application::bDriverInit = true;

			//Initialize empty texture
			unsigned int Data = 0xFFFFFFFF;
			glGenTextures(1, (GLuint*)&Texture::EMPTY);
			glBindTexture(GL_TEXTURE_2D, (GLuint)Texture::EMPTY);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Data);
		}
		else
			Wd = glfwCreateWindow(Size.X, Size.Y, Title, Mode == MD_FULLSCREEN ? Application::primary : nullptr, Context);

		if(!Wd)
		{
			glfwTerminate();
			exit(-1);
		}
		
		Window* Wds = new Window(Size); 
		Wd->Wnd = Wds;
		Wds->WindowHandle = Wd;
		//Wds->SetSize(Size);
		Wds->SetForeground({ 252, 255, 219, 255 });
		Wds->SetDrawstyle(TYPE::TYPE_COLOR);
		if (Mode == MD_MINIMIZED)
			glfwIconifyWindow(Wd);
		Wds->CurrentMode = Mode;

		//Set callbacks
		glfwSetFramebufferSizeCallback(Wd, framebuffer_size_callback);
		glfwSetWindowCloseCallback(Wd, window_close_callback);
		glfwSetCursorPosCallback(Wd, &CursorCallback);
		glfwSetKeyCallback(Wd, &KeyCallback);
		glfwSetMouseButtonCallback(Wd, &MouseClickCallback);
		glfwSetScrollCallback(Wd, &OnScrollCallback);

		//glDisable(GL_DEPTH_TEST);
		//Iterating over these are fast.
		for (unsigned int i = 0; i < 255; ++i)
		{
			if (!Window::OpenWindows[i])
			{
				Wds->WndIndex = i;
				break;
			}
		}
		Window::OpenWindows[Wds->WndIndex] = Wds;
		++Window::WndIdx;
		return Wds;
}

void Window::SetPosition(const Vect2 Pos)
{
	glfwSetWindowPos(WindowHandle, Pos.X, Pos.Y);
}

void Window::SetSize(const Vect2 Size)
{
	glfwSetWindowSize(WindowHandle, Size.X, Size.Y);
	WindowSize = Size;
	C.SetSize(Size);
}

void Window::SetForeground(Color Col)
{
	C.SetColor(Col);
}

void Window::SetForeground(Texture* Tx)
{
	C.SetImage(Tx);
}

void Window::SetDrawstyle(TYPE Mode)
{
	C.SetMode(Mode);
}

void Window::Close()
{
	glfwDestroyWindow(WindowHandle);
	delete this;	//This is still safe, as long as no one attempts to delete it again.
}

void Window::Minimize()
{
	glfwIconifyWindow(WindowHandle);
	CurrentMode = MD_MINIMIZED;
}

void Window::Maximize()
{
	glfwMaximizeWindow(WindowHandle);
	CurrentMode = MD_MAXIMIZED;
}

void Window::Restore()
{
	glfwRestoreWindow(WindowHandle);
	CurrentMode = MD_WINDOWED;
}

void Window::SetTitle(const char* Title)
{
	glfwSetWindowTitle(WindowHandle, Title);
}

//Toggle Maximized/Restore

void Window::ToggleWindow()
{
	if (CurrentMode == MD_MAXIMIZED)
		Restore();
	else
		Maximize();
}

void Window::ToggleFullscreen()
{
	glfwSetWindowMonitor(WindowHandle, CurrentMode == MD_FULLSCREEN ? nullptr : Application::primary, 0, 0, Application::ScreenRes.X, Application::ScreenRes.Y, Application::RefreshRate);
	if (CurrentMode == MD_FULLSCREEN)
		CurrentMode = MD_WINDOWED;
	else
		CurrentMode = MD_FULLSCREEN;
}

void Window::Hide()
{
	glfwHideWindow(WindowHandle);
}

void Window::Show()
{
	glfwShowWindow(WindowHandle);
}

void Window::Focus()
{
	glfwFocusWindow(WindowHandle);
}

void Window::OnCursor(double X, double Y)
{
	Widget* W;
	
	//Don't perform another hit test if we don't have to
	if(CursorPos.X == X && CursorPos.Y == Y)
		W = LastHit;
	else
	{
		CursorPos.X = X;
		CursorPos.Y = Y;
		W = GridSystem.GetWidget(CursorPos);
		if (LastHit != nullptr && LastHit != W)
			LastHit->OnMouseLeave(X, Y);
	}
	
	if (W)
		W->OnMouseEnter(X, Y);
	LastHit = W;
	//TODO: Deal with redirects later
}

void Window::OnKeyPress(int Key, int Mod)
{
	if (Focused)
		Focused->OnKeyPressed(Key, Mod);
}

void Window::OnKeyRelease(int Key, int Mod)
{
}

void Window::OnMouseClick(int Key)
{
	Focused = LastHit;
	if (!LastHit)
		return;
	switch(Key)
	{
		case MOUSE_BUTTON_LEFT:
			LastHit->OnMouseLeftClick(CursorPos.X, CursorPos.Y);
		break;
		case MOUSE_BUTTON_RIGHT:
			LastHit->OnMouseRightClick(CursorPos.X, CursorPos.Y);
		break;
	}

	printf("%f %f %p\n", CursorPos.X, CursorPos.Y, LastHit);
}

void Window::OnMouseRelease(int Key)
{
	if (!LastHit)
		return;

	switch(Key)
	{
		case MOUSE_BUTTON_LEFT:
			LastHit->OnMouseLeftReleased(CursorPos.X, CursorPos.Y);
		break;
		case MOUSE_BUTTON_RIGHT:
			LastHit->OnMouseRightReleased(CursorPos.X, CursorPos.Y);
		break;
	}
}

void Window::OnScroll(double XOffset, double YOffset)
{
	if (Focused)
		Focused->OnScroll(YOffset);
}


void CursorCallback(GLFWwindow* Wnd, double X, double Y)
{
	Wnd->Wnd->OnCursor(X, Y);
	
}

void KeyCallback(GLFWwindow* Wnd, int Key, int Scancode, int Action, int Mods)
{
	switch(Action)
	{
		case GLFW_PRESS:
		case GLFW_REPEAT:
			Wnd->Wnd->OnKeyPress(Key, Mods);
		break;
		case GLFW_RELEASE:
			Wnd->Wnd->OnKeyRelease(Key, Mods);
		break;
	}
}

void MouseClickCallback(GLFWwindow* Wnd, int Button, int Action, int Mods)
{
	switch(Action)
	{
		case GLFW_PRESS:
			Wnd->Wnd->OnMouseClick(Button);
		break;
		case GLFW_RELEASE:
			Wnd->Wnd->OnMouseClick(Button);
		break;
	}
}

void OnScrollCallback(GLFWwindow* Wnd, double XOffset, double YOffset)
{
	Wnd->Wnd->OnScroll(XOffset, YOffset);
}