#include <blocks.hpp>
#include <states/gs_main_menu.hpp>
#include <states/gs_game_play.hpp>
#include <states/gs_high_scores.hpp>

GameStateMainMenu::GameStateMainMenu(Application* app)
	: BaseGameState(app), menuOption_(0)
{	
}

GameStateMainMenu::~GameStateMainMenu(void)
{
}

const char* GameStateMainMenu::GetStateName(void) const
{
	return "GameState.MainMenu";
}

void GameStateMainMenu::OnInput(SDL_Event& evt, bool down)
{
	if (down)
	{
		switch (evt.key.keysym.sym)
		{
			case SDLK_DOWN:
				menuOption_++;
				menuOption_ %= 
#ifdef __EMSCRIPTEN__
				2
#else
				3
#endif
				;
			break;
			case SDLK_UP:
				menuOption_--;
				menuOption_ %= 
#ifdef __EMSCRIPTEN__
				2
#else
				3
#endif
				;
			break;
			case SDLK_RETURN:
			case SDLK_RIGHT:
				OnMenuOptionSelect(menuOption_);
			break;
		}
	}
}

void GameStateMainMenu::OnUpdate(real delta)
{
}

void GameStateMainMenu::OnDraw(void)
{
	SDL_RenderSetClipRect(app_->GetRenderer(), nullptr);
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(app_->GetRenderer());

	app_->DrawBox(0, 0, (FRAME_WIDTH / TILE_SIZE), (FRAME_HEIGHT / TILE_SIZE));
	app_->DrawString(6, 1, "Blocks!");
	
	app_->DrawString(4, 4, "Start");
	app_->DrawString(4, 5, "High Scores");
	
#ifndef __EMSCRIPTEN__
	app_->DrawString(4, 6, "Exit");
#endif
	
	if ((app_->GetTickCount() % 24) > 12)
	{
		app_->DrawTile(3, 4 + menuOption_, TILE_CURSOR_LEFT);
	}
}

void GameStateMainMenu::OnSuspend(BaseGameState* newState)
{
}

void GameStateMainMenu::OnResume(BaseGameState* oldState)
{
	menuOption_ = 0;
}

void GameStateMainMenu::OnInitialize(void)
{
}

void GameStateMainMenu::OnMenuOptionSelect(int option)
{
	switch (option)
	{
		case 0:
			dynamic_cast<GameStateGamePlay*>(app_->SetGameState("GameState.GamePlay"))->NewGame();
		break;
		case 1:
			dynamic_cast<GameStateHighScores*>(app_->SetGameState("GameState.HighScores"))->ViewScores();
		break;
#ifndef __EMSCRIPTEN__
		case 2:
			app_->Stop();
		break;
#endif
	}
}
