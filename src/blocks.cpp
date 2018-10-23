#include <blocks.hpp>

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

BlocksApplication* BlocksApplication::instance_ = nullptr;

typedef struct s_main_loop_context_t
{
#ifdef __EMSCRIPTEN__
	BlocksApplication* app;
	void (BlocksApplication::*func)(void* arg);
#endif

	double newTime;
	double currentTime;
	double frameTime;
	double acc;
} MainLoopContext;

BlocksApplication* BlocksApplication::GetInstance(void)
{
	if (!instance_)
		instance_ = new BlocksApplication();

	return instance_;
}

BlocksApplication::BlocksApplication(void) :
	window_(nullptr), renderer_(nullptr), tileSheet_(nullptr), isRunning_(false), smooth_(0), ticks_(0), time_(0)
{
}

BlocksApplication::~BlocksApplication(void)
{
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

void BlocksApplication::Stop(void)
{
	isRunning_ = false;
}

bool BlocksApplication::IsRunning(void) const
{
	return isRunning_;
}

ticks_t BlocksApplication::GetTickCount(void) const
{
	return ticks_;
}

gametime_t BlocksApplication::GetGameTime(void) const
{
	return time_;
}

SDL_Renderer* BlocksApplication::GetRenderer(void)
{
	return renderer_;
}

SDL_Window* BlocksApplication::GetWindow(void)
{
	return window_;
}

bool BlocksApplication::InitializeSDL(bool fullscreen)
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
			window_ = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

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

void BlocksApplication::InitializeResources(void)
{
	if (!tileSheet_)
	{
		SDL_Surface* surf = SDL_LoadBMP("../assets/sheet.bmp");

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
		}
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

void BlocksApplication::MainLoop(void* arg)
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
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					Stop();
				}
				else
				{
					HandleInput(event, false);
				}
				break;
			case SDL_KEYDOWN:
				HandleInput(event, true);
				break;
			}
		}

		HandleUpdate((float)FRAMES_ONE_STEP);

		context->acc -= FRAMES_ONE_STEP;

		time_ += FRAMES_ONE_STEP;
		ticks_ += 1;
	}

	smooth_ = (float)(context->acc / FRAMES_ONE_STEP);

	SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer_);

	HandleRender();

	SDL_RenderPresent(renderer_);
}

int BlocksApplication::Run(bool fullscreen)
{
	int ret = 0;

	if (!InitializeSDL(fullscreen))
	{
		ret = 1;
	}
	else
	{
		InitializeResources();

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

void BlocksApplication::HandleUpdate(float delta)
{
}

void BlocksApplication::HandleInput(SDL_Event& event, bool down)
{
}

void BlocksApplication::HandleRender(void)
{
	SDL_Rect r;
	r.x = r.y = 0;
	r.w = r.h = TILE_SIZE;
	SDL_RenderCopy(renderer_, tileSheet_, &tileRects_[TILE_BOX_BOT_RIGHT], &r);
}

int main(int argc, char* argv[])
{
	BlocksApplication* app = BlocksApplication::GetInstance();

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
