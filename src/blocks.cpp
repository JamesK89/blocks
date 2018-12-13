#include <blocks.hpp>
#include <gamestate.hpp>

#include <states/main.hpp>

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

Application* Application::instance_ = nullptr;

typedef struct s_main_loop_context_t
{
#ifdef __EMSCRIPTEN__
	Application* app;
	void (Application::*func)(void* arg);
#endif

	double newTime;
	double currentTime;
	double frameTime;
	double acc;
} MainLoopContext;

Application* Application::GetInstance(void)
{
	if (!instance_)
		instance_ = new Application();

	return instance_;
}

Application::Application(void) :
	window_(nullptr), renderer_(nullptr), tileSheet_(nullptr), 
	tileRects_(nullptr), target_(nullptr), isRunning_(false), 
	smooth_(0), ticks_(0), time_(0),
	gameStates_(nullptr), currentGameState_(nullptr)
{
}

Application::~Application(void)
{
	if (currentGameState_)
	{
		currentGameState_ = nullptr;
	}
	
	if (gameStates_)
	{
		BaseGameState** ptr = gameStates_;
		
		while (ptr && *ptr)
		{
			delete *ptr;
			*ptr = nullptr;
			
			ptr++;
		}
	
		delete [] gameStates_;
		gameStates_ = nullptr;
	}
	
	if (tileSheet_)
	{
		SDL_DestroyTexture(tileSheet_);
		tileSheet_ = nullptr;
	}

	if (tileRects_)
	{
		delete[] tileRects_;
		tileRects_ = nullptr;
	}

	if (renderer_)
	{
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}

	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	if (instance_ == this)
	{
		instance_ = nullptr;
		SDL_Quit();
	}
}

void Application::Stop(void)
{
	isRunning_ = false;
}

bool Application::IsRunning(void) const
{
	return isRunning_;
}

ticks_t Application::GetTickCount(void) const
{
	return ticks_;
}

gametime_t Application::GetGameTime(void) const
{
	return time_;
}

SDL_Renderer* Application::GetRenderer(void)
{
	return renderer_;
}

SDL_Window* Application::GetWindow(void)
{
	return window_;
}

bool Application::InitializeSDL(bool fullscreen)
{
	bool ret = true;

	if (!window_ && !renderer_)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			ret = false;
			fprintf(stderr, "Failed to initialize.\n");
		}
		else
		{
			window_ = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

			if (!window_)
			{
				ret = false;
				fprintf(stderr, "Failed to create window.\n");
			}
			else
			{
				renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

				if (!renderer_)
				{
					ret = false;
					fprintf(stderr, "Failed to create renderer.\n");
				}
			}
		}
	}

	return ret;
}

void Application::InitializeResources(void)
{
	if (!target_)
	{
		target_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	if (!tileSheet_)
	{
		SDL_Surface* surf = SDL_LoadBMP("./assets/sheet.bmp");

		if (surf)
		{
			SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0xFF, 0x00, 0xFF));
			tileSheet_ = SDL_CreateTextureFromSurface(renderer_, surf);

			int w, h;

			SDL_QueryTexture(tileSheet_, nullptr, nullptr, &w, &h);

			int numTilesX = w / TILE_SIZE;
			int numTilesY = h / TILE_SIZE;

			tileRects_ = new SDL_Rect[numTilesX * numTilesY];

			for (size_t y = 0; y < numTilesY; y++)
			{
				for (size_t x = 0; x < numTilesX; x++)
				{
					SDL_Rect r;

					r.w = r.h = TILE_SIZE;

					r.x = (x * TILE_SIZE);
					r.y = (y * TILE_SIZE);

					memcpy(&tileRects_[(numTilesX * y) + x], &r, sizeof(SDL_Rect));
				}
			}

			SDL_FreeSurface(surf);
			
			fprintf(stdout, "Loaded tileset successfully!");
		}
		else
		{
			fprintf(stderr, "Failed to load tileset!\n");
		}
	}
}

void Application::InitializeStates(void)
{
	gameStates_ = new BaseGameState*[2];
	gameStates_[0] = new GameStateMain(this);
	gameStates_[1] = nullptr;
	
	currentGameState_ = GetGameState("GameState.Main");
	
	BaseGameState** ptr = gameStates_;
	
	while (ptr && *ptr)
	{
		(*ptr)->OnInitialize();
		ptr++;
	}
}

#ifdef __EMSCRIPTEN__
/* Please kill me, it hurts to live! */
void MainLoopExtern(void* arg)
{
	MainLoopContext* context = static_cast<MainLoopContext*>(arg);
	((*context->app).*(context->func))(context);
}
#endif

void Application::MainLoop(void* arg)
{
	MainLoopContext* context = static_cast<MainLoopContext*>(arg);

#ifdef __EMSCRIPTEN__
	if (!isRunning_)
	{
		emscripten_cancel_main_loop();
		return;
	}
#endif

	context->newTime = (double)SDL_GetTicks();
	context->frameTime = (context->newTime - context->currentTime) / 1000.0;

	if (context->frameTime > MAX_FRAME_TIME)
	{
		context->frameTime = MAX_FRAME_TIME;
	}

	context->currentTime = context->newTime;
	context->acc += context->frameTime;

	while (context->acc >= FRAMES_ONE_STEP)
	{
		SDL_Event event;

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Stop();
				break;
			case SDL_KEYUP:
#ifndef __EMSCRIPTEN__
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					Stop();
				}
				else
#endif
				{
					HandleInput(event, false);
				}
				break;
			case SDL_KEYDOWN:
				HandleInput(event, true);
				break;
			}
		}

		HandleUpdate((real)FRAMES_ONE_STEP);

		context->acc -= FRAMES_ONE_STEP;

		time_ += FRAMES_ONE_STEP;
		ticks_ += 1;
	}

	smooth_ = (real)(context->acc / FRAMES_ONE_STEP);

	SDL_SetRenderTarget(renderer_, target_);

	HandleDraw(smooth_);

	SDL_SetRenderTarget(renderer_, nullptr);
	SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer_);

	SDL_RenderCopy(renderer_, target_, nullptr, nullptr);

	SDL_RenderPresent(renderer_);
}

int Application::Run(bool fullscreen)
{
	int ret = 0;

	if (!InitializeSDL(fullscreen))
	{
		ret = 1;
	}
	else
	{
		InitializeResources();
		InitializeStates();

		isRunning_ = true;

		/* Funky workarounds for emscripten */

		MainLoopContext context;

#ifdef __EMSCRIPTEN__
		context.app = this;
		context.func = &Application::MainLoop;
#endif

		context.newTime = 0;
		context.currentTime = (double)SDL_GetTicks();
		context.frameTime = 0;
		context.acc = 0;

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(MainLoopExtern, &context, -1, 1);
#else
		while (isRunning_)
		{
			MainLoop(&context);
			SDL_Delay(1);
		}
#endif
	}

	return ret;
}

void Application::HandleInput(SDL_Event& event, bool down)
{
	if (currentGameState_)
	{
		currentGameState_->OnInput(event, down);
	}
}

void Application::HandleUpdate(real delta)
{
	if (currentGameState_)
	{
		currentGameState_->OnUpdate(delta);
	}
}

void Application::HandleDraw(real smooth)
{
	if (currentGameState_)
	{
		currentGameState_->OnDraw();
	}
}

void Application::DrawTile(int x, int y, int tile)
{
	SDL_Rect r;
	
	r.x = (x * TILE_SIZE);
	r.y = (y * TILE_SIZE);
	r.w = r.h = TILE_SIZE;

	SDL_RenderCopy(renderer_, tileSheet_, &tileRects_[tile], &r);
}

void Application::DrawString(int x, int y, const char* str)
{
	size_t len = strnlen(str, 0xFF);

	if (len > 0)
	{
		int strX = x;
		int strY = y;

		for (size_t i = 0; i < len; i++)
		{
			int idx = 0;

			if (str[i] >= 'A' && str[i] <= 'Z')
			{
				idx = (str[i] - 'A') + TILE_TEXT;
			}
			else if (str[i] >= 'a' && str[i] <= 'z')
			{
				idx = (str[i] - 'a') + TILE_TEXT;
			}
			else if (str[i] >= '0' && str[i] <= '9')
			{
				idx = (str[i] - '0') + TILE_NUMBER;
			}
			else if (str[i] == ' ')
			{
				strX++;
			}
			else if (str[i] == '\t')
			{
				strX += 4;
			}
			else if (str[i] == '\n' || str[i] == '\r')
			{
				strX = x;
				strY++;
			}

			if (idx)
			{
				DrawTile(strX++, strY, idx);
			}
		}
	}
}

void Application::DrawBox(int x, int y, int w, int h)
{
	int ix, iy;

	for (ix = x + 1; ix < (x + w) - 1; ix++)
	{
		DrawTile(ix, y, TILE_BOX_TOP);
		DrawTile(ix, (y + h) - 1, TILE_BOX_BOT);
	}

	for (iy = y + 1; iy < (y + h) - 1; iy++)
	{
		DrawTile(x, iy, TILE_BOX_LEFT);
		DrawTile((x + w) - 1, iy, TILE_BOX_RIGHT);
	}

	DrawTile(x, y, TILE_BOX_TOP_LEFT);
	DrawTile((x + w) - 1, y, TILE_BOX_TOP_RIGHT);

	DrawTile(x, (y + h) - 1, TILE_BOX_BOT_LEFT);
	DrawTile((x + w) - 1, (y + h) - 1, TILE_BOX_BOT_RIGHT);
}

BaseGameState** Application::GetGameStates(void)
{
	return gameStates_;
}

BaseGameState* Application::GetGameState(const char* szStateName)
{
	BaseGameState** ptr = gameStates_;
	BaseGameState* result = nullptr;
	
	while (ptr && *ptr)
	{
		if (strncmp((*ptr)->GetStateName(), szStateName, 0xFF) == 0)
		{
			result = *ptr;
			break;
		}
		
		ptr++;
	}
	
	return result;
}

BaseGameState* Application::GetGameState(void)
{
	return currentGameState_;
}

int main(int argc, char* argv[])
{
	Application* app = Application::GetInstance();

	int ret = 0;

	if (!app)
	{
		ret = 1;
	}
	else
	{
		bool fullscreen = false;

		for (size_t i = 0; i < argc; i++)
		{
			if (strnlen(argv[i], 32) > 1)
			{
				if (argv[i][0] == '-' || argv[i][0] == '/')
				{
					if (argv[i][1] == 'f')
					{
						fullscreen = true;
					}
					else if (argv[i][1] == 'w')
					{
						fullscreen = false;
					}
				}
			}
		}

		ret = app->Run(fullscreen);

		delete app;
		app = nullptr;
	}

	return ret;
}
