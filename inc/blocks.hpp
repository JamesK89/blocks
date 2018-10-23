#pragma once
#ifndef _INCLUDE_BLOCKS_H_
#define _INCLUDE_BLOCKS_H_

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <SDL.h>

#define FRAMES_PER_SECOND	(50.0)
#define FRAMES_ONE_STEP		(1.0 / FRAMES_PER_SECOND)

#define MAX_FRAME_SKIP		5
#define MAX_FRAME_TIME		(1.0f / 4)

#define TILE_SIZE		8

#define WINDOW_WIDTH	320
#define WINDOW_HEIGHT	200

#define TILE_BOX_TOP_LEFT	0
#define TILE_BOX_TOP		1
#define TILE_BOX_TOP_RIGHT	2
#define TILE_BOX_LEFT		29
#define TILE_BOX_RIGHT		31
#define TILE_BOX_BOT_LEFT	58
#define TILE_BOX_BOT		59
#define TILE_BOX_BOT_RIGHT	60

#define TILE_SHDW_BOX_TOP_LEFT	0
#define TILE_SHDW_BOX_TOP		1
#define TILE_SHDW_BOX_TOP_RIGHT	2
#define TILE_SHDW_BOX_LEFT		29
#define TILE_SHDW_BOX_RIGHT		31
#define TILE_SHDW_BOX_BOT_LEFT	58
#define TILE_SHDW_BOX_BOT		59
#define TILE_SHDW_BOX_BOT_RIGHT	60

#define TILE_BLOCK_RED	61
#define TILE_BLOCK_BLU	62
#define TILE_BLOCK_YEL	63
#define TILE_BLOCK_GRN	64
#define TILE_BLOCK_GRY	65

typedef	unsigned long long ticks_t;
typedef double gametime_t;

class BlocksApplication
{
public:

	static BlocksApplication* GetInstance(void);

	int Run(bool fullscreen);
	void Stop(void);

	bool IsRunning(void) const;

	~BlocksApplication(void);

	ticks_t GetTickCount(void) const;
	gametime_t GetGameTime(void) const;

	SDL_Renderer* GetRenderer(void);
	SDL_Window* GetWindow(void);

private:

	void MainLoop(void* arg);

	BlocksApplication(void);

	bool InitializeSDL(bool fullscreen);
	void InitializeResources(void);

	static BlocksApplication* instance_;

	void HandleUpdate(float delta);
	void HandleInput(SDL_Event& event, bool down);
	void HandleRender(void);

	SDL_Window* window_;
	SDL_Renderer* renderer_;

	bool isRunning_;
	float smooth_;

	ticks_t ticks_;
	gametime_t time_;

	SDL_Texture* tileSheet_;
	SDL_Rect* tileRects_;
};

#endif