#ifndef _FONT_RASTERIZER_H_
#define _FONT_RASTERIZER_H_

class FontMap
{
public:
    static FontMap* Create(void* font_data, size_t size);
};

#endif