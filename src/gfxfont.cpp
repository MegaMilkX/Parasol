#include "gfxfont.h"

#include <iostream>

void InsertBitmapToBitmap(unsigned char *data, unsigned int w, unsigned int h,
    unsigned char* destination, unsigned int dest_w, unsigned int dest_h,
    unsigned int dest_x, unsigned int dest_y, unsigned char bpp = 1)
{
    if (dest_w < w || dest_h < h)
        return;

    int rowLen = w;
    int rowOffset = (dest_w - w);

    unsigned int dest_i = dest_x + dest_y*dest_w;

    for (int i = 0; i < w*h; i++)
    {
        int x = i - rowLen*(i / rowLen);
        int row = i / rowLen;

        for (int j = 0; j < bpp; j++)
        {
            destination[(dest_i + i + rowOffset*row)*bpp + j] = data[i*bpp + j];
        }
    }
}

bool GFXFont::ReadTTF(File file)
{
    file.Seek(0, File::BEGIN);
    unsigned int bytesRead;
    unsigned char* data = file.Read(file.Size(), bytesRead);
    if (!data || !bytesRead)
        return false;

    FT_Library ftlib;
    if (FT_Init_FreeType(&ftlib))
        return false;
    if (FT_New_Memory_Face(ftlib, data, bytesRead, 0, &face))
        return false;
    return true;
}

GFXFont GFXFont::Create()
{
    return GFXFont();
}

GFXFontGlyph& GFXFont::Glyph(uint32_t character, int size)
{
    FT_Set_Pixel_Sizes(face, 0, size);
    GFXFontGlyph glyph;
    glyph.index = character;
    glyph.size = size;
    std::map<uint32_t, GFXFontGlyph>::iterator it = glyphs.find(character);
    if (it != glyphs.end())
        return glyph = it->second;

    if (FT_Load_Char(face, character, FT_LOAD_RENDER))
        return glyph;

    glyph.advance_x = face->glyph->advance.x;
    glyph.advance_y = face->glyph->advance.y;
    glyph.h_bearing_x = face->glyph->metrics.horiBearingX;
    glyph.h_bearing_y = face->glyph->metrics.horiBearingY;
    glyph.v_bearing_x = face->glyph->metrics.vertBearingX;
    glyph.v_bearing_y = face->glyph->metrics.vertBearingY;

    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        std::cout << "FT_GLYPH_FORMAT_BITMAP" << std::endl;
        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
            return glyph;
    }

    glyph.w = face->glyph->bitmap.width;
    glyph.h = face->glyph->bitmap.rows;    
    
    glyphs.insert(std::make_pair(character, glyph));

    if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
    {
        Bitmap bmp;
        bmp.w = face->glyph->bitmap.width;
        bmp.h = face->glyph->bitmap.rows;
        bmp.bpp = 1;
        bmp.data = malloc(bmp.w * bmp.h);
        memcpy(bmp.data, face->glyph->bitmap.buffer, bmp.w * bmp.h);
        bitmaps.insert(std::make_pair(character, bmp));
        std::cout << "BMP: " << bmp.w << " " << bmp.h << std::endl;
    }

    it = glyphs.begin();
    for (it; it != glyphs.end(); ++it)
    {
        packer.AddRect(bp2D::BinRect(it->first, it->second.w, it->second.h));
    }

    packer.Pack();

    bp2D::BinRect root_rect = packer.GetRootRect();
    std::cout << "Result rect: " << root_rect.w << " " << root_rect.h << std::endl;
    std::vector<bp2D::BinRect> rects = packer.GetVolumes();

    Bitmap atlasbmp;
    atlasbmp.data = malloc(root_rect.w * root_rect.h);
    atlasbmp.bpp = 1;
    atlasbmp.w = root_rect.w;
    atlasbmp.h = root_rect.h;

    for (unsigned int i = 0; i < rects.size(); ++i)
    {
        GFXFontGlyph& g = glyphs[rects[i].id];
        g.bitmap_x = rects[i].x;
        g.bitmap_y = rects[i].y;

        InsertBitmapToBitmap((unsigned char*)bitmaps[rects[i].id].data, rects[i].w, rects[i].h, (unsigned char*)atlasbmp.data, atlasbmp.w, atlasbmp.h, rects[i].x, rects[i].y, 1);
    }

    GFXTexture2D& texture = atlases[size];
    
    texture.Bitmap((unsigned char*)atlasbmp.data, atlasbmp.w, atlasbmp.h, atlasbmp.bpp);

    free(atlasbmp.data);
    
    return glyph;
}