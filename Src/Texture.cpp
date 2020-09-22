
#include "Window.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <stdlib.h>
#include <immintrin.h>

google::dense_hash_map<HASH, Texture*> Texture::_HashTable;
//unsigned short RenderObject::RenderIdx;
//RenderObject* RenderObject::RenderItems[5120];
int Texture::EMPTY = -1;

RenderState CurrentState = RenderState::STATE_Normal;

Texture * Texture::CreateTexture(const char * Path)
{
	HASH ID = HashName(Path);
	auto it = _HashTable.find(ID);
	if (it != _HashTable.end())
	{
		return it->second;	//Don't load a texture that already exists
		printf("Reusing texture\n");
	}
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load(Path, &width, &height, &nChannels, 0); 
	if (!Data)
	{
		std::cout << "Texture: Failed to create texture: Failed to load " << Path << "\n";
		return nullptr;
	}
	printf("Creating new texture\n");
	Texture* Tx = new Texture();//Window::Interface->CreateTexture(data, width, height, nrChannels);
	Tx->Data = Data;
	Tx->TextureID = ID;
	Tx->Width = width;
	Tx->Height = height;
	Tx->Channels = nChannels;

	if (!Tx)
		return nullptr;
	if (Application::bDriverInit)
	{
		glGenTextures(1, (GLuint*)&Tx->_InternalID);
		glBindTexture(GL_TEXTURE_2D, (GLuint)Tx->_InternalID);
		//These are probably set by default
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (nChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		else if(nChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		Tx->bPending = true;
	_HashTable[Tx->TextureID] = Tx;
	return Tx;
}

Texture* Texture::GetTexture(HASH ID)
{
	auto it = _HashTable.find(ID);
	if (it != _HashTable.end())
		return it->second;
	return nullptr;
}

//Event called when driver is fully initialized.

void Texture::DriverStartup()
{
	//Upload texture Data
	if (bPending)
	{
		glGenTextures(1, (GLuint*)&_InternalID);
		glBindTexture(GL_TEXTURE_2D, (GLuint)_InternalID);
		//These are probably set by default
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (Channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		else if (Channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		bPending = false;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, (GLuint*)&_InternalID);
	_HashTable.erase(TextureID);
}


//Event called when driver is fully initialized.

void RenderObject::SetImage(const char* File, unsigned char TM)
{
	Image = Texture::CreateTexture(File);
	TextureMode = TM;
}

void RenderObject::SetImage(HASH ID)
{
	Image = Texture::GetTexture(ID);
	RenderMode |= TYPE::TYPE_TEXTURE;
}

void RenderObject::SetImage(Texture* Tx)
{
	Image = Tx;
	RenderMode |= TYPE::TYPE_TEXTURE;
}

void RenderObject::SetColor(const Color _Col)
{
	Red = (float)_Col.Red / 255.f;
	Green = (float)_Col.Green / 255.f;
	Blue = (float)_Col.Blue / 255.f;
	Alpha = (float)_Col.Alpha / 255.f;
	RenderMode |= TYPE::TYPE_COLOR;
}

void RenderObject::SetRenderMode(TYPE Mode)
{
	RenderMode = Mode;
}

void RenderObject::SetPrimitive(Vect2 Prim)
{
	Size = Prim;

	if (Vertices)
		free(Vertices);	//Clear any potentially old vertices
						//Relative positioning.
	//Adjust origin
	Vect2 Origin = Prim / 2;

	Vertices = (Vect2*)malloc(sizeof(Vect2) * 8);
	Vertices[0] = Origin;
	Vertices[1] = { 1.f, 1.f };	//UV Coords
	Vertices[2] = { Origin.X  , -Origin.Y };
	Vertices[3] = { 1.f, 0.f };	//UV Coords
	Vertices[4] = { -Origin.X, -Origin.Y };
	Vertices[5] = { 0.f, 0.f };
	Vertices[6] = { -Origin.X, Origin.Y };
	Vertices[7] = { 0.f, 1.f };

	/*Vertices[0] = Prim - Origin;
	Vertices[1] = { 1.f, 1.f };	//UV Coords
	Vertices[2] = { Prim.X  , 0.f };
	Vertices[3] = { 1.f, 0.f };	//UV Coords
	Vertices[4] = { 0.f, 0.f };
	Vertices[5] = { 0.f, 0.f };
	Vertices[6] = { 0.f, Prim.Y };
	Vertices[7] = { 0.f, 1.f };*/
	NumVertices = 6;

	//Default rectangle
	unsigned int indices[] =
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	if (Application::bDriverInit)
	{
		if (!VertexBuffer /*Don't need to check the others*/)
		{
			glGenVertexArrays(1, &VertexArray);
			glBindVertexArray(VertexArray);
			glGenBuffers(1, &VertexBuffer);
			glGenBuffers(1, &ElementBuffer);

			
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vect2) * 8, Vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else 
		{
			glBindVertexArray(VertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vect2) * 8, Vertices);
		}
	}
	else
		bPending = true;

	glBindVertexArray(0);

}

void RenderObject::SetPrimitive(Vect2 Prim, Vect2 UVSize)
{
	Size = Prim;
	UVCoordsClip.Y = 1.f - (Size.Y / UVSize.Y);
	UVCoordsClip.X = (Size.X / UVSize.X);

	if (Vertices)
		free(Vertices);	//Clear any potentially old vertices
						//Relative positioning.
	//Adjust origin
	Vect2 Origin = Prim / 2;

	Vertices = (Vect2*)malloc(sizeof(Vect2) * 8);
	Vertices[0] = Origin;								//Top right
	Vertices[1] = { UVCoordsClip.X , 1.f };				//UV Coords X
	Vertices[2] = { Origin.X  , -Origin.Y };			//Bottom right
	Vertices[3] = { UVCoordsClip.X, UVCoordsClip.Y };	//UV Coords XY
	Vertices[4] = { -Origin.X, -Origin.Y };				//Bottom left
	Vertices[5] = { 0.f , UVCoordsClip.Y };				//Y
	Vertices[6] = { -Origin.X, Origin.Y };				//Top left
	Vertices[7] = { 0.f, 1.f };							//0
	NumVertices = 6;

	//Default rectangle
	unsigned int indices[] =
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	if (Application::bDriverInit)
	{
		if (!VertexBuffer /*Don't need to check the others*/)
		{
			glGenVertexArrays(1, &VertexArray);
			glBindVertexArray(VertexArray);
			glGenBuffers(1, &VertexBuffer);
			glGenBuffers(1, &ElementBuffer);

			
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vect2) * 8, Vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else 
		{
			glBindVertexArray(VertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vect2) * 8, Vertices);
		}
	}
	else
		bPending = true;

	glBindVertexArray(0);
}

void RenderObject::SetTextureClipping(Vect2 UVSize)
{
	UVCoordsClip.Y = 1.f - (Size.Y / UVSize.Y);
	UVCoordsClip.X = (Size.X / UVSize.X);
	if (Vertices)
		free(Vertices);	//Clear any potentially old vertices
						//Relative positioning.
	//Adjust origin
	Vect2 Origin = Size / 2;

	Vertices = (Vect2*)malloc(sizeof(Vect2) * 8);
	Vertices[0] = Origin;								//Top right
	Vertices[1] = { UVCoordsClip.X , 1.f };				//UV Coords X
	Vertices[2] = { Origin.X  , -Origin.Y };			//Bottom right
	Vertices[3] = { UVCoordsClip.X, UVCoordsClip.Y };	//UV Coords XY
	Vertices[4] = { -Origin.X, -Origin.Y };				//Bottom left
	Vertices[5] = { 0.f , UVCoordsClip.Y };				//Y
	Vertices[6] = { -Origin.X, Origin.Y };				//Top left
	Vertices[7] = { 0.f, 1.f };							//0
	NumVertices = 6;

	//Default rectangle
	unsigned int indices[] =
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	if (Application::bDriverInit)
	{
		if (!VertexBuffer /*Don't need to check the others*/)
		{
			glGenVertexArrays(1, &VertexArray);
			glBindVertexArray(VertexArray);
			glGenBuffers(1, &VertexBuffer);
			glGenBuffers(1, &ElementBuffer);

			
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vect2) * 8, Vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else 
		{
			glBindVertexArray(VertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vect2) * 8, Vertices);
		}
	}
	else
		bPending = true;

	glBindVertexArray(0);
}

void RenderObject::DriverStartup()
{
	//Upload vertex data.
	if (bPending && Vertices)
	{
		//Default rectangle
		unsigned int indices[] =
		{  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		if (!VertexBuffer /*Don't need to check the others*/)
		{
			glGenVertexArrays(1, &VertexArray);
			glBindVertexArray(VertexArray);
			glGenBuffers(1, &VertexBuffer);
			glGenBuffers(1, &ElementBuffer);


			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vect2) * 8, Vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else
		{
			glBindVertexArray(VertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vect2) * 8, Vertices);
		}
		glBindVertexArray(0);
	}

	


}


//Size
//Location
//Material
void RenderObject::DrawObject(const Vect2 Pos)
{
	if (RenderMode == TYPE::TYPE_NONE || (RenderMode != TYPE::TYPE_TEXTURE && Alpha == 0.f))
		return;
	//glfwMakeContextCurrent(Owner->Window->WindowHandle);
	//glViewport(0, 0, Owner->Window->WindowSize.X, Owner->Window->WindowSize.Y);
	
	
	Vect2 Location = Pos;
	Location.RoundDown();

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glUniform3f(Shader.ColorOffset, RedOffset, GreenOffset, BlueOffset);
	glUniform2f(Shader.Viewport, 2.f / (float)RenderBuffer::CurrentFrame->Size.X, 2.f / (float)RenderBuffer::CurrentFrame->Size.Y);
	glUniformMatrix2fv(Shader.Rotation, 1, GL_FALSE, (const GLfloat*)Owner->RotMatrix.M);
	glUniform2f(Shader.Position, Location.X, (RenderBuffer::CurrentFrame->Size.Y - Location.Y) - Size.Y);
	glUniform2f(Shader.RectSize, Size.X, Size.Y);
	
	switch (RenderMode)
	{
	case TYPE::TYPE_TEXTURE:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Image->_InternalID);
		glUniform4f(Shader.Color, 1.f, 1.f, 1.f, 1.f);
		glUniform2f(Shader.UVCoords, TexOffset.X / Image->Width, TexOffset.Y / Image->Height);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	case TYPE::TYPE_BLEND: 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Image->_InternalID);
		glUniform4f(Shader.Color, Red, Green, Blue, Alpha);
		glUniform2f(Shader.UVCoords, TexOffset.X / Image->Width, TexOffset.Y / Image->Height);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	case TYPE::TYPE_COLOR:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture::EMPTY);
		glUniform4f(Shader.Color, Red, Green, Blue, Alpha);
		glUniform2f(Shader.UVCoords, 0, 0);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	}

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
	//glFlush();
	glBindVertexArray(0);
	RenderBuffer::CurrentFrame->bUpdate = true;
}

void RenderObject::DrawObject(const Vect2 Pos, const Mat2 Rotation)
{
	if (RenderMode == TYPE::TYPE_NONE || (RenderMode != TYPE::TYPE_TEXTURE && Alpha == 0.f))
		return;
	//glfwMakeContextCurrent(Owner->Window->WindowHandle);
	//glViewport(0, 0, Owner->Window->WindowSize.X, Owner->Window->WindowSize.Y);
	
	
	Vect2 Location = Pos;
	Location.RoundDown();

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glUniform3f(Shader.ColorOffset, RedOffset, GreenOffset, BlueOffset);
	glUniform2f(Shader.Viewport, 2.f / (float)RenderBuffer::CurrentFrame->Size.X, 2.f / (float)RenderBuffer::CurrentFrame->Size.Y);
	glUniformMatrix2fv(Shader.Rotation, 1, GL_FALSE, (const GLfloat*)Rotation.M);
	glUniform2f(Shader.Position, Location.X, (RenderBuffer::CurrentFrame->Size.Y - Location.Y) - Size.Y);
	glUniform2f(Shader.RectSize, Size.X, Size.Y);
	
	switch (RenderMode)
	{
	case TYPE::TYPE_TEXTURE:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Image->_InternalID);
		glUniform4f(Shader.Color, 1.f, 1.f, 1.f, 1.f);
		glUniform2f(Shader.UVCoords, TexOffset.X / Image->Width, TexOffset.Y / Image->Height);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	case TYPE::TYPE_BLEND: 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Image->_InternalID);
		glUniform4f(Shader.Color, Red, Green, Blue, Alpha);
		glUniform2f(Shader.UVCoords, TexOffset.X / Image->Width, TexOffset.Y / Image->Height);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	case TYPE::TYPE_COLOR:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture::EMPTY);
		glUniform4f(Shader.Color, Red, Green, Blue, Alpha);
		glUniform2f(Shader.UVCoords, 0, 0);		//Texture coordinates are always size neutral scalars.																		//-- UV offsets have to be in the range of 0 - 1			

		break;
	}

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
	//glFlush();
	glBindVertexArray(0);
	RenderBuffer::CurrentFrame->bUpdate = true;
}
