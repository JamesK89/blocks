#pragma once
#ifndef _INCLUDE_GAMESTATE_HIGHSCORES_H_
#define _INCLUDE_GAMESTATE_HIGHSCORES_H_

#include <blocks.hpp>
#include <gamestate.hpp>
#include <shapes.hpp>

#define NUM_SCORES	10

#define SCORES_URL	"http://jamesjohnkelly.com/blocks/scores/"

#define HIGHSCORE_STATE_FETCHING_TO_VIEW	0
#define HIGHSCORE_STATE_FETCHING_TO_SUBMIT	1
#define HIGHSCORE_STATE_VIEWING				2
#define HIGHSCORE_STATE_SUBMIT				3
#define HIGHSCORE_STATE_SUBMITTING			4
#define HIGHSCORE_STATE_INPUT				5

#define SCORE_NAME_LENGTH			3

class GameStateHighScores : public BaseGameState
{
public:

	GameStateHighScores(Application* app);
	virtual ~GameStateHighScores(void);
	
	virtual const char* GetStateName(void) const;
	
	virtual void OnInput(SDL_Event& evt, bool down);
	virtual void OnUpdate(real delta);
	virtual void OnDraw(void);
	
	virtual void OnSuspend(BaseGameState* newState);
	virtual void OnResume(BaseGameState* oldState);
	
	virtual void OnInitialize(void);
	
	virtual void ViewScores(void);
	virtual void SubmitScore(unsigned short score = 0);
	
protected:

	void Clear(void);

	void DrawScores(void);
	void DrawFetching(void);
	void DrawSubmitting(void);
	
	void OnInputViewing(SDL_Event& evt, bool down);
	void OnInputSubmitting(SDL_Event& evt, bool down);

	char submitName_[SCORE_NAME_LENGTH + 1];
	unsigned char submitNameCharIdx_;
	unsigned short submitScore_;

	char names_[NUM_SCORES][SCORE_NAME_LENGTH + 1];
	unsigned short scores_[NUM_SCORES];
	
	unsigned char state_;
	
	bool GetScores(void);
	void SubmitScore(char name[3], unsigned short score) const;
	
	void ParseScores(void);
	
	string	recvBuffer_;
	
#ifndef __EMSCRIPTEN__
	static size_t CurlWriteStat(char * data, size_t size, size_t nmemb, void* p);
	size_t CurlWriteInst(char * data, size_t size, size_t nmemb);
#else
	static void EmscriptenOnLoadStat(void* arg, void* buffer, int size);
	void EmscriptenOnLoadInst(void* buffer, int size);
#endif
};

#endif