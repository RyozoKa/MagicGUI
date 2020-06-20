
#include "Template.h"
#include <cstdio>

void GShader::InitializeShader(const char* VertexShader, const char* FragmentShader)
{
	ID = glCreateProgram();

	//Vertex
	FILE* F = fopen(VertexShader, "rb");
	fseek(F, 0L, SEEK_END);
	size_t Size = ftell(F);
	rewind(F);
	unsigned char* Data = (unsigned char*)malloc(Size + 1);
	fread(Data, 1, Size, F);
	Data[Size] = 0;
	Vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vert, 1, (const GLchar* const*) &Data, nullptr);
	glCompileShader(Vert);
	fclose(F);
	free(Data);
	int res;
	char Msg[512];
	glGetShaderiv(Vert, GL_COMPILE_STATUS, &res);
	if (!res)
	{
		int Len;

		glGetShaderiv(Vert, GL_INFO_LOG_LENGTH, &Len);
		glGetShaderInfoLog(Vert, Len, &Len, Msg);
		
		ErrorMessage(Msg, "Vertex Shader error");
		exit(-1);
	}

	//Fragment
	F = fopen(FragmentShader, "rb");
	fseek(F, 0L, SEEK_END);
	Size = ftell(F);
	rewind(F);
	Data = (unsigned char*)malloc(Size + 1);
	fread(Data, 1, Size, F);
	Data[Size] = 0;
	Frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Frag, 1, (const GLchar* const*) &Data, nullptr);
	glCompileShader(Frag);
	fclose(F);
	free(Data);
	glGetShaderiv(Frag, GL_COMPILE_STATUS, &res);
	if (!res)
	{
		int Len;
		glGetShaderiv(Frag, GL_INFO_LOG_LENGTH, &Len);
		glGetShaderInfoLog(Frag, Len, &Len, Msg);
		
		ErrorMessage(Msg, "Fragment Shader error");
		exit(-1);
	}

	glAttachShader(ID, Vert);
	glAttachShader(ID, Frag);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &res);
	if(!res) 
	{
		glGetProgramInfoLog(ID, 512, NULL, Msg);
		ErrorMessage(Msg, "Shader link error");
		return;
	}
	glValidateProgram(ID);
	glUseProgram(ID);	//Won't change.
	glDeleteShader(Frag);
	glDeleteShader(Vert);

	Viewport = glGetUniformLocation(ID, "Viewport");
	Position = glGetUniformLocation(ID, "Position");
	Rotation = glGetUniformLocation(ID, "Rotation");
	RectSize = glGetUniformLocation(ID, "Size");

	Color = glGetUniformLocation(ID, "Color");
	bValid = true;
}

GShader Shader;