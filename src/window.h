#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>

class Window
{
public:
    Window();
    ~Window();
    
    static Window Create(LPCWSTR name, int w = 640, int h = 480);
    bool Valid();
    HWND GetHandle();
    void Poll();
    bool Update();
private:
    
    HWND hWnd;
    MSG msg;
};

#endif