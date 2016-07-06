#ifndef _GS_RENDER_REWORK_H_
#define _GS_RENDER_REWORK_H_

#include "GameState.h"

#include "scene.h"
#include "camera.h"

class GSRenderRework : public GameState
{
public:
    void OnInit();
    void OnSwitch();
    void OnUpdate();
    void OnRender();
    void OnInput(InputEvent& e);

    void OnCleanup();
private:
    Scene scene;
    Camera* camera;
};

#endif