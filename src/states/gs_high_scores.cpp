#include <blocks.hpp>
#include <states/gs_high_scores.hpp>
#include <states/gs_main_menu.hpp>

#include <json-c/json.h>

#ifndef __EMSCRIPTEN__
#	include <curl/curl.h>
#endif

GameStateHighScores::GameStateHighScores(Application* app) : 
	BaseGameState(app),
	recvBuffer_(),
	state_(HIGHSCORE_STATE_FETCHING_TO_VIEW),
	submitScore_(0),
	submitNameCharIdx_(0)
{	
	Clear();
}

GameStateHighScores::~GameStateHighScores(void)
{
}

const char* GameStateHighScores::GetStateName(void) const
{
	return "GameState.HighScores";
}

void GameStateHighScores::OnInput(SDL_Event& evt, bool down)
{
	if (state_ == HIGHSCORE_STATE_VIEWING)
	{
		OnInputViewing(evt, down);
	}
	else if (state_ == HIGHSCORE_STATE_SUBMITTING)
	{
		OnInputSubmitting(evt, down);
	}
}

void GameStateHighScores::OnUpdate(real delta)
{
}

void GameStateHighScores::OnDraw(void)
{
	SDL_RenderSetClipRect(app_->GetRenderer(), nullptr);
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(app_->GetRenderer());

	app_->DrawBox(0, 0, (FRAME_WIDTH / TILE_SIZE), (FRAME_HEIGHT / TILE_SIZE));
	app_->DrawString(4, 1, "High Scores");
	
	if (state_ == HIGHSCORE_STATE_VIEWING)
	{
		DrawScores();
	}
	else if (state_ == HIGHSCORE_STATE_FETCHING_TO_VIEW || state_ == HIGHSCORE_STATE_FETCHING_TO_SUBMIT)
	{
		DrawFetching();
	}
	else if (state_ == HIGHSCORE_STATE_SUBMITTING)
	{
		DrawSubmitting();
	}
}

void GameStateHighScores::OnSuspend(BaseGameState* newState)
{
}

void GameStateHighScores::OnResume(BaseGameState* oldState)
{
}

void GameStateHighScores::OnInitialize(void)
{
}

void GameStateHighScores::Clear(void)
{
	memset(scores_, ZERO, sizeof(scores_));
	memset(names_, ZERO, sizeof(names_));
	memset(submitName_, ZERO, sizeof(submitName_));
	
	for (int i = 0; i < SCORE_NAME_LENGTH; i++) {
		submitName_[i] = 'A';
	}
	
	submitScore_ = 0;
	submitNameCharIdx_ = 0;
}

void GameStateHighScores::ViewScores(void)
{
	Clear();
	
	state_ = HIGHSCORE_STATE_FETCHING_TO_VIEW;
	
	GetScores();
}

void GameStateHighScores::SubmitScore(unsigned short score)
{
	Clear();
	
	state_ = HIGHSCORE_STATE_FETCHING_TO_SUBMIT;
	submitScore_ = score;
	
	GetScores();
}

void GameStateHighScores::DrawScores(void)
{
	int y = 3;
	
	for (int i = 0; i < NUM_SCORES; i++)
	{
		if (names_[i][0])
		{
			app_->DrawString(1, y, &names_[i][0]);
			
			char buffer[9];
			
			memset(buffer, ZERO, sizeof(buffer));
			snprintf(buffer, sizeof(buffer), "%i", scores_[i]);
			
			int len = strlen(buffer);
			
			app_->DrawString(((FRAME_WIDTH / TILE_SIZE) - 1) - len, y, &buffer[0]);
			
			y++;
		}
	}
}

void GameStateHighScores::DrawFetching(void)
{
	if ((app_->GetTickCount() % 50) > 25)
	{
		app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 8, 4, "FETCHING SCORES");
	}
}

void GameStateHighScores::DrawSubmitting(void)
{
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 7, 4, "ENTER INITIALS");
}

void GameStateHighScores::OnInputViewing(SDL_Event& evt, bool down)
{
	if (down)
	{
		app_->SetGameState("GameState.MainMenu");
	}
}
	
void GameStateHighScores::OnInputSubmitting(SDL_Event& evt, bool down)
{
	
}

#ifndef __EMSCRIPTEN__
size_t GameStateHighScores::CurlWriteStat(char * data, size_t size, size_t nmemb, void* p)
{
	return static_cast<GameStateHighScores*>(p)->CurlWriteInst(data, size, nmemb);
}

size_t GameStateHighScores::CurlWriteInst(char * data, size_t size, size_t nmemb)
{
	recvBuffer_.append(data, size * nmemb);
	return size * nmemb;
}
#else
void GameStateHighScores::EmscriptenOnLoadStat(void* arg, void* buffer, int size)
{
	return static_cast<GameStateHighScores*>(arg)->EmscriptenOnLoadInst(buffer, size);
}

void GameStateHighScores::EmscriptenOnLoadInst(void* buffer, int size)
{
	recvBuffer_.append(static_cast<char*>(buffer), size * sizeof(char));
	ParseScores();
}
#endif

bool GameStateHighScores::GetScores(void)
{
	bool result = false;

	Clear();
	
#ifndef __EMSCRIPTEN__
	recvBuffer_.clear();
	
	CURL* handle = curl_easy_init();
	
	if (handle)
	{
		struct curl_slist* headers = nullptr;

		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		
		curl_easy_setopt(handle, CURLOPT_URL, SCORES_URL);
		curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, dynamic_cast<void*>(this));
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &GameStateHighScores::CurlWriteStat);
		
		CURLcode code = curl_easy_perform(handle);
		curl_easy_cleanup(handle);
		
		result = (code == CURLE_OK);
	}
	
	if (result)
	{
		ParseScores();
	}
#else
	emscripten_async_wget_data(SCORES_URL, dynamic_cast<void*>(this), &GameStateHighScores::EmscriptenOnLoadStat, nullptr);
	result = true;
#endif

	return result;
}

void GameStateHighScores::SubmitScore(char name[3], unsigned short score) const
{
}

void GameStateHighScores::ParseScores(void)
{
	const char* szJson = recvBuffer_.c_str();
	
	json_tokener_error error;
	
	struct json_object* pJsObj = json_tokener_parse_verbose(szJson, &error);
	struct json_object* pScoresObj = nullptr;
	
	if (json_object_object_get_ex(pJsObj, "scores", &pScoresObj))
	{
		int length = MIN(NUM_SCORES, json_object_array_length(pScoresObj));
		int scoresIdx = 0;
		
		for (int i = 0; i < length; i++)
		{
			struct json_object* pScoreObj = json_object_array_get_idx(pScoresObj, i);
			
			if (!pScoreObj)
				continue;
				
			struct json_object* pScoreName = nullptr;
			struct json_object* pScoreValue = nullptr;

			if (json_object_object_get_ex(pScoreObj, "name", &pScoreName) && json_object_object_get_ex(pScoreObj, "score", &pScoreValue))
			{
				const char* str = json_object_get_string(pScoreName);
				
				snprintf(&names_[scoresIdx][0], SCORE_NAME_LENGTH + 1, "%s", str);
				scores_[scoresIdx] = json_object_get_int(pScoreValue);
			}
				
			scoresIdx++;
		}
	}
	
	recvBuffer_.clear();
	
	if (state_ == HIGHSCORE_STATE_FETCHING_TO_VIEW)
	{
		state_ = HIGHSCORE_STATE_VIEWING;
	}
	else if (state_ == HIGHSCORE_STATE_FETCHING_TO_SUBMIT)
	{
		state_ = HIGHSCORE_STATE_SUBMIT;
	}
}
