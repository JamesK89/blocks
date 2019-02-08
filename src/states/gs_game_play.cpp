#include <blocks.hpp>
#include <states/gs_game_play.hpp>

GameStateGamePlay::GameStateGamePlay(Application* app)
	: BaseGameState(app), 
	score_(0), 
	level_(0), 
	playfield_(nullptr), 
	gamestate_(0), 
	pauseMenuOption_(0), 
	currentShape_(nullptr), 
	nextShape_(nullptr), 
	nextTick_(0), 
	inputAction_(0),
	shapes_(nullptr),
	numShapes_(0),
	shapePlayed_(nullptr),
	numCompleteLines_(0),
	messages_()
{	
}

GameStateGamePlay::~GameStateGamePlay(void)
{
	if (playfield_)
	{
		delete [] playfield_;
		playfield_ = nullptr;
	}
	
	if (shapes_)
	{
		for (unsigned int i = 0; i < numShapes_; i++)
		{
			if (shapes_[i])
			{
				if (currentShape_ == shapes_[i])
				{
					currentShape_ = nullptr;
				}
				
				delete shapes_[i];
				shapes_[i] = nullptr;
			}
		}
		
		delete [] shapes_;
		shapes_ = nullptr;
	}
	
	if (shapePlayed_)
	{
		delete [] shapePlayed_;
		shapePlayed_ = nullptr;
	}
	
	if (currentShape_)
	{
		delete currentShape_;
		currentShape_ = nullptr;
	}
}

const char* GameStateGamePlay::GetStateName(void) const
{
	return "GameState.GamePlay";
}

void GameStateGamePlay::OnInput(SDL_Event& evt, bool down)
{
	if (down)
	{		
		if (gamestate_ == GAMEPLAY_STATE_PLAYING)
		{
			if (evt.key.keysym.sym == SDLK_LEFT)
			{
				inputAction_ = INPUT_ACTION_SHIFT_LEFT;
			}
			else if (evt.key.keysym.sym == SDLK_RIGHT)
			{
				inputAction_ = INPUT_ACTION_SHIFT_RIGHT;
			}
			else if (evt.key.keysym.sym == SDLK_UP)
			{
				inputAction_ = INPUT_ACTION_ROTATE;
			}
			else if (evt.key.keysym.sym == SDLK_DOWN)
			{
				inputAction_ = INPUT_ACTION_DROP;
			}
		}
	}
}

void GameStateGamePlay::OnUpdate(real delta)
{
	if (gamestate_ == GAMEPLAY_STATE_PLAYING)
	{
		nextTick_ -= delta;
		messageTick_ -= delta;
		
		if (inputAction_ != INPUT_ACTION_NONE)
		{
			int incX = 0;
			
			if (inputAction_ == INPUT_ACTION_ROTATE)
			{
				RotateShape(currentShape_);
			}
			else if (inputAction_ == INPUT_ACTION_SHIFT_LEFT)
			{
				incX = -1;
			}
			else if (inputAction_ == INPUT_ACTION_SHIFT_RIGHT)
			{
				incX = 1;
			}
			else if (inputAction_ == INPUT_ACTION_DROP)
			{
				MoveShape(currentShape_, 0, 1);
			}
			
			if (incX != 0)
			{
				MoveShape(currentShape_, incX, 0);
			}
			
			inputAction_ = INPUT_ACTION_NONE;
		}
		
		if (messageTick_ <= 0.0f)
		{
			if (messages_.size() > 0)
			{
				messages_.pop_back();
			}
			
			messageTick_ += 0.5;
		}
		
		if (nextTick_ <= 0.0f)
		{
			OnTick();
		}
	}
	else if (gamestate_ == GAMEPLAY_STATE_SCORING)
	{
		nextTick_ -= delta;
		
		if (nextTick_ <= 0.0f)
		{
			OnTick();
		}
	}
}

void GameStateGamePlay::OnDraw(void)
{
	SDL_RenderSetClipRect(app_->GetRenderer(), nullptr);
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(app_->GetRenderer());
	
	DrawScore();
	DrawLevel();
	DrawPlayfield();
	
	app_->DrawString(playfieldWidth_ + 3, 7, "Next");
	
	if (gamestate_ == GAMEPLAY_STATE_PAUSED)
		DrawPause();
}

void GameStateGamePlay::OnSuspend(BaseGameState* newState)
{
}

void GameStateGamePlay::OnResume(BaseGameState* oldState)
{
}

void GameStateGamePlay::OnInitialize(void)
{
	srand(time(NULL));
	
	FILE* fp = fopen("/dev/urandom", "r");
	
	if (fp)
	{
		unsigned int r;
		fread(&r, sizeof(r), 1, fp);
		fclose(fp);
		
		srand(r);
	}
	
	playfieldWidth_ = ((WINDOW_WIDTH / TILE_SIZE) >> 1);
	playfieldHeight_ = (WINDOW_HEIGHT / TILE_SIZE) - 2;
	
	playfield_ = new unsigned char[playfieldWidth_ * playfieldHeight_];
	
	memset(playfield_, NULL, sizeof(unsigned char) * (playfieldWidth_ * playfieldHeight_));

	numShapes_ = SHAPE_COUNT;
	shapePlayed_ = new bool[numShapes_];
	shapes_ = new Shape*[numShapes_];
	
	for (unsigned int i = 0; i < numShapes_; i++)
	{
		shapePlayed_[i] = false;
		shapes_[i] = new Shape(app_, &SHAPES[i]);
	}
	
	gamestate_ = GAMEPLAY_STATE_PLAYING;
	
	messages_.clear();
}

void GameStateGamePlay::DrawLevel(void)
{
	char levelStr[64];
	memset(&levelStr[0], NULL, sizeof(levelStr));
	snprintf(&levelStr[0], sizeof(levelStr) / sizeof(char), "%i", (level_ + 1));
	
	app_->DrawString(playfieldWidth_ + 3, 1, "LEVEL");
	app_->DrawString(playfieldWidth_ + 4, 2, levelStr);
}

void GameStateGamePlay::DrawScore(void)
{
	char scoreStr[64];
	memset(&scoreStr[0], NULL, sizeof(scoreStr));
	snprintf(&scoreStr[0], sizeof(scoreStr) / sizeof(char), "%i", score_);
	
	app_->DrawString(playfieldWidth_ + 3, 4, "SCORE");
	app_->DrawString(playfieldWidth_ + 4, 5, scoreStr);
	
	int stringsY = ((WINDOW_HEIGHT / TILE_SIZE) - 5);
	
	if ((app_->GetTickCount() % 6) < 3)
	{
		for (vector<const char*>::iterator i = messages_.begin(); i != messages_.end(); ++i)
		{
			if (stringsY >= ((WINDOW_HEIGHT / TILE_SIZE) - 1))
			{
				break;
			}
			
			const char* pStr = *i;
			
			app_->DrawString(playfieldWidth_ + 3, stringsY, pStr);
			
			stringsY++;
		}
	}
}

void GameStateGamePlay::DrawPlayfield()
{
	for (int y = 0; y < playfieldHeight_; y++)
	{
		bool isACompleteLine = IsLineComplete(y);
		
		for(int x = 0; x < playfieldWidth_; x++)
		{
			unsigned char tile = playfield_[(y * playfieldWidth_) + x];
			
			if (tile && (!isACompleteLine || (isACompleteLine && (app_->GetTickCount() % 6) > 3)))
			{
				app_->DrawTile(x + 1, y + 1, tile);
			}
		}
	}
	
	app_->DrawShadowBox(0, 0, playfieldWidth_ + 2, playfieldHeight_ + 2);
	
	SDL_Rect olRect;
	
	SDL_RenderGetClipRect(app_->GetRenderer(), &olRect);
	
	SDL_Rect pfRect;
	
	pfRect.x = 1 * TILE_SIZE;
	pfRect.y = 1 * TILE_SIZE;
	pfRect.w = playfieldWidth_  * TILE_SIZE;
	pfRect.h = playfieldHeight_ * TILE_SIZE;
	
	SDL_RenderSetClipRect(app_->GetRenderer(), &pfRect);
	
	DrawShape(currentShape_);
	
	SDL_RenderSetClipRect(app_->GetRenderer(), &olRect);
}

void GameStateGamePlay::DrawPause()
{
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	r.w = WINDOW_WIDTH;
	r.h = WINDOW_HEIGHT;
	
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0x80);
	SDL_RenderFillRect(app_->GetRenderer(), &r);
	
	if ((app_->GetTickCount() % 100) > 50)
		app_->DrawString(((WINDOW_WIDTH / TILE_SIZE) >> 1) - 4, ((WINDOW_HEIGHT / TILE_SIZE) >> 1) - 4, "PAUSED");
		
	
	app_->DrawString(((WINDOW_WIDTH / TILE_SIZE) >> 1) - 5, ((WINDOW_HEIGHT / TILE_SIZE) >> 1), "CONTINUE");
	app_->DrawString(((WINDOW_WIDTH / TILE_SIZE) >> 1) - 3, ((WINDOW_HEIGHT / TILE_SIZE) >> 1) + 2, "EXIT");
	
	if ((app_->GetTickCount() % 24) > 12)
	{
		app_->DrawTile(((WINDOW_WIDTH / TILE_SIZE) >> 1) - 6, ((WINDOW_HEIGHT / TILE_SIZE) >> 1) + pauseMenuOption_, TILE_CURSOR_LEFT);
	}
}

void GameStateGamePlay::OnTick(void)
{
	if (gamestate_ == GAMEPLAY_STATE_PLAYING)
	{
		if (!CheckForEndGame())
		{
			ClearCompleteLines();
			
			if (!currentShape_)
			{
				SpawnShape();
			}
			else if (!MoveShape(currentShape_, 0, 1))
			{
				ImpressShapeOntoPlayfield(currentShape_);
				currentShape_ = nullptr;
				
				if (CheckForCompleteLines())
				{
					UpdateScore();
					nextTick_ += 1.0;
					gamestate_ = GAMEPLAY_STATE_SCORING;
				}
			}
		
			if (gamestate_ == GAMEPLAY_STATE_PLAYING)
				nextTick_ += 1.0f - (level_ * 0.1f);
		}
		else
		{
			gamestate_ = GAMEPLAY_STATE_GAME_OVER;
		}
	}
	else if (gamestate_ == GAMEPLAY_STATE_SCORING)
	{
		gamestate_ = GAMEPLAY_STATE_PLAYING;
		nextTick_ += 1.0;
	}
	
}

void GameStateGamePlay::ClampShape(Shape* shape)
{
	if (shape)
	{
		bool recheck = false;
		
		do
		{
			recheck = false;
			
			int tlx, tly;
			int brx, bry;
			
			int x, y;
			
			shape->GetPosition(x, y);
			shape->GetInnerBounds(tlx, tly, brx, bry);
			
			if (tlx < 1)
			{
				shape->SetPosition(x + 1, y);
				recheck = true;
			}
			else if (brx > playfieldWidth_ + 1)
			{
				shape->SetPosition(x - 1, y);
				recheck = true;
			}
			else if (bry > playfieldHeight_ + 1)
			{
				shape->SetPosition(x, y - 1);
				recheck = true;
			}
		} while (recheck);
	}
}

bool GameStateGamePlay::MoveShape(Shape* shape, int dx, int dy)
{		
	bool result = true;
	
	if (shape)
	{
		int x, y;
		int oX, oY;
		
		shape->GetPosition(x, y);
		
		oX = x;
		oY = y;
		
		x += dx;
		y += dy;
		
		shape->SetPosition(x, y);
		
		ClampShape(shape);
		shape->GetPosition(x, y);
		
		bool doesCollide = DoesShapeCollide(shape);
		
		if (oY == y || doesCollide)
		{
			if (doesCollide)
			{
				shape->SetPosition(oX, oY);
			}
			result = false;
		}
	}
	else
	{
		result = false;
	}
	
	return result;
}

bool GameStateGamePlay::RotateShape(Shape* shape)
{
	bool result = false;
	
	if (shape)
	{
		unsigned char orient = shape->GetOrientation();
		int x, y;
		
		shape->GetPosition(x, y);
		
		int i = orient + 1;
		
		for (int i = orient + 1; i < (orient + shape->GetNumOrientations()); i++)
		{
			shape->SetOrientation(i % shape->GetNumOrientations());
			ClampShape(shape);
			
			if (DoesShapeCollide(shape))
			{
				shape->SetPosition(x, y - 1);
				ClampShape(shape);
				
				if (!DoesShapeCollide(shape))
				{
					result = true;
					break;
				}
			}
			else
			{
				result = true;
				break;
			}
		}
		
		if (!result)
		{
			shape->SetOrientation(orient);
			shape->SetPosition(x, y);
		}
	}
	
	return result;
}

bool GameStateGamePlay::DoesShapeCollide(const Shape* shape) const
{
	bool result = false;
	
	if (shape)
	{
		int tlx, tly;
		int brx, bry;
		
		int x, y;
		
		int tileIdx = shape->GetTile();
		
		shape->GetPosition(x, y);
		shape->GetBounds(tlx, tly, brx, bry);
		
		for (int bx = tlx; bx < brx; bx++)
		{
			for (int by = tly; by < bry; by++)
			{
				int innerX = bx - tlx;
				int innerY = by - tly;
				
				int pfX = (tlx + innerX) - 1;
				int pfY = (tly + innerY) - 1;
				
				if (pfX >= 0 && pfY >= 0)
				{
					unsigned char tile = shape->GetShapeTile(innerX, innerY);
						
					if ((tile & 1) && (playfield_[(playfieldWidth_ * pfY) + pfX]))
					{
						result = true;
						break;
					}
				}
			}
		}
	}
	
	return result;
}

void GameStateGamePlay::ImpressShapeOntoPlayfield(const Shape* shape)
{
	if (shape)
	{
		int tlx, tly;
		int brx, bry;
		
		int x, y;
		
		int tileIdx = shape->GetTile();
		
		shape->GetPosition(x, y);
		shape->GetBounds(tlx, tly, brx, bry);
		
		for (int bx = tlx; bx < brx; bx++)
		{
			for (int by = tly; by < bry; by++)
			{
				int innerX = bx - tlx;
				int innerY = by - tly;
				
				int pfX = (tlx + innerX) - 1;
				int pfY = (tly + innerY) - 1;
				
				if (pfX >= 0 && pfY >= 0)
				{
					unsigned char tile = shape->GetShapeTile(innerX, innerY);
						
					if (tile & 1)
					{
						playfield_[(playfieldWidth_ * pfY) + pfX] = tileIdx;
					}
				}
			}
		}
	}
}

void GameStateGamePlay::SpawnShape(void)
{
	unsigned int numAvailableShapes = 0;
	
	for (unsigned int i = 0; i < numShapes_; i++)
	{
		numAvailableShapes += (shapePlayed_[i] ? 0 : 1);
	}
	
	if (numAvailableShapes < 1)
	{
		for (unsigned int j = 0; j < numShapes_; j++)
		{
			shapePlayed_[j] = false;
		}
		
		numAvailableShapes = numShapes_;
	}
	
	currentShape_ = nullptr;
	
	do
	{
		int idx = rand() % numShapes_;
		
		if (!shapePlayed_[idx])
		{
			currentShape_ = shapes_[idx];
			shapePlayed_[idx] = true;
			break;
		}
	} while (true);
	
	int mostBottomY = 0;
	
	for (unsigned int y = 0; y < SHAPE_HEIGHT; y++)
	{
		for (unsigned int x = 0; x < SHAPE_WIDTH; x++)
		{
			if (currentShape_->GetShapeTile(x, y) & 1)
			{
				mostBottomY = y;
			}
		}
	}
	
	currentShape_->SetPosition(((playfieldWidth_ >> 1) - (SHAPE_WIDTH >> 1)) + 1, 1);
}

void GameStateGamePlay::DrawShape(const Shape* shape)
{
	if (shape)
	{
		shape->Draw();
		
#if 0
		int tlx, tly;
		int brx, bry;
		
		SDL_Rect r;
		/*
		shape->GetBounds(tlx, tly, brx, bry);
		
		r.x = tlx * TILE_SIZE;
		r.y = tly * TILE_SIZE;
		r.w = (brx * TILE_SIZE) - r.x;
		r.h = (bry * TILE_SIZE) - r.y;
		
		SDL_SetRenderDrawColor(app_->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(app_->GetRenderer(), &r);
		*/
		shape->GetInnerBounds(tlx, tly, brx, bry);
		
		r.x = tlx * TILE_SIZE;
		r.y = tly * TILE_SIZE;
		r.w = (brx * TILE_SIZE) - r.x;
		r.h = (bry * TILE_SIZE) - r.y;
		
		SDL_SetRenderDrawColor(app_->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(app_->GetRenderer(), &r);
#endif
	}
}

bool GameStateGamePlay::CheckForCompleteLines(void)
{
	bool result = false;
	
	numCompleteLines_ = 0;
	
	for (int y = 0; y < playfieldHeight_; y++)
	{
		if (IsLineComplete(y))
		{	
			numCompleteLines_++;
			result = true;
		}
	}
	
	return result;
}

void GameStateGamePlay::ClearCompleteLines(void)
{
	unsigned char* tempPlayfield = new unsigned char[playfieldWidth_ * playfieldHeight_];
	memset(tempPlayfield, NULL, sizeof(unsigned char) * (playfieldWidth_ * playfieldHeight_));
	
	int pfY = playfieldHeight_ - 1;
	int tmpY = playfieldHeight_ - 1;
	
	while (pfY > -1 && tmpY > -1)
	{
		if (IsLineComplete(pfY))
		{
			pfY--;
		}
		else
		{
			memcpy(&tempPlayfield[playfieldWidth_ * tmpY], &playfield_[playfieldWidth_ * pfY], sizeof(unsigned char) * playfieldWidth_);
			
			pfY--;
			tmpY--;
		}
	}
	
	memcpy(playfield_, tempPlayfield, sizeof(unsigned char) * (playfieldWidth_ * playfieldHeight_));
	
	delete [] tempPlayfield;
	numCompleteLines_ = 0;
}

bool GameStateGamePlay::IsLineComplete(int y) const
{
	bool result = true;
	
	for (int x = 0; x < playfieldWidth_; x++)
	{
		if (!playfield_[(playfieldWidth_ * y) + x])
		{
			result = false;
			break;
		}
	}
	
	return result;
}


bool GameStateGamePlay::CheckForCompleteColumns(void)
{
	return false;
}

bool GameStateGamePlay::IsColumnComplete(int x) const
{
	return false;
}

int GameStateGamePlay::FindNumberOfContiguousLines(int numLines) const
{
	int result = 0;
	int lineCount = 0;
	
	for (int y = 0; y <= playfieldHeight_; y++)
	{
		if (IsLineComplete(y))
		{
			lineCount++;
		}
		else
		{
			if (numLines == lineCount)
			{
				result++;
			}
			
			lineCount = 0;
		}
	}
	
	return result;
}

void GameStateGamePlay::UpdateScore(void)
{
	int singles = FindNumberOfContiguousLines(1);
	int dubs = FindNumberOfContiguousLines(2);
	int trips = FindNumberOfContiguousLines(3);
	int quads = FindNumberOfContiguousLines(4);
	
	if (singles)
	{
		score_ += singles * 5;
	}
	
	if (dubs)
	{
		score_ += dubs * 15;
		messages_.push_back("DOUBLE");
	}
	
	if (trips)
	{
		score_ += trips * 25;
		messages_.push_back("TRIPLE");
	}
	
	if (quads)
	{
		score_ += quads * 50;
		messages_.push_back("QUADS!");
	}
	
	int oldLevel = level_;
	
	level_ = (score_ / 100);
	
	if (level_ > oldLevel)
	{
		messages_.push_back("LEVEL");
	}
}

bool GameStateGamePlay::CheckForEndGame(void) const
{
	return false;
}
