#pragma once


#include "Template.h"
#include "RenderBuffer.h"

/*
	This class is a modified version of the RenderBuffer
	Primary reason is to separate text generation/assembly and text drawing.
	This will also allow us to reuse a previously assembled text string to draw repeating text

	Revision:
		Christopher Békési 2020-07-17

*/

class MAGICGUIAPI TextBuffer
{
public:
	TextBuffer(String &S, class Map8*);
	
	class Texture* GetTexture();

	//Internal
	static void Init();
	void DriverInit();
	void Detach();
	bool bUpdate = false;
	Vect2 Size;

	static TextBuffer* GetDrawableUTF8Text(String &S, String &Font, int Height);
	static TextBuffer* GetDrawableUTF8Text(String &S, class Map8*);
	
	//Main text buffer pool. This stores all the currently existing
	//Text buffers for 1) Reuse text pools 2) Precache text pools for tasks
	//That require a lot of dynamic text
	static dense_hash_map<HASH, TextBuffer*> TextPool;
private:

	bool bInitialized = false;
	int FBO = 0;
	Texture Tx;
	String Text;
	unsigned int References = 0;
	HASH Hash;
	class Map8* Font;
};

//TODO: Implement post initializ
inline bool RegisterBSubsystem()
{
	Application::SubsystemModules += &TextBuffer::Init;
	return true;
}

const bool bRegs = RegisterBSubsystem();