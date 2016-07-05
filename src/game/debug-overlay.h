#ifndef _DEBUG_OVERLAY_H_
#define _DEBUG_OVERLAY_H_

#include <libgame.h>

#include "scene.h"
#include "camera.h"
#include "text-renderable.h"

class DebugOverlay
{
public:
	void Init()
	{
		camera = scene.Add<Camera>();
		camera->Ortho(0.0f, 1280, 0.0f, 720, -100.0f, 100.0f);
		fps_text = scene.Add<TextRenderable>();
		fps_text->Text("");
		fps_text->Align(GFXString::LEFT | GFXString::TOP);
		fps_text->Translate(10.0f, 720.0f, 0.0f);
	}
	void Render()
	{
		camera->Render();
	}
	void SetFPS(unsigned short fps)
	{
		fps_text->Text("fps: " + std::to_string(fps));
	}
private:
	Scene scene;
	Camera* camera;
	TextRenderable* fps_text;
};

#endif