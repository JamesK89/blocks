#pragma once
#ifndef _INCLUDE_GAMESTATE_GAMEPLAY_H_
#define _INCLUDE_GAMESTATE_GAMEPLAY_H_

#include <blocks.hpp>
#include <gamestate.hpp>
#include <shapes.hpp>

#define GAMEPLAY_STATE_PLAYING	0
#define GAMEPLAY_STATE_PAUSED	1

#define INPUT_ACTION_NONE			0
#define INPUT_ACTION_ROTATE			(1 << 0)
#define INPUT_ACTION_SHIFT_LEFT		(1 << 1)
#define INPUT_ACTION_SHIFT_RIGHT	(1 << 2)
#define INPUT_ACTION_DROP			(1 << 3)

class GameStateGamePlay : public BaseGameState
{
public:

	GameStateGamePlay(Application* app);
	virtual ~GameStateGamePlay(void);
	
	virtual const char* GetStateName(void) const;
	
	virtual void OnInput(SDL_Event& evt, bool down);
	virtual void OnUpdate(real delta);
	virtual void OnDraw(void);
	
	virtual void OnSuspend(BaseGameState* newState);
	virtual void OnResume(BaseGameState* oldState);
	
	virtual void OnInitialize(void);
	
protected:
	unsigned short level_;
	unsigned short score_;
	
	unsigned char gamestate_;
	
	unsigned char pauseMenuOption_;
	
	unsigned int playfieldWidth_;
	unsigned int playfieldHeight_;
	
	unsigned char* playfield_;
	
	unsigned char inputAction_;
	
	Shape* currentShape_;
	Shape* nextShape_;
	
	Shape** shapes_;
	unsigned int numShapes_;
	bool* shapePlayed_;
	
	real nextTick_;
	
	int numCompleteLines_;
	bool scoring_;
	
	void ClampShape(Shape* shape);
	
	bool DoesShapeCollide(const Shape* shape) const;
	void ImpressShapeOntoPlayfield(const Shape* shape);
	
	bool MoveShape(Shape* shape, int dx, int dy);
	bool RotateShape(Shape* shape);
	
	bool CheckForCompleteLines(void);
	void ClearCompleteLines(void);
	bool IsLineComplete(int y) const;
	
	void OnTick(void);
	
	void SpawnShape(void);
	void DrawShape(const Shape* shape);
	
	void DrawLevel(void);
	void DrawScore(void);
	void DrawPlayfield(void);
	void DrawPause(void);
};

#endif