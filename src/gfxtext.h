#ifndef _GFXTEXT_H_
#define _GFXTEXT_H_

#include "gfxfont.h"

class GFXText
{
public:
    static GFXText Create(std::string font_name, int glyph_size);
};

#endif