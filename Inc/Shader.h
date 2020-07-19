#pragma once

/*
	Basic Shader object
	Revision:
		Christopher Békési
*/

class MAGICGUIAPI GShader
{
public:
	void InitializeShader(const char* VertexShader, const char* FragmentShader);

	unsigned int ID;
	unsigned int Vert;
	unsigned int Frag;

	//Vertex uniforms
	unsigned int Position;
	unsigned int Viewport;
	unsigned int Rotation;
	unsigned int RectSize;

	//Fragment uniforms
	unsigned int Color;
	unsigned int ColorOffset;
	unsigned int UVCoords;

	bool bValid;	//False by default

	GShader()
	{
		bValid = false;
	}	//This lets us initialize the shader once the context is available
	
};


extern GShader Shader;