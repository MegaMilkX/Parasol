#include "gs-render-rework.h"

#include "gfx-sys-plain.h"

void GSRenderRework::OnInit()
{
    scene.AddSystem<GFXSysPlain>();

    camera = scene.Add<Camera>();
    camera->Perspective(1.5f, 1280, 720, 0.1f, 100.0f);
    camera->Translate(0.0f, 1.0f, 3.0f);

    Renderable* r = scene.Add<Renderable>();
    r->Mesh(Resource<GFXMesh>::Get("miku"));
    r->Material(Resource<GFXMaterial>::Get("material"));
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
