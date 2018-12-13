#include <blocks.hpp>
#include <states/main.hpp>

GameStateMain::GameStateMain(Application* app)
	: BaseGameState(app)
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
	app_->DrawString(1, 1, "Blocks\t12989\nTest Application");
	app_->DrawTile(1, 1, TILE_BLOCK_BLU);
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
