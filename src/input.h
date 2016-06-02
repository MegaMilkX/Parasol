#ifndef _INPUT_H_
#define _INPUT_H_

#include <windows.h>

bool InputInit(HWND hWnd);
void InputCleanup();
void InputUpdate();

void InputShowCursor(bool show);

class InputMouse
{
public:
    static int GetX() { return x; }
    static int GetY() { return y; }

    static void _upd(int x, int y) { InputMouse::x = x; InputMouse::y = y; }
private:
    InputMouse() {}
    static int x, y;
};

#endif