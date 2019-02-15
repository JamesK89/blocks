#pragma once
#ifndef _INCLUDE_SHAPES_H_
#define _INCLUDE_SHAPES_H_

const unsigned char SHAPE_WIDTH = 5;
const unsigned char SHAPE_HEIGHT = 5;
const unsigned char SHAPE_SIZE = (SHAPE_WIDTH * SHAPE_HEIGHT);

const unsigned int SHAPE_T_SIZE = 4;

const unsigned int SHAPE_COUNT = 7;

const unsigned char SHAPE_T[SHAPE_T_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 1, 3, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 3, 1, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 3, 1, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 1, 3, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_J_SIZE = 4;

const unsigned char SHAPE_J[SHAPE_J_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 3, 0, 0,
		0, 1, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 1, 3, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 1, 0,
		0, 0, 3, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 3, 1, 0,
		0, 0, 0, 1, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_L_SIZE = 4;

const unsigned char SHAPE_L[SHAPE_L_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 3, 0, 0,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 3, 1, 0,
		0, 1, 0, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 1, 1, 0, 0,
		0, 0, 3, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 1, 0,
		0, 1, 3, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_Z_SIZE = 2;

const unsigned char SHAPE_Z[SHAPE_Z_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 3, 0, 0,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 1, 0,
		0, 0, 3, 1, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_S_SIZE = 2;

const unsigned char SHAPE_S[SHAPE_S_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 3, 1, 0,
		0, 1, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 1, 3, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_O_SIZE = 1;

const unsigned char SHAPE_O[SHAPE_O_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 3, 0, 0,
		0, 1, 1, 0, 0,
		0, 0, 0, 0, 0
	}
};

const unsigned int SHAPE_I_SIZE = 2;

const unsigned char SHAPE_I[SHAPE_I_SIZE][SHAPE_SIZE] = {
	{
		0, 0, 1, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 3, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		1, 1, 3, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	}
};

typedef struct 
{
	const char type;
	const unsigned char num;
	const unsigned char tile;
	const unsigned char (*data)[SHAPE_SIZE];
} ShapeInfo;

const ShapeInfo SHAPES[] = {
	{'J', SHAPE_J_SIZE, TILE_BLOCK_RED, &SHAPE_J[0]},
	{'L', SHAPE_L_SIZE, TILE_BLOCK_BLU, &SHAPE_L[0]},
	{'S', SHAPE_S_SIZE, TILE_BLOCK_GRN, &SHAPE_S[0]},
	{'Z', SHAPE_Z_SIZE, TILE_BLOCK_ORG, &SHAPE_Z[0]},
	{'T', SHAPE_T_SIZE, TILE_BLOCK_PUR, &SHAPE_T[0]},
	{'O', SHAPE_O_SIZE, TILE_BLOCK_YEL, &SHAPE_O[0]},
	{'I', SHAPE_I_SIZE, TILE_BLOCK_CYN, &SHAPE_I[0]}
};

class Shape
{
public:
	Shape(Application* app);
	Shape(Application* app, const ShapeInfo* info);
	Shape(Application* app, const Shape* shape);
	~Shape(void);
	
	void Set(const ShapeInfo* info);
	void Set(const Shape* shape);
	
	void Draw(bool withOffset = true) const;
	
	unsigned char GetNumOrientations(void) const;
	
	unsigned char GetOrientation(void) const;
	void SetOrientation(unsigned char n);
	
	void SetPosition(int x, int y);
	void GetPosition(int& x, int& y) const;
	
	int GetTile(void) const;
	void SetTile(int tile);
	
	void GetBounds(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY) const;
	void GetInnerBounds(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY) const;
	
	void GetOffset(int& offX, int& offY) const;
	
	unsigned char GetShapeTile(int x, int y) const;
	
	const ShapeInfo* GetInfo(void) const;
	
protected:
	Application* app_;
	
	const ShapeInfo* shapeInfo_;
	
	int x_;
	int y_;
	
	int offX_;
	int offY_;
	
	int ibbOffX_;
	int ibbOffY_;
	
	int tile_;
	
	unsigned char orientation_;
	
	void CalculateOffset(void);
};

#endif
