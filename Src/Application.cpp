/*
	The primary purpose of this Application manager is to handle hit checks and events

*/


#include "Template.h"
#include "Window.h"
#include <thread>
#include <stdlib.h>
#include <wingdi.h>
#include "Image.h"
#include "RenderBuffer.h"
#define GLFW_EXPOSE_NATIVE_WIN32 1
#define GLFW_EXPOSE_NATIVE_WGL 1
#include "GLFW/glfw3native.h"
#ifdef _WIN32
#include <windows.h>
#endif





bool Application::bDriverInit = false;
Vect2 Application::PhysicalSize = { 0, 0 };
Vect2 Application::ScreenRes = { 0, 0 };
unsigned short Application::RefreshRate = 0;
GLFWmonitor* Application::primary = nullptr;
Callbacks<> Application::SubsystemModules;

void Application::Execute()
{
	timeBeginPeriod(1);
	double Delta = 0;
	double Last = 0;
	double Start = glfwGetTime();
	glfwSwapInterval( 0 );
	for (int i = 0; i < Window::WndIdx; ++i)
		Window::OpenWindows[i]->C.Initialize();

	while (Window::WndIdx)
	{
		for(int i = 0; i < Window::WndIdx; ++i)
		{
			if (!Window::OpenWindows[i])
				continue;

			RenderBuffer::WindowFrame = &Window::OpenWindows[i]->C.Buffer;
			RenderBuffer::CurrentFrame = &Window::OpenWindows[i]->C.Buffer;

			if(Window::OpenWindows[i] && glfwGetCurrentContext() != Window::OpenWindows[i]->WindowHandle)
				glfwMakeContextCurrent(Window::OpenWindows[i]->WindowHandle);
			//wglMakeCurrent(NULL,
			//			   NULL);
			//HWND wnd = glfwGetWin32Window(Window::OpenWindows[i]->WindowHandle);
			//HDC Dc = GetDC(wnd);
			///HGLRC Glrc = glfwGetWGLContext(Window::OpenWindows[i]->WindowHandle);
			//wglMakeCurrent(Dc,
			//			   Glrc);
			//glewInit();
			
			//glfwMakeContextCurrent(nullptr);
			
			glViewport(0, 0, Window::OpenWindows[i]->WindowSize.X, Window::OpenWindows[i]->WindowSize.Y);

		//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Window::OpenWindows[i]->Tick(Delta);
			
			//glFlush();
			glfwSwapBuffers(Window::OpenWindows[i]->WindowHandle);
			//SwapBuffers(GetDC(glfwGetWin32Window(Window::OpenWindows[i]->WindowHandle)));
			
			//glfwMakeContextCurrent(nullptr);
			//if (bPopup)
			//	continue;
			
			glfwPollEvents();
			if( Window::OpenWindows[i] && Window::OpenWindows[i]->bPendingResize)
			{
				Vect2 OldSize = Window::OpenWindows[i]->WindowSize;
				Window::OpenWindows[i]->SetSize(Window::OpenWindows[i]->NewSize);
				Window::OpenWindows[i]->C.OnWindowResize(Window::OpenWindows[i]->NewSize - OldSize);
				Window::OpenWindows[i]->bPendingResize = false;
			}
		}
		
		Last = glfwGetTime();
		Delta = Last - Start;
		Start = Last;
		if (Delta > 0.025)
			Delta = 0.025;
		int Val = (int)(25.0 - (Delta * 1000.f));
		if(Val > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds( Val ));//(int)( 16.6666 - (Delta  * 1000.f)  )

		
	}
	timeEndPeriod(1);

}