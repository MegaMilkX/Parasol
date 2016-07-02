#ifndef _GSMAINMENU_H_
#define _GSMAINMENU_H_

#include "GameState.h"

class GSMainMenu : public GameState
{
public:
    void OnInit()
    {
        
    }
    void OnSwitch()
    {
        glClearColor(0.8f, 0.4f, 0.2f, 1.0f);
    }
    void OnUpdate()
    {
        //std::cout << "MAIN_MENU" << std::endl;
    }
    void OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void OnCleanup()
    {

    }
};

#endif