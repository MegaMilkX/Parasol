#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#include <map>
#include <set>
#include <vector>

#include "macro/macro_readers_def.h"

#include "gfxmesh.h"
#include "gfxmaterial.h"
#include "gfxtexture2d.h"

#include "math3f.h"

#include "external/freetype/include/ft2build.h"
#include "external/freetype/include/freetype/freetype.h"
#include "external/freetype/include/freetype/ftglyph.h"

#include "bitmap-pack/bitmap-pack.h"

class GFXString;
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

	struct Vertex
	{
		VERTEX
		(
			(VertexAttrib::Position<0>) position,
			(VertexAttrib::RGBA<0>) color,
			(VertexAttrib::UV<0>) uv
		)
	};

	enum ALIGN
	{
		LEFT = 1,
		RIGHT = 2,
		TOP = 4,
		BOTTOM = 8,
		HCENTER = LEFT | RIGHT,
		VCENTER = TOP | BOTTOM
	};

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

	class GFXFontGlyphCompare
	{
	public:
		bool operator()(const GFXFont::Glyph& a, const GFXFont::Glyph& b) const
		{
			if (a.size != b.size)
				return a.size < b.size;
			else
				return a.index < b.index;
		}
	};

	Glyph GetGlyph(uint32_t character, int size);

	void MakeString(GFXString* gfxstring, const std::string& string, unsigned char size, int align = LEFT | TOP);

	void DebugDumpAtlasPNG(unsigned char size, const std::string& filename);

	void Bind(unsigned char size);
	ResHdl<GFXTexture2D> GetAtlas(unsigned char size) { return atlases[size]; }

	friend GFXString;
private:
	static FT_Face _LoadDefaultFace();
	static ResHdl<GFXMaterial> _GetDefaultShader();

	void _RegString(GFXString* ptr) { strings.insert(ptr); }
	void _UnregString(GFXString* ptr) { strings.erase(ptr); }

	std::set<GFXString*> strings;

	FT_Library ftlib;
    FT_Face face;
	std::set<GFXFont::Glyph, GFXFontGlyphCompare> glyphs;
	ResHdl<GFXMaterial> material;
	std::map<int, BitmapPack> packs;
    std::map<int, ResHdl<GFXTexture2D>> atlases;
	std::map<int, bool> atlas_dirty;
};

#endif