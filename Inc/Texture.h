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

	unsigned int RenderMode;
	unsigned char TextureMode;

	unsigned int VertexBuffer = 0; 		
	unsigned int VertexArray = 0;
	unsigned int ElementBuffer = 0;
	
	float Red, Green, Blue, Alpha;
	Texture* Image;
	bool bPending;					//--	This is set initially, in case the driver has not yet been initialized. Any upload to the GPU needs to be delayed until DriverStartup() is called.

	unsigned int	ShaderProgram;	//--	Shader program used for this RenderObject

	class Widget* Owner;

	//RenderObject registry
	static RenderObject* RenderItems[5120];	//More than enough
	static unsigned short RenderIdx;

	//Store GPU data locally too.
	Vect2 * Vertices;
	unsigned int NumVertices;
	
	//Quad primitive
	Vect2 Size;


	void SetImage(const char* File, unsigned char TM);

	void SetImage(HASH ID);

	void SetImage(Texture* Tx);

	void SetColor(const Color _Col);

	void SetRenderMode(TYPE Mode);

	void SetPrimitive(Vect2 Prim);

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

	//-- Location: Absolute position for the entire object
	//-- Pos: Scissorbox segment position
	//-- Size: Scissorbox size
	void DrawSegment(const Vect2 Location, const Vect2 Pos, const Vect2 Size);
	

	
};

//Render specific statics

//Current scissor box, allows for two nested boxes.
//This should be redone to simulate multiple scissor boxes later
extern Vect2 ScissorSize;
extern Vect2 ScissorPos;
extern bool bScissorState;


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