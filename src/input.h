#ifndef _INPUT_H_
#define _INPUT_H_

#include <windows.h>

class InputEvent
{
public:
	enum EVENT
	{
		DOWN,
		UP,
		CLICK,
		MOUSEMOVE
	};

	enum KEY
	{
		KEY_LBUTTON = 0x01, KEY_RBUTTON = 0x02, KEY_CANCEL = 0x03, KEY_MBUTTON = 0x04
	};

	InputEvent(EVENT e, unsigned int key_code)
	{
		this->event = e;
		this->key_code = key_code;
	}

	EVENT Event() { return event; }
	unsigned int KeyCode() { return key_code; }

	EVENT event;
	unsigned int key_code;
};

typedef void(*input_event_handler)(InputEvent&);

bool InputInit(HWND hWnd, input_event_handler handler = 0);
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

    static void _upd(int x, int y) { InputMouse::x = x; InputMouse::y = y; }
    static void _updRel(int x, int y) { InputMouse::rel_x = x; InputMouse::rel_y = y; }

private:
    InputMouse() {}
    static int x, y;
    static int rel_x, rel_y;
};

class InputKB
{
public:
    static bool Key(unsigned char key);
};

#endif