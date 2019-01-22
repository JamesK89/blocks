#include <blocks.hpp>
#include <states/gs_main_menu.hpp>

GameStateMain::GameStateMain(Application* app)
	: BaseGameState(app), menuOption_(0)
{	
}

GameStateMain::~GameStateMain(void)
{
}

const char* GameStateMain::GetStateName(void) const
{
	return "GameState.Main";
}

void GameStateMain::OnInput(SDL_Event& evt, bool down)
{
	if (down)
	{
		switch (evt.key.keysym.sym)
		{
			case SDLK_DOWN:
				menuOption_++;
				menuOption_ %= 2;
			break;
			case SDLK_UP:
				menuOption_--;
				menuOption_ %= 2;
			break;
			case SDLK_RETURN:
				if (menuOption_ == 1)
				{
					app_->Stop();
				}
			break;
		}
	}
}

void GameStateMain::OnUpdate(real delta)
{
}

void GameStateMain::OnDraw(void)
{
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(app_->GetRenderer());

	app_->DrawBox(0, 0, (WINDOW_WIDTH / TILE_SIZE), (WINDOW_HEIGHT / TILE_SIZE));
	app_->DrawBox(1, 3, (WINDOW_WIDTH / TILE_SIZE) - 2, (WINDOW_HEIGHT / TILE_SIZE) - 4);
	app_->DrawString(1, 1, "Blocks");
	
	app_->DrawString(3, 4, "Start");
	app_->DrawString(3, 5, "Exit");
	
	if ((app_->GetTickCount() % 24) > 12)
	{
		app_->DrawTile(2, 4 + menuOption_, TILE_CURSOR_LEFT);
	}
}

void GameStateMain::OnSuspend(BaseGameState* newState)
{
}

void GameStateMain::OnResume(BaseGameState* oldState)
{
}

void GameStateMain::OnInitialize(void)
{
}
