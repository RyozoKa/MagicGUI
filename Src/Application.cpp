/*
	The primary purpose of this Application manager is to handle hit checks and events

*/

#include "Template.h"
#include "Window.h"
#include <thread>
#include <stdlib.h>

#include "Image.h"
#include "RenderBuffer.h"

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

	
	// During init, enable debug output
	
	/*GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, Window::OpenWindows[0]->WindowSize.X, Window::OpenWindows[0]->WindowSize.Y, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Failed to create framebuffer\n");
		return;
	}
	*/
	//RenderObject Obj;

	//Obj.SetPrimitive({ Window::OpenWindows[0]->WindowSize.X, Window::OpenWindows[0]->WindowSize.Y });
	Image Dummy;
	RenderBuffer ABuf(&Dummy.ImgObj);
	ABuf.SetSize({ Window::OpenWindows[0]->WindowSize.X - 200, Window::OpenWindows[0]->WindowSize.Y - 200});
	Dummy.Window = Window::OpenWindows[0];
	//Obj.Owner = &Dummy;
	//Obj.SetRenderMode(TYPE::TYPE_COLOR);
	//Obj.SetImage(&Dest);
	//Obj.SetColor(Color(255, 0, 0));
	Dummy.SetPosition(Vect2(100.f, 100.f));
	Dummy.SetSize({ Window::OpenWindows[0]->WindowSize.X - 200, Window::OpenWindows[0]->WindowSize.Y - 200});
	Dummy.SetImage(ABuf.GetTexture());

	while (Window::WndIdx)
	{
		for(int i = 0; i < Window::WndIdx; ++i)
		{
			//glfwSwapInterval(1);
			//glAddSwapHintRectWIN(PFD_SWAP_EX)
			//glDrawBuffer(GL_FRONT_AND_BACK);
			//Sanity check. 
			RenderBuffer::WindowFrame = &Window::OpenWindows[i]->C.Buffer;
			RenderBuffer::CurrentFrame = &Window::OpenWindows[i]->C.Buffer;

			//ABuf.Select();
			if(Window::OpenWindows[i] && glfwGetCurrentContext() != Window::OpenWindows[i]->WindowHandle)
				glfwMakeContextCurrent(Window::OpenWindows[i]->WindowHandle);
			glViewport(0, 0, Window::OpenWindows[i]->WindowSize.X, Window::OpenWindows[i]->WindowSize.Y);
			
			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			//Tick all open windows here
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
			Window::OpenWindows[i]->Tick(Delta);
			//Polling event has to be done within the same loop.
			//Otherwise we won't be able to check for the close flag and the program will continue forever
			//glClearColor(1.f, 0.f, 0.f, 1.0);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glFlush();
			//glFlush();
			//glFinish();
			//glDrawBuffers(1, DrawBuffers);
			//ABuf.SelectDefault();
			//glViewport(0, 0, Window::OpenWindows[i]->WindowSize.X, Window::OpenWindows[i]->WindowSize.Y);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//Dummy.RenderObjects();
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