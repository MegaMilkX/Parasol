#ifndef _GFXTEXTURE2D_H_
#define _GFXTEXTURE2D_H_

#include "glextutil.h"

#include "macro\macro_readers_def.h"

#include "math3f.h"

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

	vec2i GetDimensions() { return dim; }
	unsigned char GetBPP() { return bpp; }
private:
  unsigned int buffer;
  vec2i dim;
  unsigned char bpp;
};

#endif