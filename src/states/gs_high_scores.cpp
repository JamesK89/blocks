#include <blocks.hpp>
#include <states/gs_high_scores.hpp>
#include <states/gs_main_menu.hpp>

#include <json-c/json.h>

GameStateHighScores::GameStateHighScores(Application* app) : 
	BaseGameState(app),
	recvBuffer_(),
	state_(HIGHSCORE_STATE_FETCHING_TO_VIEW),
	submitScore_(0),
	submitNameCharIdx_(0)
#ifndef __EMSCRIPTEN__
	,	curlMultHandle_(nullptr),
		curlEasyHandle_(nullptr)
#endif
	
{	
	Clear();
}

GameStateHighScores::~GameStateHighScores(void)
{
#ifndef __EMSCRIPTEN__
	CurlCleanup();
#endif
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
	else if (state_ == HIGHSCORE_STATE_SUBMIT)
	{
		OnInputSubmit(evt, down);
	}
}

void GameStateHighScores::OnUpdate(real delta)
{
#ifndef __EMSCRIPTEN__
	if (curlMultHandle_)
	{
		int runningHandles = 0;
		if (curl_multi_perform(curlMultHandle_, &runningHandles) == CURLM_OK)
		{
			if (runningHandles < 1)
			{
				if (state_ == HIGHSCORE_STATE_FETCHING_TO_VIEW || state_ == HIGHSCORE_STATE_FETCHING_TO_SUBMIT)
				{
					ParseScores();
				}
				
				CurlCleanup();
			}
		}
	}
#endif
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
	else if (state_ == HIGHSCORE_STATE_SUBMIT)
	{
		DrawSubmit();
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
	int y = 5;
	
	for (int i = 0; i < NUM_SCORES; i++)
	{
		if (names_[i][0])
		{
			app_->DrawString(1, y, &names_[i][0]);
			
			char buffer[9];
			
			memset(buffer, ZERO, sizeof(buffer));
			snprintf(buffer, sizeof(buffer), "%i", scores_[i]);
			
			int len = int(strlen(buffer));
			
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
	if ((app_->GetTickCount() % 50) > 25)
	{
		app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 5, 4, "SUBMITTING");
	}
}

void GameStateHighScores::DrawSubmit(void)
{
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 7, 4, "ENTER INITIALS");
	
	int startX = ((FRAME_WIDTH / TILE_SIZE) >> 1) - 2;
	
	app_->DrawString(startX, 6, submitName_);
	
	if ((app_->GetTickCount() % 24) > 12)
		app_->DrawTile(startX + submitNameCharIdx_, 7, TILE_CURSOR);
		
	char buffer[9];
	memset(buffer, ZERO, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "%i", submitScore_);
	
	int len = strlen(buffer);
	
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 7, 9, "FOR FINAL SCORE");
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - (len >> 1) - 1, 11, &buffer[0]);
}

void GameStateHighScores::OnInputViewing(SDL_Event& evt, bool down)
{
	if (down)
	{
		app_->SetGameState("GameState.MainMenu");
	}
}
	
void GameStateHighScores::OnInputSubmit(SDL_Event& evt, bool down)
{
	if (down)
	{
		switch (evt.key.keysym.sym)
		{
			case SDLK_RIGHT:
				submitNameCharIdx_++;
			break;
			case SDLK_LEFT:
				submitNameCharIdx_--;
			break;
			case SDLK_UP:
				submitName_[submitNameCharIdx_]++;
			break;
			case SDLK_DOWN:
				submitName_[submitNameCharIdx_]--;
			break;
			case SDLK_RETURN:
				SubmitScore(submitName_, submitScore_);
			break;
		}
		
		if (submitNameCharIdx_ < 0)
			submitNameCharIdx_ = SCORE_NAME_LENGTH - 1;
		else if (submitNameCharIdx_ > SCORE_NAME_LENGTH - 1)
			submitNameCharIdx_ = 0;
			
		if (submitName_[submitNameCharIdx_] < 'A')
			submitName_[submitNameCharIdx_] = 'Z';
		else if (submitName_[submitNameCharIdx_] > 'Z')
			submitName_[submitNameCharIdx_] = 'A';
	}
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

void GameStateHighScores::CurlCleanup(void)
{
	if (curlEasyHandle_)
	{
		if (curlMultHandle_)
		{
			curl_multi_remove_handle(curlMultHandle_, curlEasyHandle_);
		}
		
		curl_easy_cleanup(curlEasyHandle_);
		curlEasyHandle_ = nullptr;
	}
	
	if (curlMultHandle_)
	{
		curl_multi_cleanup(curlMultHandle_);
		curlMultHandle_ = nullptr;
	}
}
#else
void GameStateHighScores::EmscriptenSuccessStat(emscripten_fetch_t* fetch)
{
	if (fetch && fetch->userData)
	{
		static_cast<GameStateHighScores*>(fetch->userData)->EmscriptenSuccessImpl(fetch);
	}
}

void GameStateHighScores::EmscriptenSuccessImpl(emscripten_fetch_t* fetch)
{
	if (fetch)
	{
		if (fetch->data && fetch->numBytes)
		{
			recvBuffer_.append(fetch->data, fetch->numBytes / sizeof(char));
		}
		
		emscripten_fetch_close(fetch);
	}
	
	ParseScores();
}

void GameStateHighScores::EmscriptenErrorStat(emscripten_fetch_t* fetch)
{
	if (fetch && fetch->userData)
	{
		static_cast<GameStateHighScores*>(fetch->userData)->EmscriptenErrorImpl(fetch);
	}
}

void GameStateHighScores::EmscriptenErrorImpl(emscripten_fetch_t* fetch)
{
	if (fetch)
	{
		emscripten_fetch_close(fetch);
	}
}
#endif

bool GameStateHighScores::GetScores(void)
{
	bool result = false;

#ifndef __EMSCRIPTEN__
	CurlCleanup();
	
	recvBuffer_.clear();
	curlMultHandle_ = curl_multi_init();
	
	if (curlMultHandle_)
	{
		curlEasyHandle_ = curl_easy_init();
		
		if (curlEasyHandle_)
		{
			curl_easy_setopt(curlEasyHandle_, CURLOPT_URL, SCORES_URL);
			curl_easy_setopt(curlEasyHandle_, CURLOPT_HTTPGET, 1L);
			curl_easy_setopt(curlEasyHandle_, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curlEasyHandle_, CURLOPT_WRITEDATA, dynamic_cast<void*>(this));
			curl_easy_setopt(curlEasyHandle_, CURLOPT_WRITEFUNCTION, &GameStateHighScores::CurlWriteStat);
			
			if (curl_multi_add_handle(curlMultHandle_, curlEasyHandle_) == CURLM_OK)
			{
				result = true;
			}
		}
	}
#else
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.overriddenMimeType = "application/json";
	attr.userData = dynamic_cast<void*>(this);
	attr.onsuccess = &GameStateHighScores::EmscriptenSuccessStat;
	attr.onerror = &GameStateHighScores::EmscriptenErrorStat;
  
	emscripten_fetch(&attr, SCORES_URL);
  
	result = true;
#endif

	return result;
}

bool GameStateHighScores::SubmitScore(const char* name, unsigned short score)
{
	bool result = false;

	if (name && score)
	{
#ifndef __EMSCRIPTEN__
		CurlCleanup();
		
		recvBuffer_.clear();
		curlMultHandle_ = curl_multi_init();
		
		if (curlMultHandle_)
		{
			curlEasyHandle_ = curl_easy_init();
			
			if (curlEasyHandle_)
			{
				char buffer[1024];
				
				snprintf(buffer, sizeof(buffer) / sizeof(char), "name=%s&score=%i", name, score);
				
				struct curl_slist* headers = nullptr;

				headers = curl_slist_append(headers, "charsets: utf-8");
				
				curl_easy_setopt(curlEasyHandle_, CURLOPT_URL, SCORES_URL);
				curl_easy_setopt(curlEasyHandle_, CURLOPT_POSTFIELDS, buffer);
				curl_easy_setopt(curlEasyHandle_, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curlEasyHandle_, CURLOPT_WRITEDATA, dynamic_cast<void*>(this));
				curl_easy_setopt(curlEasyHandle_, CURLOPT_WRITEFUNCTION, &GameStateHighScores::CurlWriteStat);
			
				if (curl_multi_add_handle(curlMultHandle_, curlEasyHandle_) == CURLM_OK)
				{
					result = true;
				}
			}
		}
#else
		result = true;
#endif
	}

	return result;
}

void GameStateHighScores::ParseScores(void)
{
	const char* szJson = recvBuffer_.c_str();
	
	json_tokener_error error;
	
	struct json_object* pJsObj = json_tokener_parse_verbose(szJson, &error);
	struct json_object* pScoresObj = nullptr;
	
	if (json_object_object_get_ex(pJsObj, "scores", &pScoresObj))
	{
		int length = int(MIN(NUM_SCORES, json_object_array_length(pScoresObj)));
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
	
	if (state_ == HIGHSCORE_STATE_FETCHING_TO_VIEW || state_ == HIGHSCORE_STATE_SUBMIT)
	{
		state_ = HIGHSCORE_STATE_VIEWING;
	}
	else if (state_ == HIGHSCORE_STATE_FETCHING_TO_SUBMIT)
	{
		state_ = HIGHSCORE_STATE_SUBMIT;
	}
}
