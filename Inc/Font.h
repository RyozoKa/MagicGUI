#pragma once

/*
	Basic font management class

	TODO: It would be nice to have the ability to precache static text as
	one whole texture with the text to be drawn.
	
	Revision
		Christopher Békési 2020-07-16
*/

#include "Template.h"
#include "HDynamicArray.h"

struct Character 
{
    unsigned int	TextureID;  // ID handle of the glyph texture
    Vect2			Size;       // Size of glyph
    Vect2			Bearing;    // Offset from baseline to left/top of glyph
    unsigned int	Advance;    // Offset to advance to next glyph
};

struct Map8
{
	Character CharMap[255];
	int Height;
	HASH FontName;
};

struct Map16
{
	Character CharMap[65535];
	int Height;
	HASH FontName;
};

class MAGICGUIAPI Font
{
public:

	static dense_hash_map<HASH, Map16*> UTF16Maps;
	static dense_hash_map<HASH, Map8*> UTF8Maps;

	static Map8* LoadTrueTypeFont8(const char * FontPath, int Height);
	static Map16* LoadTrueTypeFont16(const char * FontPath, int Height);

};