#include "gstest.h"
#include "gsmainmenu.h"

int main()
{
    SetPriorityClass(GetModuleHandle(NULL), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    if(!GameState::Init())
        return 0;
    
    GameState::Push<GSMainMenu>();
    GameState::Push<GSTest>();
    
	while (GameState::Update());
    
    GameState::Cleanup();
    return 0;
}