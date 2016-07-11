#ifndef _GFX_SCENE_SYSTEM_H_
#define _GFX_SCENE_SYSTEM_H_

#include "scene-system.h"

#include <libgame.h>

struct GFXObject
{
    mat4f transform;
    ResHdl<GFXMaterial> material;
    ResHdl<GFXMesh> mesh;
};

struct GFXLight
{

};

class IGFXSceneSystem : public ISceneSystem
{
public:
    IGFXSceneSystem(Scene* scene)
    {
        scene->AddSystemInstance(this);
        scene->AddSystemType(TypeInfo<IGFXSceneSystem>::GetId(), this);
    }

    virtual void Render(const mat4f& projection, const mat4f& view) = 0;
};

#endif