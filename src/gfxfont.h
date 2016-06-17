#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#include <map>
#include <set>
#include <vector>

#include "macro/macro_readers_def.h"

#include "gfxmesh.h"
#include "gfxshader.h"
#include "gfxtexture2d.h"

#include "math3f.h"

#include "external/freetype/include/ft2build.h"
#include "external/freetype/include/freetype/freetype.h"
#include "external/freetype/include/freetype/ftglyph.h"

#include "bitmap-pack/bitmap-pack.h"

class GFXFont
{
public:
    READERS
    (
        (ReadTTF) "ttf"
    )
    bool ReadTTF(File file);

    static GFXFont Create();

	GFXFont();

	struct Glyph
	{
		uint32_t index;
		int size;
		float h_bearing_x, h_bearing_y;
		float v_bearing_x, v_bearing_y;
		float advance_x, advance_y;
		float w, h;
		unsigned int bitmap_x, bitmap_y;
		unsigned int bitmap_index;
		vec2f uv_begin;
		vec2f uv_end;
	};

	Glyph* GetGlyph(uint32_t character, int size);

	GFXMesh MakeString(const std::string& string, unsigned char size);

	void DebugDumpAtlasPNG();

	void Bind(unsigned char size);
	GFXTexture2D GetAtlas(unsigned char size) { return atlases[size]; }
private:
	static FT_Face _LoadDefaultFace();
	static GFXShader _GetDefaultShader();

	BitmapPack pack;
	FT_Library ftlib;
    FT_Face face;
    std::map<uint32_t, GFXFont::Glyph> glyphs;
	GFXShader shader;
    std::map<int, GFXTexture2D> atlases;
};

class GFXFontGlyphCompare
{
public:
	bool operator()(GFXFont::Glyph& a, GFXFont::Glyph& b)
	{
		if (a.size != b.size)
			return a.size < b.size;
		else
			return a.index < b.index;
	}
};

#endif