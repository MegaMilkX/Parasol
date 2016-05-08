#ifndef _GFXTEXTURE2D_H_
#define _GFXTEXTURE2D_H_

#include "glextutil.h"

#include "macro\macro_readers_def.h"

class GFXTexture2D
{
public:
    READERS
    (
        (ReadPNG) "png"
    )
    bool ReadPNG(File file);
    
    static GFXTexture2D Create();
    
    void Use(unsigned int layer = 0);
private:
  unsigned int buffer;
};

#endif