/*
	The primary purpose of this Application manager is to handle hit checks and events

*/

#include "Template.h"
#include "Window.h"
#include <thread>
#include <stdlib.h>

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (Window::WndIdx)
	{
		for(int i = 0; i < Window::WndIdx; ++i)
		{
			//glfwSwapInterval(1);
			//glAddSwapHintRectWIN(PFD_SWAP_EX)
			//glDrawBuffer(GL_FRONT_AND_BACK);
			//Sanity check. 
			if(Window::OpenWindows[i])
				glfwMakeContextCurrent(Window::OpenWindows[i]->WindowHandle);
			//Tick all open windows here
			glViewport(0, 0, Window::OpenWindows[i]->WindowSize.X, Window::OpenWindows[i]->WindowSize.Y);
			Window::OpenWindows[i]->Tick(Delta);
			//Polling event has to be done within the same loop.
			//Otherwise we won't be able to check for the close flag and the program will continue forever
			
			//glFlush();
			//glFinish();
			glFlush();
			
			//if(Window::OpenWindows[i])
			//	glfwSwapBuffers(Window::OpenWindows[i]->WindowHandle);
			glfwPollEvents();
			//copy_buffer();
			//if(glfwWindowShouldClose(Window::OpenWindows[i]->WindowHandle))
			//	return;
		}

		Last = glfwGetTime();
		Delta = Last - Start;
		Start = Last;
		if (Delta > 0.0166666667)
			Delta = 0.0166666667;
		int Val = (int)(16.6666667 - (Delta * 1000.f));
		if(Val > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds( Val ));//(int)( 16.6666 - (Delta  * 1000.f)  )

		
	}
	timeEndPeriod(1);

}