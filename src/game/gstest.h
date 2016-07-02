#ifndef _GSLOADING_H_
#define _GSLOADING_H_

#include <Windows.h>

#include "GameState.h"

#include "gui.h"
#include "scene.h"
#include "renderable.h"
#include "text-renderable.h"
#include "sprite.h"
#include "camera.h"
#include "lightomni.h"
#include "bounding-shape.h"

#include <string>
#include <iostream>

class GSTest : public GameState
{
public:
    GSTest() : start_time(0), time(0) {}
    void OnInit();
    void OnSwitch();
    void OnUpdate();
    void OnRender();
	void OnInput(InputEvent& e);

    void OnCleanup();
	
private:
    DWORD start_time;
    DWORD time;
    
    Scene scene;
	Scene scene2d;

    Camera* camera;
    Camera* camera2d;
    Renderable* cube;
    Renderable* cube2;

    GFXLine line;

	Sprite* sprite;
};

#endif