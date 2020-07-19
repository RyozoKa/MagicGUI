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

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	if(type != 0x8251)
  printf( "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
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

			

			//Initialize empty texture
			unsigned int Data = 0xFFFFFFFF;
			glGenTextures(1, (GLuint*)&Texture::EMPTY);
			glBindTexture(GL_TEXTURE_2D, (GLuint)Texture::EMPTY);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Data);
			for(auto it = Texture::_HashTable.begin(); it != Texture::_HashTable.end(); ++it)
			{
				it->second->DriverStartup();
			}
			Application::bDriverInit = true;
			Application::SubsystemModules.DelegateCallbacks();
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
		RenderBuffer::WindowFrame = &Wds->C.Buffer;
		RenderBuffer::CurrentFrame = &Wds->C.Buffer;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);  
		glDisable(GL_CULL_FACE);

		glEnable              ( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( MessageCallback, 0 );

		return Wds;
}

void Window::SetPosition(const Vect2 Pos)
{
	glfwSetWindowPos(WindowHandle, Pos.X, Pos.Y);
}

//Not called when created. See constructor!
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
	CursorPos.X = X;
	CursorPos.Y = Y;
	C.OnCursor(X, Y);
}

void Window::OnKeyPress(int Key, int Mod)
{
	C.OnKeyPress(Key, Mod);
}

void Window::OnKeyRelease(int Key, int Mod)
{
}

void Window::OnMouseClick(int Key)
{
	switch(Key)
	{
		case MOUSE_BUTTON_LEFT:
			C.OnMouseLeftClick(CursorPos.X, CursorPos.Y);
		break;
		case MOUSE_BUTTON_RIGHT:
			C.OnMouseRightClick(CursorPos.X, CursorPos.Y);
		break;
	}
}

void Window::OnMouseRelease(int Key)
{
	switch(Key)
	{
		case MOUSE_BUTTON_LEFT:
			C.OnMouseLeftReleased(CursorPos.X, CursorPos.Y);
		break;
		case MOUSE_BUTTON_RIGHT:
			C.OnMouseRightReleased(CursorPos.X, CursorPos.Y);
		break;
	}
}

void Window::OnScroll(double XOffset, double YOffset)
{
	C.OnScroll(YOffset);
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
			Wnd->Wnd->OnMouseRelease(Button);
		break;
	}
}

void OnScrollCallback(GLFWwindow* Wnd, double XOffset, double YOffset)
{
	Wnd->Wnd->OnScroll(XOffset, YOffset);
}

//Stacked scissor box
struct Scissor
{
	Vect2 Position;
	Vect2 Size;
};

HDynamicArray<Scissor> Stack;

Scissor CurrentScissor;

#include "RenderBuffer.h"

void ScissorBox(Vect2 Pos, Vect2 Size)
{
	//Pos += RenderBuffer::CurrentFrame->AbsPosition;
	if (!CurrentScissor.Size.IsEmpty())
	{
		Stack.Insert(CurrentScissor);		//-- Push previous box to the stack

		Vect2& CurrentPos = CurrentScissor.Position;
		Vect2& CurrentSize = CurrentScissor.Size;

		//Perform necessary box clipping
		Vect2 LastPos = CurrentPos;
		CurrentPos.X = MAX(CurrentPos.X, Pos.X);
		CurrentPos.Y = MAX(CurrentPos.Y, Pos.Y);

		CurrentSize.X = MIN(LastPos.X + CurrentSize.X, Pos.X + Size.X);
		CurrentSize.Y = MIN(LastPos.Y + CurrentSize.Y, Pos.Y + Size.Y);
		CurrentSize -= CurrentPos;

		//Very important that we don't pass negatives to the scissor box
		CurrentSize.X = MAX(CurrentSize.X, 0.f);
		CurrentSize.Y = MAX(CurrentSize.Y, 0.f);
	}
	else
	{
		CurrentScissor.Position = Pos;
		CurrentScissor.Size = Size;

		CurrentScissor.Size.X = MAX(CurrentScissor.Size.X, 0.f);
		CurrentScissor.Size.Y = MAX(CurrentScissor.Size.Y, 0.f);
	}
	glScissor(CurrentScissor.Position.X, (RenderBuffer::CurrentFrame->Size.Y - CurrentScissor.Position.Y) - CurrentScissor.Size.Y, CurrentScissor.Size.X, CurrentScissor.Size.Y);
	glEnable(GL_SCISSOR_TEST);
}

void DisableScissor()
{
	if(Stack.Size())
	{
		CurrentScissor = Stack.PopLast();
		glScissor(CurrentScissor.Position.X, (RenderBuffer::CurrentFrame->Size.Y - CurrentScissor.Position.Y) - CurrentScissor.Size.Y, CurrentScissor.Size.X, CurrentScissor.Size.Y);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
		CurrentScissor.Position.Zero();
		CurrentScissor.Size.Zero();
	}
}