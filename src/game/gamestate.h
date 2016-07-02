#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <stack>
#include <map>
#include <iostream>

#include <libgame.h>

#include "debug-overlay.h"

class GameState
{
public:
    virtual ~GameState(){}

    virtual void OnInit() = 0;
    virtual void OnSwitch() = 0;
    virtual void OnCleanup() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;

	virtual void OnInput(InputEvent& e) {}
    
    static bool Init();
    static void Cleanup();

    template<typename T>
    static T* Push();
    static void Pop();

    static bool Update();

	static void PostInput(InputEvent& e);
protected:
    static std::stack<GameState*> state_stack;
	static std::map<int, GameState*> state_cache;
    static Window window;

	int type_index;

	static DebugOverlay debug_overlay;
};

template<typename T>
T* GameState::Push()
{
	std::map<int, GameState*>::iterator it = state_cache.find(TypeInfo<T>::GetId());
	GameState* state = 0;
	if (it == state_cache.end())
	{
		state = new T();
		state->type_index = TypeInfo<T>::GetId();
		state_cache.insert(std::make_pair(state->type_index, state));
		state->OnInit();
	}
	else
		state = it->second;

    state_stack.push(state);
    state->OnSwitch();

	return (T*)state;
}

#endif