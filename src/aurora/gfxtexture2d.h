#ifndef _GFXTEXTURE2D_H_
#define _GFXTEXTURE2D_H_

#include "glextutil.h"

#include "macro\macro_readers_def.h"

#include "math3f.h"

#include "resource.h"

class GFXSpriteMap;
class GFXTexture2D
{
public:
    READERS
    (
        (ReadPNG) "png"
    )
    bool ReadPNG(File file);
    
    static GFXTexture2D Create();

    void Bitmap(unsigned char* data, int w, int h, int bpp);
    
    void Use(unsigned int layer = 0);

	vec2i GetDimensions() const { return dim; }
	unsigned char GetBPP() { return bpp; }

	ResHdl<GFXSpriteMap> SpriteMap() { return sprite_map; }
private:
	static int AddGFXSpriteMapPath();
	unsigned int buffer;
	vec2i dim;
	unsigned char bpp;
	ResHdl<GFXSpriteMap> sprite_map;
};

#endif