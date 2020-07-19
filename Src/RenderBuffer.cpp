#include "RenderBuffer.h"

HDynamicArray<RenderBuffer*> RenderBuffer::Buffers;
RenderBuffer* RenderBuffer::CurrentFrame = nullptr;
RenderBuffer* RenderBuffer::WindowFrame = nullptr;
HDynamicArray<RenderBuffer*> RenderBuffer::Stack;

RenderBuffer::RenderBuffer(RenderObject* Frame)
{
	BoundFrame = Frame;
}

void RenderBuffer::SetSize(const Vect2 aSize)
{
	if(Application::bDriverInit)
	{
		if (!bInitialized)
		{
			glGenFramebuffers(1, (GLuint*)&FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glGenTextures(1, (GLuint*)&Tx._InternalID);
			glBindTexture(GL_TEXTURE_2D, Tx._InternalID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aSize.X, aSize.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tx._InternalID, 0);
			GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, DrawBuffers);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				printf("Failed to create framebuffer\n");
			bInitialized = true;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			Tx.Width = aSize.X;
			Tx.Height = aSize.Y;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, Tx._InternalID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aSize.X, aSize.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			Tx.Width = aSize.X;
			Tx.Height = aSize.Y;
			if (BoundFrame)
				BoundFrame->SetTextureClipping(aSize);
		}
		
	}
	Size = aSize;
}

void RenderBuffer::Select()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	Stack.Insert(CurrentFrame);
	CurrentFrame = this;
}

void RenderBuffer::Restore()
{
	CurrentFrame = Stack.PopLast();
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFrame->FBO);
}

void RenderBuffer::SelectDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CurrentFrame = WindowFrame;
}

Texture* RenderBuffer::GetTexture()
{
	return &Tx;
}


//Static
void RenderBuffer::Init()
{
	for (int i = 0; i < Buffers.Size(); ++i)
		Buffers[i]->DriverInit();
}

void RenderBuffer::DriverInit()
{
	if (!bInitialized)
	{
		glGenFramebuffers(1, (GLuint*)&FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, (GLuint*)&Tx._InternalID);
		glBindTexture(GL_TEXTURE_2D, Tx._InternalID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Size.X, Size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tx._InternalID, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Failed to create framebuffer\n");
	}
}

