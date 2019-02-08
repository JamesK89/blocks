#pragma once
#ifndef _INCLUDE_GAMESTATE_MAINMENU_H_
#define _INCLUDE_GAMESTATE_MAINMENU_H_

#include <blocks.hpp>
#include <gamestate.hpp>
#include <shapes.hpp>

class GameStateMainMenu : public BaseGameState
{
public:

	GameStateMainMenu(Application* app);
	virtual ~GameStateMainMenu(void);
	
	virtual const char* GetStateName(void) const;
	
	virtual void OnInput(SDL_Event& evt, bool down);
	virtual void OnUpdate(real delta);
	virtual void OnDraw(void);
	
	virtual void OnSuspend(BaseGameState* newState);
	virtual void OnResume(BaseGameState* oldState);
	
	virtual void OnInitialize(void);
	
protected:
	unsigned int menuOption_;
	
	void OnMenuOptionSelect(int option);
};

#endif