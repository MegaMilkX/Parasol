#ifndef _SCENE_SYSTEM_H_
#define _SCENE_SYSTEM_H_

#include "scene.h"

class ISceneSystem
{
public:
    ISceneSystem() {}
    virtual ~ISceneSystem() {}
};


#define SCENE_SYS_BODY(NAME, BASE) \
    public: \
    NAME(Scene* scene) : BASE(scene) \
    { \
        scene->AddSystemInstance(this); \
        scene->AddSystemType(TypeInfo<NAME>::GetId(), this); \
    }


#endif