#pragma once

/*
	Class that handles basic 2D textures to be rendered by a shader

	Call example:
	


	Revision:
	2020-03-14 Christopher Békési
*/

//#include "google/dense_hash_map.h"
#include "Template.h"
#include <google/dense_hash_map.h>

//Render states. For now, this keeps track of the current render method
//A segmented rendering will already have all of the lower level objects updated'
//A normal draw will have to redraw the underlying objects if the current object is
enum RenderState
{
	STATE_Segmented,
	STATE_Normal
} extern CurrentState;

class MAGICGUIAPI Texture
{
public:

	int _InternalID;		//--	Texture index on the GPU Mem
	HASH	TextureID;		//--	Texture hash ID
	bool bPending;			//--	See RenderObject for details.

	//Image data
	unsigned char* Data;	//--	Frame buffer
	unsigned short Height;
	unsigned short Width;
	unsigned char Channels;

	//Static data
	static int EMPTY;
	
	static google::dense_hash_map<HASH, Texture*> _HashTable;	//--	Internal hash table to keep track of loaded textures and avoid duplicates.

	//Illegals
	Texture& operator=	(Texture& T);
	Texture				(Texture&);

	
	//Loads a new texture using the filepath as argument if it doesn't already exist 
	static Texture* CreateTexture(const char* Path);
	//Return an already existing texture, or null if none
	static Texture* GetTexture(HASH ID);

	//Event called when driver is fully initialized.
	void DriverStartup();

	HASH GetID()
	{
		return TextureID;
	}
	~Texture();
	Texture()
	{
		bPending = false;
	};	//--	Don't use this to create a new texture!
	
};

enum TYPE
{
	TYPE_NONE = 0,
	TYPE_TEXTURE = 1,
	TYPE_COLOR = 2,
	TYPE_BLEND = TYPE_TEXTURE | TYPE_COLOR
};

/*
	Class containing drawing modes and stores meta data for various primitives
	This can be used in any object that has something to render. 2D and 3D
*/
class MAGICGUIAPI RenderObject
{
	public:

	unsigned int RenderMode = TYPE::TYPE_NONE;
	unsigned char TextureMode;

	unsigned int VertexBuffer = 0; 		
	unsigned int VertexArray = 0;
	unsigned int ElementBuffer = 0;
	
	float Red = 0.f, Green = 0.f, Blue = 0.f, Alpha = 1.f;
	Texture* Image = NULL;
	bool bPending;					//--	This is set initially, in case the driver has not yet been initialized. Any upload to the GPU needs to be delayed until DriverStartup() is called.
	float RedOffset = 0.f, GreenOffset = 0.f, BlueOffset = 0.f;				//--	Color addition for brightening a texture
	//Not currently used
	unsigned int	ShaderProgram;	//--	Shader program used for this RenderObject

	class Widget* Owner;

	

	//Store GPU data locally too.
	Vect2 * Vertices;
	unsigned int NumVertices;
	
	//Quad primitive
	Vect2 Size;

	//UV offset
	Vect2 TexOffset = { 0.f, 0.f };

	//UV coords
	Vect2 UVCoordsClip = { 0.f , 0.f }; //Lower left texture clipping

	void SetImage(const char* File, unsigned char TM);	//-- Relic!!

	void SetImage(HASH ID);

	void SetImage(Texture* Tx);

	void SetColor(const Color _Col);

	void SetRenderMode(TYPE Mode);

	void SetPrimitive(Vect2 Prim);
	void SetPrimitive(Vect2 Prim, Vect2 UVSize);
	void SetTextureClipping(Vect2 UVSize);

	//Constructor
	RenderObject()
	{
		bPending = false;
		RenderMode = TYPE::TYPE_NONE;
		Vertices = nullptr;
	}

	//Event called when driver is fully initialized.
	void DriverStartup();

	void DrawObject(const Vect2 Location);	
};

//Render specific statics

//Current scissor box, allows for two nested boxes.
//This should be redone to simulate multiple scissor boxes later


/*
	DrawSegment:
	Scissor
	Enable
		if(CurrentScissorEnd > NextScissorEnd)
			CreateScissorBox

		Draw

		if(!bScissorState)
		Disable
		else
		RestoreBox
	Zero Nest
	Disable



*/