#include "window.h"

#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

Window::Window() : hWnd(0), msg({0})
{

}
Window::~Window(){}
    
Window Window::Create(LPCWSTR name, int w, int h)
{
    Window window;
    
    HINSTANCE hInstance = GetModuleHandle(0);
    
    WNDCLASSEX wc;
    HWND hWnd;
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = name;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if(!RegisterClassEx(&wc))
    {
        return window;
    }
    
    hWnd = CreateWindowEx(
        0,
        name,
        name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, w, h,
        NULL, NULL, hInstance, NULL);
        
    if(hWnd == NULL)
    {
        return window;
    }
    
    window.hWnd = hWnd;
    window.msg = { 0 };
    
    ShowWindow(hWnd, 1);
    UpdateWindow(hWnd);
    
    return window;
}

bool Window::Valid()
{
    return msg.message != WM_QUIT;
}

HWND Window::GetHandle()
{
    return hWnd;
}

void Window::Poll()
{
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Window::Update()
{
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        if(msg.message == WM_QUIT)
            return false;
    }
    return true;
}
