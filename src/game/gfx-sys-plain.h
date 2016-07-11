#ifndef _GFX_SYS_PLAIN_H_
#define _GFX_SYS_PLAIN_H_

#include "gfx-scene-system.h"

class GFXSysPlain : public IGFXSceneSystem
{
    SCENE_SYS_BODY(GFXSysPlain, IGFXSceneSystem)
public:
    void Render(const mat4f& projection, const mat4f& view)
    {
        
    }
private:
    std::vector<GFXObject> objects;
    std::vector<GFXObject> visible_cache;
};

#endif