#pragma once


#include "Template.h"

//Represents a single grid block
class Block
{
public:

	class Widget* Widgets[128];
	unsigned char Num;

	Widget* GetWidget(const Vect2 Pos);
	bool AddWidget(class Widget&);
	void RemoveWidget(class Widget&);
};


//Grids are always 64x64
//A better idea: Our grid array is always a fixed size anyway. Therefore, we can simply treat the grid as a fixed size grid overall too. Meaning we don't have to rearrange them when the window is resized
//By separating the grid and the window size this way we can save a lot of processing power and complexity. It should not affect the cache either so that's all good.
//The grid will therefore be a constant size, and the window size will simply determine how much of that grid is "visible"
class Gridsubsystem
{
public:

	//These are the number of horizontal and vertical blocks
	const Vect2 GridSize = { 120, 68 };

	//Get block -> Very fast
	Block& GetBlock(const Vect2 Pos);
	unsigned int GetBlockIndex(const Vect2 Pos);

	Widget* GetWidget(const Vect2 Pos);

	Block& InsertWidget(class Widget&);
	Block& RemoveWidget(class Widget&);

	void MoveWidget(class Widget&, const Vect2 Pos);

	void UpdateSegment(class Widget& W, const Vect2 Pos, const Vect2 Size);
	void ResizeWidget(class Widget& W, const Vect2& Delta);

	//Grid array for up to 8k resolution
	class Block Grid[8160];
	
};

