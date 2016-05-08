#ifndef _INPUT_H_
#define _INPUT_H_

#include <windows.h>

bool InputInit(HWND hWnd);
void InputCleanup();
void InputUpdate();

void InputShowCursor(bool show);

#endif