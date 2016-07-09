#ifndef _GFX_SCENE_SYSTEM_H_
#define _GFX_SCENE_SYSTEM_H_

#include "scene-system.h"

class IGFXSceneSystem : public ISceneSystem
{
public:
    IGFXSceneSystem(Scene* scene)
    {
        scene->AddSystemInstance(this);
        scene->AddSystemType(TypeInfo<IGFXSceneSystem>::GetId(), this);
    }
    
};

#endif