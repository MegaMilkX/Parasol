#ifndef _GFX_LINE_H_
#define _GFX_LINE_H_

#include "math3f.h"
#include <vector>
#include "geometrybuffer.h"
#include "gfxshader.h"

class GFXLine
{
public:
    GFXLine();
    void Render(std::vector<vec3f>& points);
private:
    GFXShader shader;
    GeometryBuffer vertex_buffer;
};

#endif