#include "gfxtexture2d.h"

#include <vector>
#include <SOIL\SOIL.h>
#include "math3f.h"

#include <iostream>

GFXTexture2D GFXTexture2D::Create()
{
    GFXTexture2D texture;
    glGenTextures(1, &texture.buffer);
    return texture;
}

bool GFXTexture2D::ReadPNG(File file)
{
    file.Seek(0, File::BEGIN);
    unsigned int bytesRead;
    unsigned char* data = file.Read(file.Size(), bytesRead);
    if(!data || !bytesRead)
        return false;
    
    int width, height, channels = 0;
    int force_channels = 4;
    
    unsigned char* img = SOIL_load_image_from_memory(
                                data, bytesRead,
                                &width, &height, &channels,
                                force_channels );
                                
    if(!img)
        return false;
    
    unsigned int format = 0;
    switch( force_channels )
    {
    case 1:
        format = GL_LUMINANCE;
        break;
    case 2:
        format = GL_LUMINANCE_ALPHA;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data( img );
    
    return true;
}

void GFXTexture2D::Use(unsigned int layer)
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GL_TEXTURE_2D, buffer);
}