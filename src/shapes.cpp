#include <blocks.hpp>
#include <shapes.hpp>

Shape::Shape(Application* app) :
	app_(app), 
	shapeInfo_(nullptr), 
	x_(0), 
	y_(0), 
	offX_(0), 
	offY_(0), 
	ibbOffX_(9999),
	ibbOffY_(9999),
	orientation_(0), 
	tile_(TILE_BLOCK_RED)
{
}

Shape::Shape(Application* app, const ShapeInfo* info) :
	app_(app), 
	shapeInfo_(nullptr), 
	x_(0), 
	y_(0), 
	offX_(0), 
	offY_(0), 
	ibbOffX_(9999),
	ibbOffY_(9999),
	orientation_(0), 
	tile_(TILE_BLOCK_RED)
{	
	Set(info);
}

Shape::Shape(Application* app, const Shape* shape) :
	app_(app), 
	shapeInfo_(nullptr), 
	x_(0), 
	y_(0), 
	offX_(0), 
	offY_(0), 
	ibbOffX_(9999),
	ibbOffY_(9999),
	orientation_(0), 
	tile_(TILE_BLOCK_RED)
{	
	Set(shape);
}

Shape::~Shape(void)
{
	shapeInfo_ = nullptr;
}

void Shape::Set(const ShapeInfo* info)
{
	if (info)
	{
		shapeInfo_ = info;
		tile_ = info->tile;
		CalculateOffset();
	}
}

void Shape::Set(const Shape* shape)
{
	if (shape)
	{
		shapeInfo_ = shape->shapeInfo_;
		tile_ = shape->tile_;
		x_ = shape->x_;
		y_ = shape->y_;
		offX_ = shape->offX_;
		offY_ = shape->offY_;
		ibbOffX_ = shape->ibbOffX_;
		ibbOffY_ = shape->ibbOffY_;
	}
}

void Shape::Draw(bool withOffset) const
{
	for (int y = 0; y < SHAPE_HEIGHT; y++)
	{
		for (int x = 0; x < SHAPE_WIDTH; x++)
		{
			bool isTile = (shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] & 1);
			
			if (isTile)
			{
				int realX, realY;
			
				if (withOffset)
				{
					realX = (offX_ + x_) + x;
					realY = (offY_ + y_) + y;
				}
				else
				{
					realX = (x_ + x) + ibbOffX_;
					realY = (y_ + y) + ibbOffY_;
				}
			
				app_->DrawTile(realX, realY, tile_);
			}
		}
	}
}

unsigned char Shape::GetNumOrientations(void) const
{
	return shapeInfo_->num;
}

unsigned char Shape::GetOrientation(void) const
{
	return orientation_;
}

void Shape::SetOrientation(unsigned char n)
{
	orientation_ = n;
	CalculateOffset();
}

void Shape::GetPosition(int& x, int& y) const
{
	x = x_;
	y = y_;
}

void Shape::SetPosition(int x, int y)
{
	x_ = x;
	y_ = y;
}

int Shape::GetTile(void) const
{
	return tile_;
}

void Shape::SetTile(int tile)
{
	tile_ = tile;
}

void Shape::CalculateOffset(void)
{
	ibbOffX_ = SHAPE_WIDTH + 1;
	ibbOffY_ = SHAPE_HEIGHT + 1;
	
	for (int y = 0; y < SHAPE_HEIGHT; y++)
	{
		for (int x = 0; x < SHAPE_WIDTH; x++)
		{
			bool isTile = (shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] & 1);
			bool isPivot = (shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] & 2);
			
			if (isTile)
			{
				ibbOffX_ = MIN(ibbOffX_, x);
				ibbOffY_ = MIN(ibbOffY_, y);
			}
			
			if (isPivot)
			{
				offX_ = x * -1;
				offY_ = y * -1;
			}
		}
	}
	
	ibbOffX_ *= -1;
	ibbOffY_ *= -1;
}

void Shape::GetOffset(int& offX, int& offY) const
{
	offX = offX_;
	offY = offY_;
}

void Shape::GetBounds(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY) const
{
	topLeftX = x_ + offX_;
	topLeftY = y_ + offY_;
	
	botRightX = (x_ + SHAPE_WIDTH) + offX_;
	botRightY = (y_ + SHAPE_HEIGHT) + offY_;
}

void Shape::GetInnerBounds(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY) const
{
	int tlX = SHAPE_WIDTH + 1;
	int tlY = SHAPE_HEIGHT + 1;
	
	int brX = -1;
	int brY = -1;
	
	topLeftX = 0;
	topLeftY = 0;
	
	botRightX = 0;
	botRightY = 0;
	
	for (int ty = 0; ty < SHAPE_HEIGHT; ty++)
	{
		for (int tx = 0; tx < SHAPE_WIDTH; tx++)
		{
			bool isTile = (shapeInfo_->data[orientation_][(ty * SHAPE_WIDTH) + tx] & 1);
			
			if (isTile)
			{
				tlX = MIN(tx, tlX);
				tlY = MIN(ty, tlY);
				
				brX = MAX(tx, brX);
				brY = MAX(ty, brY);
			}
		}
	}
	
	topLeftX = (tlX + x_) + offX_;
	topLeftY = (tlY + y_) + offY_;
	
	// I have no idea why but for some reason we need to add one to get an accurate bounding box.
	// You would think this needn't be necessary since we are using MIN and MAX above.
	botRightX = (brX + x_) + offX_ + 1;
	botRightY = (brY + y_) + offY_ + 1;
}

unsigned char Shape::GetShapeTile(int x, int y) const
{
	return shapeInfo_ ? shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] : 0;
}

const ShapeInfo* Shape::GetInfo(void) const
{
	return shapeInfo_;
}
