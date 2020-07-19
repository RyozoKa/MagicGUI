#include "Font.h"
#include "HIO.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

dense_hash_map<HASH, Map16*> Font::UTF16Maps;
dense_hash_map<HASH, Map8*> Font::UTF8Maps;

Map8* Font::LoadTrueTypeFont8(const char * FontPath, int Height)
{
    HASH HName = HashName(FontPath) + Height;

    //Mitigate duplicates.
    auto it = UTF8Maps.find(HName);
    if (it != UTF8Maps.end())
        return it->second;

    if (!FileBase::Exists(FontPath))
    {
        std::cout << "ERROR::FREETYPE: Font " << FontPath << ": File not found" << std::endl;
        return nullptr;

    }
	FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return nullptr;
    }

    FT_Face face;
    if (FT_New_Face(ft, FontPath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, 0, Height); 
  
    Map8* Map = new Map8;
    Map->Height = Height;
    Map->FontName = HName;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    for (unsigned char c = 0; c < 255; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, 
            Vect2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
            Vect2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x >> 6
        };
        Map->CharMap[c] = character;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
    UTF8Maps[HName] = Map;
    return Map;
}

Map16* Font::LoadTrueTypeFont16(const char * FontPath, int Height)
{
    

    HASH HName = HashName(FontPath) + Height;

    //Mitigate duplicates.
    auto it = UTF16Maps.find(HName);
    if (it != UTF16Maps.end())
        return it->second;

    if (!FileBase::Exists(FontPath))
    {
        std::cout << "ERROR::FREETYPE: Font " << FontPath << ": File not found" << std::endl;
        return nullptr;
    }

	FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return nullptr;
    }

    FT_Face face;
    if (FT_New_Face(ft, FontPath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, 0, Height);  

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
    Map16* Map = new Map16;
    Map->Height = Height;
    Map->FontName = HName;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    for (unsigned short c = 0; c < face->num_glyphs; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, 
            Vect2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
            Vect2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x >> 6
        };
        Map->CharMap[c] = character;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
    UTF16Maps[HName] = Map;
    return Map;
}
