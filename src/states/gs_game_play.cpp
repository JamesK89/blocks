#include <blocks.hpp>
#include <states/gs_game_play.hpp>
#include <states/gs_high_scores.hpp>

GameStateGamePlay::GameStateGamePlay(Application* app)
	: BaseGameState(app), 
	initialized_(false),
	score_(0), 
	level_(0), 
	playfield_(nullptr), 
	gamestate_(0), 
	prePauseGameState_(0),
	pauseMenuOption_(0), 
	currentShape_(nullptr),
	nextShape_(nullptr), 
	holdShape_(nullptr),
	ghostShape_(nullptr),
	nextTick_(0), 
	lockDelayTicks_(0),
	inputAction_(0),
	shapes_(nullptr),
	numShapes_(0),
	shapePlayed_(nullptr),
	numCompleteLines_(0),
	messages_(),
	hasSwappedShapeThisDrop_(false),
	droppedAQuad_(false),
	shapeOffsetY_(0)
{	
}

GameStateGamePlay::~GameStateGamePlay(void)
{
	if (playfield_)
	{
		delete [] playfield_;
		playfield_ = nullptr;
	}
	
	if (currentShape_)
	{
		delete currentShape_;
		currentShape_ = nullptr;
	}
	
	if (nextShape_)
	{
		delete nextShape_;
		nextShape_ = nullptr;
	}
	
	if (holdShape_)
	{
		delete holdShape_;
		holdShape_ = nullptr;
	}
	
	if (ghostShape_)
	{
		delete ghostShape_;
		ghostShape_ = nullptr;
	}
	
	if (shapes_)
	{
		for (unsigned int i = 0; i < numShapes_; i++)
		{
			if (shapes_[i])
			{
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
			switch (evt.key.keysym.sym)
			{
				case SDLK_LEFT:
					inputAction_ = INPUT_ACTION_SHIFT_LEFT;
				break;
				case SDLK_RIGHT:
					inputAction_ = INPUT_ACTION_SHIFT_RIGHT;
				break;
				case SDLK_UP:
					inputAction_ = INPUT_ACTION_ROTATE;
				break;
				case SDLK_DOWN:
					inputAction_ = INPUT_ACTION_DROP;
				break;
				case SDLK_SPACE:
					inputAction_ = INPUT_ACTION_QUICK_DROP;
				break;
				case SDLK_LSHIFT:
					inputAction_ = INPUT_ACTION_HOLD;
				break;
				case SDLK_ESCAPE:
				case SDLK_PAUSE:
				case SDLK_p:
					pauseMenuOption_ = 0;
					prePauseGameState_ = gamestate_;
					gamestate_ = GAMEPLAY_STATE_PAUSED;
				break;
			}
		}
		else if (gamestate_ == GAMEPLAY_STATE_PAUSED)
		{
			switch (evt.key.keysym.sym)
			{
				case SDLK_UP:
					pauseMenuOption_--;
				break;
				case SDLK_DOWN:
					pauseMenuOption_++;
				break;
				case SDLK_RIGHT:
				case SDLK_RETURN:
					if (pauseMenuOption_ == 0)
					{
						gamestate_ = prePauseGameState_;
					}
					else
					{
						app_->SetGameState("GameState.MainMenu");
					}
				break;
				case SDLK_ESCAPE:
					gamestate_ = prePauseGameState_;
				break;
			}
			
			if (pauseMenuOption_ < 0)
			{
				pauseMenuOption_ = 1;
			}
			else if (pauseMenuOption_ > 1)
			{
				pauseMenuOption_ = 0;
			}
		}
		else if (gamestate_ == GAMEPLAY_STATE_GAME_OVER)
		{
			dynamic_cast<GameStateHighScores*>(app_->SetGameState("GameState.HighScores"))->SubmitScore(score_);
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
				if (lockDelayTicks_ > 0)
					RotateShape(currentShape_);
			}
			else if (inputAction_ == INPUT_ACTION_SHIFT_LEFT)
			{
				if (lockDelayTicks_ > 0)
					incX = -1;
			}
			else if (inputAction_ == INPUT_ACTION_SHIFT_RIGHT)
			{
				if (lockDelayTicks_ > 0)
					incX = 1;
			}
			else if (inputAction_ == INPUT_ACTION_DROP)
			{
				MoveShape(currentShape_, 0, 1);
			}
			else if (inputAction_ == INPUT_ACTION_QUICK_DROP)
			{
				if (currentShape_)
				{
					while (MoveShape(currentShape_, 0, 1))
					{
						lockDelayTicks_ = 0;
					}
				}
			}
			else if (inputAction_ == INPUT_ACTION_HOLD)
			{
				if (!hasSwappedShapeThisDrop_)
				{
					if (!holdShape_)
					{
						holdShape_ = new Shape(app_);
					
						holdShape_->Set(currentShape_);
						holdShape_->SetOrientation(0);
						
						SpawnShape();
					}
					else
					{
						Shape* shp = holdShape_;
						
						holdShape_ = currentShape_;
						currentShape_ = shp;
						
						PositionShapeAtSpawn(currentShape_);
					}
					
					hasSwappedShapeThisDrop_ = true;
				}
			}
			
			if (incX != 0)
			{
				MoveShape(currentShape_, incX, 0);
			}
			
			inputAction_ = INPUT_ACTION_NONE;
			
			UpdateGhostShape();
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
		
#if 0
		if (currentShape_)
		{
			Shape predictShape(app_);
			
			predictShape.Set(currentShape_);
			predictShape.SetOrientation(currentShape_->GetOrientation());
			
			MoveShape(&predictShape, 0, 1);
			
			int px, py;
			int sx, sy;
			predictShape.GetPosition(px, py);
			currentShape_->GetPosition(sx, sy);
			
			shapeOffsetY_ = int((((py - sy) * TILE_SIZE) * nextTick_) * -1);
		}
#endif
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
	DrawNextAndHold();
	DrawPlayfield();
	
	app_->DrawString(playfieldWidth_ + 3, 7, "Next");
	
	if (gamestate_ == GAMEPLAY_STATE_GAME_OVER)
		DrawGameOver();
	else if (gamestate_ == GAMEPLAY_STATE_PAUSED)
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
	srand((unsigned int)time(NULL));
	
#if !defined(__EMSCRIPTEN__) && !defined(WIN32)
	FILE* fp = fopen("/dev/urandom", "r");
	
	if (fp)
	{
		unsigned int r;
		fread(&r, sizeof(r), 1, fp);
		fclose(fp);
		
		srand(r);
	}
#endif
	
	playfieldWidth_ = ((FRAME_WIDTH / TILE_SIZE) >> 1);
	playfieldHeight_ = (FRAME_HEIGHT / TILE_SIZE) - 2;
	
	playfield_ = new unsigned char[playfieldWidth_ * playfieldHeight_];

	numShapes_ = SHAPE_COUNT;
	shapePlayed_ = new bool[numShapes_];
	shapes_ = new Shape*[numShapes_];
	
	for (unsigned int i = 0; i < numShapes_; i++)
	{
		shapes_[i] = new Shape(app_, &SHAPES[i]);
	}
	
	NewGame();
	
	initialized_ = true;
}

void GameStateGamePlay::NewGame(void)
{
	for (unsigned int i = 0; i < numShapes_; i++)
	{
		shapePlayed_[i] = false;
	}
	
	memset(playfield_, ZERO, sizeof(unsigned char) * (playfieldWidth_ * playfieldHeight_));
	
	score_ = 0;
	level_ = 0;
	
	lockDelayTicks_ = GAMEPLAY_LOCK_DELAY_TICKS;
	
	gamestate_ = GAMEPLAY_STATE_PLAYING;
	
	messages_.clear();
	
	if (nextShape_)
	{
		delete nextShape_;
		nextShape_ = nullptr;
	}
	
	if (currentShape_)
	{
		delete currentShape_;
		currentShape_ = nullptr;
	}
	
	if (holdShape_)
	{
		delete holdShape_;
		holdShape_ = nullptr;
	}
	
	CalculateLevel();
		
	SpawnShape();
}

void GameStateGamePlay::DrawLevel(void)
{
	char levelStr[64];
	memset(&levelStr[0], ZERO, sizeof(levelStr));
	snprintf(&levelStr[0], sizeof(levelStr) / sizeof(char), "%i", (level_ + 1));
	
	app_->DrawString(playfieldWidth_ + 3, 1, "LEVEL");
	app_->DrawString(playfieldWidth_ + 4, 2, levelStr);
}

void GameStateGamePlay::DrawScore(void)
{
	char scoreStr[64];
	memset(&scoreStr[0], ZERO, sizeof(scoreStr));
	snprintf(&scoreStr[0], sizeof(scoreStr) / sizeof(char), "%i", score_);
	
	app_->DrawString(playfieldWidth_ + 3, 4, "SCORE");
	app_->DrawString(playfieldWidth_ + 4, 5, scoreStr);
	
	int stringsY = ((FRAME_HEIGHT / TILE_SIZE) - 5);
	
	if ((app_->GetTickCount() % 6) < 3)
	{
		for (vector<const char*>::iterator i = messages_.begin(); i != messages_.end(); ++i)
		{
			if (stringsY >= ((FRAME_HEIGHT / TILE_SIZE) - 1))
			{
				break;
			}
			
			const char* pStr = *i;
			
			app_->DrawString(playfieldWidth_ + 3, stringsY, pStr);
			
			stringsY++;
		}
	}
}

void GameStateGamePlay::DrawNextAndHold(void)
{		
	app_->DrawString(playfieldWidth_ + 3, 7, "NXT HLD");

	if (nextShape_)
	{
		int ox, oy;
		
		nextShape_->GetOffset(ox, oy);
		
		nextShape_->SetPosition((playfieldWidth_ + 3), 9);
		nextShape_->Draw(false);
	}
	
	if (holdShape_)
	{
		int ox, oy;
		
		holdShape_->SetOrientation(0);
		holdShape_->GetOffset(ox, oy);
		
		holdShape_->SetPosition((playfieldWidth_ + 7), 9);
		holdShape_->Draw(false);
	}
}

void GameStateGamePlay::DrawPlayfield()
{
	for (unsigned int y = 0; y < playfieldHeight_; y++)
	{
		bool isACompleteLine = IsLineComplete(y);
		
		for(unsigned int x = 0; x < playfieldWidth_; x++)
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
	
	if (gamestate_ == GAMEPLAY_STATE_PLAYING)
	{
		DrawShape(ghostShape_, GAMEPLAY_GHOST_ALPHA);
	}
	
	int offsX, offsY;
	
	app_->GetDrawOffset(&offsX, &offsY);
	app_->SetDrawOffset(offsX, offsY + shapeOffsetY_);
	
	DrawShape(currentShape_);
	
	app_->SetDrawOffset(offsX, offsY);
	
	SDL_RenderSetClipRect(app_->GetRenderer(), &olRect);
}

void GameStateGamePlay::DrawPause()
{
	SDL_RenderSetClipRect(app_->GetRenderer(), nullptr);
	
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	r.w = FRAME_WIDTH;
	r.h = FRAME_HEIGHT;
	
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0xDD);
	SDL_RenderFillRect(app_->GetRenderer(), &r);
	
	if ((app_->GetTickCount() % 100) > 50)
		app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 4, ((FRAME_HEIGHT / TILE_SIZE) >> 1) - 4, "PAUSED");
		
	
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 5, ((FRAME_HEIGHT / TILE_SIZE) >> 1), "CONTINUE");
	app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 3, ((FRAME_HEIGHT / TILE_SIZE) >> 1) + 2, "EXIT");
	
	if ((app_->GetTickCount() % 24) > 12)
	{
		app_->DrawTile((((FRAME_WIDTH / TILE_SIZE) >> 1) - 6) + (pauseMenuOption_ * 2), ((FRAME_HEIGHT / TILE_SIZE) >> 1) + (pauseMenuOption_ * 2), TILE_CURSOR_LEFT);
	}
}

void GameStateGamePlay::DrawGameOver(void)
{
	SDL_RenderSetClipRect(app_->GetRenderer(), nullptr);
	
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	r.w = FRAME_WIDTH;
	r.h = FRAME_HEIGHT;
	
	SDL_SetRenderDrawColor(app_->GetRenderer(), 0x00, 0x00, 0x00, 0xAF);
	SDL_RenderFillRect(app_->GetRenderer(), &r);
	
	if ((app_->GetTickCount() % 50) > 25)
		app_->DrawString(((FRAME_WIDTH / TILE_SIZE) >> 1) - 5, ((FRAME_HEIGHT / TILE_SIZE) >> 1) - 4, "GAME OVER");
}

void GameStateGamePlay::OnTick(void)
{
	if (gamestate_ == GAMEPLAY_STATE_PLAYING)
	{
		ClearCompleteLines();
		
		if (!currentShape_)
		{
			SpawnShape();
			
			lockDelayTicks_ = GAMEPLAY_LOCK_DELAY_TICKS;
			
			UpdateGhostShape();
			
			if (DoesShapeCollide(currentShape_))
			{
				gamestate_ = GAMEPLAY_STATE_GAME_OVER;
			}
		}
		else if (!MoveShape(currentShape_, 0, 1))
		{
			if (--lockDelayTicks_ < 1)
			{
				int stlx, stly;
				int sbrx, sbry;
				
				currentShape_->GetInnerBounds(stlx, stly, sbrx, sbry);
				
				if (stly <= 0)
				{
					gamestate_ = GAMEPLAY_STATE_GAME_OVER;
				}
				
				ImpressShapeOntoPlayfield(currentShape_);
				
				currentShape_ = nullptr;
				hasSwappedShapeThisDrop_ = false;
				
				if (CheckForCompleteLines())
				{
					UpdateScore();
					nextTick_ += real(0.25);
					gamestate_ = GAMEPLAY_STATE_SCORING;
				}
			}
		}
		else
		{
			UpdateGhostShape();
			lockDelayTicks_ = GAMEPLAY_LOCK_DELAY_TICKS;
		}

		if (gamestate_ == GAMEPLAY_STATE_PLAYING)
		{
			real tickInc = real(0.5 - (level_ * 0.05));
			
			if (tickInc < real(0.1))
				tickInc = real(0.1);
			
			nextTick_ += tickInc;
		}
	}
	else if (gamestate_ == GAMEPLAY_STATE_SCORING)
	{
		gamestate_ = GAMEPLAY_STATE_PLAYING;
		nextTick_ += 1.0;
	}
}

void GameStateGamePlay::PositionShapeAtSpawn(Shape* shape)
{
	if (shape)
	{	
		int tlx, tly;
		int brx, bry;
		
		int offx, offy;
	
		shape->SetOrientation(0);
		shape->GetInnerBounds(tlx, tly, brx, bry);
		shape->GetOffset(offx, offy);
		
		shape->SetPosition((((playfieldWidth_ >> 1) - (SHAPE_WIDTH >> 1))) - offx, (((bry - tly) + offy) * -1));
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
			else if (brx > int(playfieldWidth_ + 1))
			{
				shape->SetPosition(x - 1, y);
				recheck = true;
			}
			else if (bry > int(playfieldHeight_ + 1))
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
				
				if (pfX >= 0 && pfY >= 0 && pfX <= int(playfieldWidth_) && pfY <= int(playfieldHeight_))
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

Shape* GameStateGamePlay::DraftShape(void)
{
	Shape* result = nullptr;
	
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
	
	do
	{
		int idx = rand() % numShapes_;
		
		if (!shapePlayed_[idx])
		{
			result = shapes_[idx];
			shapePlayed_[idx] = true;
			break;
		}
	} while (true);
	
#if 0
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
#endif
	
	return result;
}

void GameStateGamePlay::SpawnShape(void)
{
	if (!nextShape_)
	{
		nextShape_ = new Shape(app_);
		nextShape_->Set(DraftShape());
	}
		
	if (!currentShape_)
	{
		currentShape_ = new Shape(app_);
		currentShape_->Set(DraftShape());
	}
	
	currentShape_->Set(nextShape_);
	nextShape_->Set(DraftShape());

	nextShape_->SetOrientation(0);	
	PositionShapeAtSpawn(currentShape_);
}

void GameStateGamePlay::DrawShape(const Shape* shape, unsigned char alpha)
{
	if (shape)
	{
		shape->Draw(true, alpha);
		
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

void GameStateGamePlay::UpdateGhostShape(void)
{
	if (currentShape_)
	{
		if (!ghostShape_)
		{
			ghostShape_ = new Shape(app_);
		}

		ghostShape_->Set(currentShape_);
		ghostShape_->SetTile(TILE_BLOCK_GRY);
		ghostShape_->SetOrientation(currentShape_->GetOrientation());
		
		while (MoveShape(ghostShape_, 0, 1))
		{
			/* ... */
		}
	}
}

bool GameStateGamePlay::CheckForCompleteLines(void)
{
	bool result = false;
	
	numCompleteLines_ = 0;
	
	for (unsigned int y = 0; y < playfieldHeight_; y++)
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
	memset(tempPlayfield, ZERO, sizeof(unsigned char) * (playfieldWidth_ * playfieldHeight_));
	
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
	
	for (unsigned int x = 0; x < playfieldWidth_; x++)
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
	bool result = false;
	
	for (unsigned int x = 0; x < playfieldWidth_; x++)
	{
		if (IsColumnComplete(x))
		{
			result = true;
			break;
		}
	}
	
	return result;
}

bool GameStateGamePlay::IsColumnComplete(int x) const
{
	bool result = true;
	
	const unsigned char* tile = &playfield_[x];
	
	for (unsigned int y = 0; y < playfieldHeight_; y++)
	{
		const unsigned char t = *tile;
		
		if (!t)
		{
			result = false;
			break;
		}
	}
	
	return result;
}

int GameStateGamePlay::FindNumberOfContiguousLines(int numLines) const
{
	int result = 0;
	int lineCount = 0;
	
	for (unsigned int y = 0; y <= playfieldHeight_; y++)
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
		droppedAQuad_ = false;
	}
	
	if (dubs)
	{
		score_ += dubs * 15;
		messages_.push_back("DOUBLE");
		droppedAQuad_ = false;
	}
	
	if (trips)
	{
		score_ += trips * 25;
		messages_.push_back("TRIPLE");
		droppedAQuad_ = false;
	}
	
	if (quads)
	{
		if (droppedAQuad_)
		{
			score_ += quads * 75;
			messages_.push_back("DUB!");
		}
		else
		{
			score_ += quads * 50;
			messages_.push_back("QUADS!");
		}
		
		droppedAQuad_ = true;
	}
	
	int oldLevel = level_;
	
	CalculateLevel();
	
	if (level_ > oldLevel)
	{
		messages_.push_back("LEVEL");
	}
}

void GameStateGamePlay::CalculateLevel(void)
{
	int base = 2;
	int exp = 5;
	
	int newLevel = 0;
	
	int product = 0;
	
	int score = score_;
	
	while (score > 0)
	{
		product = int(powf(base, exp++));
		
		if (score < product)
			break;

		score -= product;
		newLevel++;
	}
	
	level_ = newLevel;
}