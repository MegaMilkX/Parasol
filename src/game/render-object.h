#ifndef _RENDER_OBJECT_H_
#define _RENDER_OBJECT_H_

#include <libgame.h>

struct RenderObject
{
    mat4f transform;
    ResHdl<GFXMaterial> material;
    ResHdl<GFXMesh> mesh;
};

#endif