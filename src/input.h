#ifndef _INPUT_H_
#define _INPUT_H_

#include <windows.h>

#include "action.h"

bool InputInit(HWND hWnd);
void InputCleanup();
void InputUpdate();

void InputShowCursor(bool show);

class InputMouse
{
public:
    static int GetX() { return x; }
    static int GetY() { return y; }
    static float GetXNorm();
    static float GetYNorm();
    static int GetXRel() { return rel_x; }
    static int GetYRel() { return rel_y; }

    static void SetMoveCallback(Action& act);

    static void _upd(int x, int y) { InputMouse::x = x; InputMouse::y = y; }
    static void _updRel(int x, int y) { InputMouse::rel_x = x; InputMouse::rel_y = y; }

    static void _triggerMoveAction() { move_action->Execute(); }
private:
    InputMouse() {}
    static int x, y;
    static int rel_x, rel_y;
    static Action* move_action;
};

class InputKB
{
public:
    static bool Key(unsigned char key);
};

#endif