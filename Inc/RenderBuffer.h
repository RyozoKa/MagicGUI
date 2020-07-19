#pragma once

/*
	A basic render buffer that helps abstract OpenGL code.
	Selecting this buffer before drawing will draw to this buffer instead
	of the main buffer. 

	Revision
	Christopher Békési 2020-06-30
*/

#include "Template.h"
#include "Texture.h"
#include "HDynamicArray.h"

class MAGICGUIAPI RenderBuffer
{
public:
	RenderBuffer(RenderObject* Frame);
	void SetSize(const Vect2 Size);
	void Select();
	void Restore();
	static void SelectDefault();
	class Texture* GetTexture();

	//Internal
	static void Init();
	void DriverInit();

	bool bUpdate = false;

	Vect2 Size;
	class Widget* Owner;
	Vect2 AbsPosition;	//TODO: Remove
	static RenderBuffer* CurrentFrame;
	static RenderBuffer* WindowFrame;

private:

	bool bInitialized = false;
	int FBO = 0;
	Texture Tx;
	RenderObject* BoundFrame;
	static HDynamicArray<RenderBuffer*> Buffers;
	static HDynamicArray<RenderBuffer*> Stack;

	friend class Application;
	friend class RenderObject;
	friend class Window;
	friend class Canvas;
	friend class WindowCanvas;
	friend class TextBuffer;
};

inline bool RegisterSubsystem()
{
	Application::SubsystemModules += &RenderBuffer::Init;
	return true;
}

const bool bReg = RegisterSubsystem();
