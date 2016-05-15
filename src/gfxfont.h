#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#include <map>
#include <set>
#include <vector>

#include "macro/macro_readers_def.h"

#include "gfxtexture2d.h"

#include "external/freetype/include/ft2build.h"
#include "external/freetype/include/freetype/freetype.h"
#include "external/freetype/include/freetype/ftglyph.h"

#include "external/binpack2d.h"

struct GFXFontGlyph
{
    uint32_t index;
    int size;
    float h_bearing_x, h_bearing_y;
    float v_bearing_x, v_bearing_y;
    float advance_x, advance_y;
    float w, h;
    unsigned int bitmap_x, bitmap_y;
};

class GFXFontGlyphCompare
{
public:
    bool operator()(GFXFontGlyph& a, GFXFontGlyph& b)
    {
        if (a.size != b.size)
            return a.size < b.size;
        else
            return a.index < b.index;
    }
};

class GFXFont
{
public:
    READERS
    (
        (ReadTTF) "ttf"
    )
    bool ReadTTF(File file);

    static GFXFont Create();

    GFXFontGlyph& Glyph(uint32_t character, int size);

    struct Bitmap
    {
        void* data;
        unsigned int w, h;
        unsigned char bpp;
    };
private:
    bp2D::BinPacker2D packer;
    FT_Face face;
    std::map<uint32_t, GFXFontGlyph> glyphs;
    std::map<uint32_t, Bitmap> bitmaps;
    std::map<int, GFXTexture2D> atlases;
};

#endif