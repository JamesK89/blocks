#pragma once
#ifndef _INCLUDE_BLOCKS_H_
#define _INCLUDE_BLOCKS_H_

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL.h>

#define MIN(x,y)	(((x) < (y)) ? (x) : (y))
#define MAX(x,y)	(((x) > (y)) ? (x) : (y))

using namespace std;

#define FRAMES_PER_SECOND	(50.0)
#define FRAMES_ONE_STEP		(1.0 / FRAMES_PER_SECOND)

#define MAX_FRAME_SKIP		5
#define MAX_FRAME_TIME		(1.0f / 4)

#define TILE_SIZE		8

#define WINDOW_WIDTH	160
#define WINDOW_HEIGHT	144
#define WINDOW_TITLE	"Blocks"

#define TILE_TEXT			3
#define TILE_NUMBER			32

#define TILE_BOX_TOP_LEFT	0
#define TILE_BOX_TOP		1
#define TILE_BOX_TOP_RIGHT	2
#define TILE_BOX_LEFT		29
#define TILE_BOX_RIGHT		31
#define TILE_BOX_BOT_LEFT	58
#define TILE_BOX_BOT		59
#define TILE_BOX_BOT_RIGHT	60

#define TILE_SHDW_BOX_TOP_LEFT	87
#define TILE_SHDW_BOX_TOP		88
#define TILE_SHDW_BOX_TOP_RIGHT	89
#define TILE_SHDW_BOX_LEFT		116
#define TILE_SHDW_BOX_RIGHT		118
#define TILE_SHDW_BOX_BOT_LEFT	145
#define TILE_SHDW_BOX_BOT		146
#define TILE_SHDW_BOX_BOT_RIGHT	147

#define TILE_BLOCK_RED	61
#define TILE_BLOCK_BLU	62
#define TILE_BLOCK_YEL	63
#define TILE_BLOCK_GRN	64
#define TILE_BLOCK_GRY	65
#define TILE_BLOCK_PUR	66
#define TILE_BLOCK_ORG	67
#define TILE_BLOCK_CYN	68

#define TILE_CURSOR			90
#define TILE_CURSOR_LEFT	91
#define TILE_CURSOR_RIGHT	92

typedef	unsigned long long ticks_t;
typedef double gametime_t;

typedef float real;

#ifndef BaseGameState
	class BaseGameState;
#endif

class Application
{
public:

	static Application* GetInstance(void);

	int Run(bool fullscreen);
	void Stop(void);

	bool IsRunning(void) const;

	~Application(void);

	void DrawTile(int x, int y, int tile);
	void DrawString(int x, int y, const char* str);
	void DrawBox(int x, int y, int w, int h);
	void DrawShadowBox(int x, int y, int w, int h);

	ticks_t GetTickCount(void) const;
	gametime_t GetGameTime(void) const;

	SDL_Renderer* GetRenderer(void);
	SDL_Window* GetWindow(void);
	
	BaseGameState** GetGameStates(void);
	BaseGameState* GetGameState(const char* szStateName);
	BaseGameState* GetGameState(void);
	
	BaseGameState* SetGameState(const char* szStateName);
	BaseGameState* SetGameState(BaseGameState* state);

private:

	void MainLoop(void* arg);

	Application(void);

	bool InitializeSDL(bool fullscreen);
	void InitializeResources(void);
	
	void InitializeStates(void);

	static Application* instance_;

	void HandleInput(SDL_Event& event, bool down);
	void HandleUpdate(real delta);
	void HandleDraw(real smooth);

	SDL_Window* window_;
	SDL_Renderer* renderer_;

	SDL_Texture* target_;

	SDL_Texture* tileSheet_;
	SDL_Rect* tileRects_;

	bool isRunning_;
	
	real smooth_;
	ticks_t ticks_;
	gametime_t time_;
	
	BaseGameState** gameStates_;
	BaseGameState* currentGameState_;
};

#endif