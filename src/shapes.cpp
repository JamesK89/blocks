#include <blocks.hpp>
#include <shapes.hpp>

Shape::Shape(Application* app, const ShapeInfo* info) :
	app_(app), shapeInfo_(info), x_(0), y_(0), offX_(0), offY_(0), orientation_(0), tile_(TILE_BLOCK_RED)
{	
	CalculateOffset();
	tile_ = info->tile;
}

Shape::~Shape(void)
{
	shapeInfo_ = nullptr;
}

void Shape::Draw(void) const
{
	for (int y = 0; y < SHAPE_HEIGHT; y++)
	{
		for (int x = 0; x < SHAPE_WIDTH; x++)
		{
			bool isTile = (shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] & 1);
			
			if (isTile)
			{
				int realX = (offX_ + x_) + x;
				int realY = (offY_ + y_) + y;
			
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
	for (int y = 0; y < SHAPE_HEIGHT; y++)
	{
		for (int x = 0; x < SHAPE_WIDTH; x++)
		{
			bool isPivot = (shapeInfo_->data[orientation_][(y * SHAPE_WIDTH) + x] & 2);
			
			if (isPivot)
			{
				offX_ = x * -1;
				offY_ = y * -1;
				
				break;
			}
		}
	}
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