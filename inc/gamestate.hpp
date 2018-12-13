#pragma once
#ifndef _INCLUDE_GAMESTATE_H_
#define _INCLUDE_GAMESTATE_H_

#include <SDL.h>

#ifndef Application
	class Application;
#endif

class BaseGameState
{
protected:

	Application* app_;
	
public:

	BaseGameState(Application* app) : app_(app) {};
	virtual ~BaseGameState(void) { };
	
	Application* GetApplication(void) const { return app_; };
	
	virtual const char* GetStateName(void) const = 0;
	
	virtual void OnInput(SDL_Event& evt, bool down) = 0;
	virtual void OnUpdate(real delta) = 0;
	virtual void OnDraw(void) = 0;
	
	virtual void OnSuspend(BaseGameState* newState) = 0;
	virtual void OnResume(BaseGameState* oldState) = 0;
	
	virtual void OnInitialize(void) = 0;
};

#endif