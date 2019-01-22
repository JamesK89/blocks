#pragma once
#ifndef _INCLUDE_GAMESTATE_MAIN_H_
#define _INCLUDE_GAMESTATE_MAIN_H_

#include <blocks.hpp>
#include <gamestate.hpp>

class GameStateMain : public BaseGameState
{
public:

	GameStateMain(Application* app);
	virtual ~GameStateMain(void);
	
	virtual const char* GetStateName(void) const;
	
	virtual void OnInput(SDL_Event& evt, bool down);
	virtual void OnUpdate(real delta);
	virtual void OnDraw(void);
	
	virtual void OnSuspend(BaseGameState* newState);
	virtual void OnResume(BaseGameState* oldState);
	
	virtual void OnInitialize(void);
	
protected:
	unsigned int menuOption_;
};

#endif