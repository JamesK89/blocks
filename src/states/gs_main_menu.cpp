#include <blocks.hpp>
#include <states/gs_main_menu.hpp>

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
				menuOption_ %= 2;
			break;
			case SDLK_UP:
				menuOption_--;
				menuOption_ %= 2;
			break;
			case SDLK_RETURN:
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
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(app_->GetRenderer());

	app_->DrawBox(0, 0, (WINDOW_WIDTH / TILE_SIZE), (WINDOW_HEIGHT / TILE_SIZE));
	app_->DrawString(7, 1, "Blocks");
	
	app_->DrawString(7, 4, "Start");
	app_->DrawString(7, 5, "Exit");
	
	if ((app_->GetTickCount() % 24) > 12)
	{
		app_->DrawTile(6, 4 + menuOption_, TILE_CURSOR_LEFT);
	}
}

void GameStateMainMenu::OnSuspend(BaseGameState* newState)
{
}

void GameStateMainMenu::OnResume(BaseGameState* oldState)
{
}

void GameStateMainMenu::OnInitialize(void)
{
}

void GameStateMainMenu::OnMenuOptionSelect(int option)
{
	switch (option)
	{
		case 0:
			app_->SetGameState("GameState.GamePlay");
		break;
		case 1:
			app_->Stop();
		break;
	}
}
