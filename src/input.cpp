#include "input.h"
#include <iostream>
int InputMouse::x, InputMouse::y;
int InputMouse::rel_x, InputMouse::rel_y;

#define RID_COUNT 2
#define RID_MOUSE 0x02
#define RID_KEYBOARD 0x06

HWND targetWindow;
WNDPROC OldWndProc;
RAWINPUTDEVICE rid[RID_COUNT];

input_event_handler eventHandler = 0;

enum KEY_STATE
{
    UP,
    DOWN,
    CLICKED
};

KEY_STATE kbKeyState[256];

bool showCursor = true;

LRESULT CALLBACK InputWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_MOUSEMOVE:
        InputMouse::_upd(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_INPUT:
        {
            size_t sz = 40;
            static unsigned char lpb[40];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &sz, sizeof(RAWINPUTHEADER));

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE) // RIM_TYPEMOUSE is zero
            {
                int xPosRel = raw->data.mouse.lLastX;
                int yPosRel = raw->data.mouse.lLastY;

                InputMouse::_updRel(xPosRel, yPosRel);
				if (eventHandler)
					eventHandler(InputEvent(InputEvent::MOUSEMOVE, 0));
            }
            else if (raw->header.dwType & RIM_TYPEKEYBOARD)
            {
                if (raw->data.keyboard.Flags == RI_KEY_MAKE)
                {
                    kbKeyState[raw->data.keyboard.VKey] = KEY_STATE::DOWN;
					if (eventHandler)
						eventHandler(InputEvent(InputEvent::DOWN, raw->data.keyboard.VKey));
                }
                else if (raw->data.keyboard.Flags & RI_KEY_BREAK)
                {
                    kbKeyState[raw->data.keyboard.VKey] = KEY_STATE::UP;
					if (eventHandler)
						eventHandler(InputEvent(InputEvent::UP, raw->data.keyboard.VKey));
                }
                /*
                if(raw->data.keyboard.Message == WM_KEYDOWN)
                    kbKeyState[raw->data.keyboard.VKey] = KEY_STATE::DOWN;
                else if (raw->data.keyboard.Message == WM_KEYUP)
                    kbKeyState[raw->data.keyboard.VKey] = KEY_STATE::UP;*/
            }
        }
        break;
    default:
        return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
    }
    //return DefWindowProc(hWnd, msg, wParam, lParam);
    //return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
    return 0;
}

bool InputInit(HWND hWnd, input_event_handler handler)
{
    // Replacing the window procedure with our own one
    OldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)InputWndProc);
    if(!OldWndProc)
        return false;
    
    // Initizlize raw input
    
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = RID_MOUSE;
    rid[0].dwFlags = 0;//RIDEV_NOLEGACY;
    rid[0].hwndTarget = hWnd;
    
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = RID_KEYBOARD;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = hWnd;
    
    if(!RegisterRawInputDevices(rid, RID_COUNT, sizeof(RAWINPUTDEVICE)))
        return false;

    targetWindow = hWnd;

	eventHandler = handler;
   
    return true;
}

void InputCleanup()
{
    
}

void InputUpdate()
{
    if(!showCursor)
    {
        if(GetActiveWindow() == targetWindow)
        {
            RECT rect;
            GetWindowRect(targetWindow, &rect);
            SetCursorPos(rect.left + (rect.right - rect.left) * 0.5, rect.top + (rect.bottom - rect.top) * 0.5);
        }
    }
}

void InputShowCursor(bool show)
{
    showCursor = show;
    ShowCursor(showCursor);
}

float InputMouse::GetXNorm()
{
    RECT rect;
    GetClientRect(targetWindow, &rect);
    return (x / (float)(rect.right - rect.left));
}
float InputMouse::GetYNorm()
{
    RECT rect;
    GetClientRect(targetWindow, &rect);
    return (y / (float)(rect.bottom - rect.top));
}

bool InputKB::Key(unsigned char key)
{
    if (kbKeyState[key] == KEY_STATE::DOWN)
        return true;
    else
        return false;
}