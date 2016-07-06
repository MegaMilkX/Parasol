#include "gs-render-rework.h"

void GSRenderRework::OnInit()
{
    camera = scene.Add<Camera>();
    camera->Perspective(1.5f, 1280, 720, 0.1f, 100.0f);
}

void GSRenderRework::OnSwitch()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    InputShowCursor(true);
}

void GSRenderRework::OnUpdate()
{

}

void GSRenderRework::OnRender()
{
    camera->Render();
}

void GSRenderRework::OnInput(InputEvent& e)
{

}

void GSRenderRework::OnCleanup()
{

}
