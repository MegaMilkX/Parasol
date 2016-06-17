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

    unsigned char* img_flipped = new unsigned char[width * height * 4];
    for (int i = 0, k = width * height - width; i < width * height; i += width, k -= width)
    {
        for (int j = 0; j < width; ++j)
        {
            img_flipped[(i + j) * 4] = img[(k + j) * 4];
            img_flipped[(i + j) * 4 + 1] = img[(k + j) * 4 + 1];
            img_flipped[(i + j) * 4 + 2] = img[(k + j) * 4 + 2];
            img_flipped[(i + j) * 4 + 3] = img[(k + j) * 4 + 3];
        }
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_flipped);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data( img );
    delete[] img_flipped;
    glFinish();
    
	dim.x = width;
	dim.y = height;
	bpp = 4;

    return true;
}

void GFXTexture2D::Bitmap(unsigned char* data, int w, int h, int bpp)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer);
    if (bpp == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    else if (bpp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (bpp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        return;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

	dim.x = w;
	dim.y = h;
	bpp = bpp;
}

void GFXTexture2D::Use(unsigned int layer)
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GL_TEXTURE_2D, buffer);
}