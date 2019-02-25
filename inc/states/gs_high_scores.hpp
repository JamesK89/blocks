#pragma once
#ifndef _INCLUDE_GAMESTATE_HIGHSCORES_H_
#define _INCLUDE_GAMESTATE_HIGHSCORES_H_

#include <blocks.hpp>
#include <gamestate.hpp>
#include <shapes.hpp>

#define NUM_SCORES	10

#ifndef __EMSCRIPTEN__
#	define SCORES_URL	"https://www.jamesjohnkelly.com/blocks/scores/"
#else
#	define SCORES_URL	"//www.jamesjohnkelly.com/blocks/scores/"
#endif

#define HIGHSCORE_STATE_ERROR_FETCHING		0
#define HIGHSCORE_STATE_ERROR_SUBMITTING	1
#define HIGHSCORE_STATE_FETCHING_TO_VIEW	2
#define HIGHSCORE_STATE_FETCHING_TO_SUBMIT	3
#define HIGHSCORE_STATE_VIEWING				4
#define HIGHSCORE_STATE_SUBMIT				5
#define HIGHSCORE_STATE_SUBMITTING			6
#define HIGHSCORE_STATE_INPUT				7

#define SCORE_NAME_LENGTH					3

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

	void DrawError(void);
	void DrawScores(void);
	void DrawFetching(void);
	void DrawSubmit(void);
	void DrawSubmitting(void);
	
	void OnInputViewing(SDL_Event& evt, bool down);
	void OnInputSubmit(SDL_Event& evt, bool down);

	char submitName_[SCORE_NAME_LENGTH + 1];
	int submitNameCharIdx_;
	unsigned short submitScore_;

	char names_[NUM_SCORES][SCORE_NAME_LENGTH + 1];
	unsigned short scores_[NUM_SCORES];
	
	unsigned char state_;
	
	bool GetScores(void);
	bool SubmitScore(const char* name, unsigned short score);
	
	void ParseScores(void);
	
	string	recvBuffer_;
	
#ifndef __EMSCRIPTEN__
	CURL*		curlEasyHandle_;
	CURLM*		curlMultHandle_;
	curl_mime*	curlMimeData_;
	
	static size_t CurlWriteStat(char * data, size_t size, size_t nmemb, void* p);
	size_t CurlWriteInst(char * data, size_t size, size_t nmemb);
	
	void CurlCleanup(void);
#else
	char* emFormData_;
	
	static void EmscriptenSuccessStat(emscripten_fetch_t *fetch);
	void EmscriptenSuccessImpl(emscripten_fetch_t *fetch);
	
	static void EmscriptenErrorStat(emscripten_fetch_t *fetch);
	void EmscriptenErrorImpl(emscripten_fetch_t *fetch);
#endif
};

#endif