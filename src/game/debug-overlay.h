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
		QueryPerformanceFrequency(&freq);
	}
	void Render()
	{
		fps_text->Text("fps: " + std::to_string(fps));
		camera->Render();
	}
	void PerfCountBegin() 
	{
		QueryPerformanceCounter(&t1);
	}
	void PerfCountEnd() 
	{
		QueryPerformanceCounter(&t2);
		unsigned short _fps = (unsigned short)(1 / ((t2.QuadPart - t1.QuadPart) / (float)freq.QuadPart));
		fps = (fps + _fps) / 2;
	}
private:
	Scene scene;
	Camera* camera;
	TextRenderable* fps_text;

	LARGE_INTEGER freq;
	LARGE_INTEGER t1, t2;
	unsigned short fps;
};

#endif