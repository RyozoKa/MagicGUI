#include "TextRenderer.h"
#include "Font.h"

dense_hash_map<HASH, TextBuffer*> TextBuffer::TextPool;

TextBuffer::TextBuffer(String S, Map8* FontMap)
{
	//Pre-calculate metrics
	int MaxWidth = 0;
	int MaxHeight = FontMap->Height;
	int CurrentMaxHeight = MaxHeight;	//-- This is used for 
	for (int i = 0; i < S.Length(); ++i)
	{
		int NegOffset = FontMap->CharMap[S[i]].Size.Y - FontMap->CharMap[S[i]].Bearing.Y;	//Negative offset delta
		
		if (S[i] == '\n')
		{
			MaxHeight += CurrentMaxHeight;
			CurrentMaxHeight = FontMap->Height;
			Size.X = MAX(MaxWidth, Size.X);	//Modulate max width
			continue;
		}
		CurrentMaxHeight = MAX(FontMap->Height + NegOffset, CurrentMaxHeight);
		MaxWidth += FontMap->CharMap[S[i]].Advance;
	}
	Size.X = MAX(MaxWidth, Size.X);
	Size.Y = MAX(CurrentMaxHeight, MaxHeight);

	Hash = HashName(S) + FontMap->FontName;
	Font = FontMap;
	//Size.X = MaxWidth;
	//Size.Y = FontMap->Height;
	Text = S;
	TextPool[Hash] = this;
	if(Application::bDriverInit)
	{
		unsigned int VertexBuffer = 0; 		
		unsigned int VertexArray = 0;
		unsigned int ElementBuffer = 0;

		glGenFramebuffers(1, (GLuint*)&FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		
		glGenTextures(1, (GLuint*)&Tx._InternalID);
		glBindTexture(GL_TEXTURE_2D, Tx._InternalID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Size.X, Size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tx._InternalID, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Failed to create textbuffer\n");
		bInitialized = true;
		int CaretPos = 0;
		int CaretY = 0;
		Tx.Height = Size.Y;
		Tx.Width = Size.X;
		unsigned int indices[] =
		{  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		//Draw fonts
		for(int i = 0; i < S.Length(); ++i)
		{
			if (S[i] == ' ' || S[i] == '\t')
			{
				CaretPos += FontMap->CharMap[S[i]].Advance;
				continue;
			}
			else if (S[i] == '\n')
			{
				CaretY += FontMap->Height;
				CaretPos = 0;
				continue;
			}
			int NegOffset = FontMap->CharMap[S[i]].Size.Y - FontMap->CharMap[S[i]].Bearing.Y;	//Negative offset delta
			Vect2 FontSize = FontMap->CharMap[S[i]].Size;
			Vect2 Origin = FontSize / 2;
			//Draw texture upside down
			Vect2 Vertices[8];
			Vertices[0] = Origin;								//Top right
			Vertices[1] = { 1.f , 0.f };						//UV Coords X
			Vertices[2] = { Origin.X  , -Origin.Y };			//Bottom right
			Vertices[3] = { 1.f, 1.f };							//UV Coords XY
			Vertices[4] = { -Origin.X, -Origin.Y };				//Bottom left
			Vertices[5] = { 0.f , 1.f };						//Y
			Vertices[6] = { -Origin.X, Origin.Y };				//Top left
			Vertices[7] = { 0.f, 0.f };							//0
			int NumVertices = 6;

			glGenVertexArrays(1, &VertexArray);
			glBindVertexArray(VertexArray);
			glGenBuffers(1, &VertexBuffer);
			glGenBuffers(1, &ElementBuffer);

			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vect2) * 8, Vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

			//Draw it
			glUniform2f(Shader.Viewport, 2.f / (float)RenderBuffer::WindowFrame->Size.X, 2.f / (float)RenderBuffer::WindowFrame->Size.Y);
			
			glUniformMatrix2fv(Shader.Rotation, 1, GL_FALSE, (const GLfloat*)Mat2().M);
			glUniform2f(Shader.Position, (float)CaretPos, (Size.Y - CaretY) - FontMap->Height - NegOffset);
			glUniform2f(Shader.RectSize, FontSize.X, FontSize.Y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FontMap->CharMap[S[i]].TextureID);
			glUniform4f(Shader.Color, 0.f, 0.f, 0.f, 1.f);
			glUniform2f(Shader.UVCoords, 0, 0);
			CaretPos += FontMap->CharMap[S[i]].Advance;

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
			glDeleteVertexArrays(1, &VertexArray);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, RenderBuffer::CurrentFrame->FBO);
	}
	glBindVertexArray(0);
}

Texture* TextBuffer::GetTexture()
{
	return &Tx;
}

void TextBuffer::Init()
{

}

void TextBuffer::DriverInit()
{

}

void TextBuffer::Detach()
{
	--References;
	if (!References)
	{
		TextPool.erase(Hash);
		delete this;
	}
}

TextBuffer* TextBuffer::GetDrawableUTF8Text(String S, String FontName, int Height)
{
	Map8* Font = Font::LoadTrueTypeFont8(FontName.Tochar(), Height);
	
	if (!Font)		//-- If selected font doesn't exist, select a default one.
		Font = Font::LoadTrueTypeFont8("Fonts/arial.ttf", Height);

	HASH HName = HashName(S) + Font->FontName;

	//Check if it already exists
	auto it = TextPool.find(HName);
	if (it != TextPool.end())
	{
		++it->second->References;
		return it->second;
	}

	TextBuffer* TB = new TextBuffer(S, Font);
	++TB->References;
	return TB;
}

TextBuffer* TextBuffer::GetDrawableUTF8Text(String S, Map8* Font)
{
	HASH HName = HashName(S) + Font->FontName;

	//Check if it already exists
	auto it = TextPool.find(HName);
	if (it != TextPool.end())
	{
		++it->second->References;
		return it->second;
	}

	TextBuffer* TB = new TextBuffer(S, Font);
	++TB->References;
	return TB;
}
