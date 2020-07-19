
#include "Gridsubsystem.h"
#include "Window.h"
#include "Widget.h"
#include <cstdio>
#include "Canvas.h"

Block& Gridsubsystem::GetBlock(const Vect2 Pos)
{
	int Index = ( ((int)(Pos.Y / 64)) * GridSize.X) + (int)(Pos.X / 64);
	return Grid[Index];
}

unsigned int Gridsubsystem::GetBlockIndex(const Vect2 Pos)
{
	return ( ((int)(Pos.Y / 64)) * GridSize.X) + (int)(Pos.X / 64);
}

Widget* Gridsubsystem::GetWidget(const Vect2 Pos)
{
	Block& B = GetBlock(Pos + OwnerCanvas->Offset);
	return B.GetWidget(Pos + OwnerCanvas->Offset);
}

Block& Gridsubsystem::InsertWidget(Widget& W)
{
	//We'll need to get all the blocks covered and insert the widget
	bUpdate = true;
	//Get the index for the first block
	int Index = GetBlockIndex(W.Position);

	//Figure out how many blocks we cover.
	//This is done by dividing the vector between the top left of the first block and the end of the widget by 64 and rounding up the remainder
	Vect2 Len = W.Position.GetBlockDelta() + W.DrawSize;

	//This will give us a scalar value of how many blocks this vector spans over
	Vect2 Num = Len / 64.f;
	//Round it up so we have something useful to go by when iterating
	Num.RoundUp();	//Round up so that we know how many we've actually got including the ones partially crossed
		
	unsigned int XNum = Num.X;
	unsigned int YNum = Num.Y;

	//Add the widget to all the relevant blocks by iterating over the grid in 2D
	for(unsigned int Y = 0; Y < YNum; ++Y)		//Step vertically
	{	
		for (unsigned int X = 0; X < XNum; ++X)	//Step horizontally
		{
			//The vertical move needs to be multiplied by the total width of the grid to get to the next row. Index offsets the blocks
			Grid[(Y * (unsigned int)GridSize.X) + X + Index].AddWidget(W); 
		}
		
	}

	//I haven't found any use for this yet
	return Grid[Index];
}

Block& Gridsubsystem::RemoveWidget(Widget& W)
{
	int Index = GetBlockIndex(W.Position);
	Vect2 Len = W.Position.GetBlockDelta() + W.DrawSize;
	Vect2 Num = Len / Vect2(64.f, 64.f);
	Num.RoundUp();
	unsigned int XNum = Num.X;
	unsigned int YNum = Num.Y;
	for(unsigned int Y = 0; Y < YNum; ++Y)		//Step vertically
	{	
		for(unsigned int X = 0; X < XNum; ++X)	//Step horizontally
			Grid[(Y * (unsigned int)GridSize.X) + X + Index].RemoveWidget(W);
	}

	//I haven't found any use for this yet
	return Grid[Index];
}

void Gridsubsystem::MoveWidget(Widget& W, const Vect2 Delta)
{
	//Did the widget leave any blocks?
	//TODO: Optimize this so we don't remove from blocks that does not need to be removed from
	//Optimization1: Only move if we end up on a different block
	if (W.Position.GetBlock() != (W.Position + Delta).GetBlock())
	{
		RemoveWidget(W);
		W.Position += Delta;
		InsertWidget(W);
	}
	else
		W.Position += Delta;
}

//This is responsible for redrawing everything within a scissor box in the correct order
//According to ZIndex. The calling Widget will be drawn ontop of the other widgets
void Gridsubsystem::UpdateSegment(class Widget& W, const Vect2 Pos, const Vect2 Size)
{
	//Create first level scissor box
	CurrentState = RenderState::STATE_Segmented;
	ScissorBox(Pos, Size);

	//Iterate over the blocks
	/*int Index = GetBlockIndex(Pos);
	Vect2 Len = Pos.GetBlockDelta() + Size;
	Vect2 Num = Len / Vect2(64.f, 64.f);
	Num.RoundUp();
	unsigned int XNum = Num.X;
	unsigned int YNum = Num.Y;
	unsigned int WCount = 0;
	for(unsigned int Y = 0; Y < YNum; ++Y)		//Step vertically
	{	
		for (unsigned int X = 0; X < XNum; ++X)	//Step horizontally
		{
			//Step through the widgets in this container
			int BNum = Grid[(Y * (unsigned int)GridSize.X) + X + Index].Num;
			Block& B = Grid[(Y * (unsigned int)GridSize.X) + X + Index];
			for(int i = 0; i < BNum; ++i)
			{
				if (!B.Widgets[i])
					break;
				//Make sure the widget is within the frame
				Widget& Wd = *B.Widgets[i];
				if ( &Wd != &W && RectOverlap(Wd.Position, Wd.Size + Wd.Position, Pos, Pos + Size))
					Wd.bUpdate = true;
			}
		}
	}*/
	//Redraw segments
	//Bug: This will draw things that won't show up but still disables its update. We don't want that!
	OwnerCanvas->bUpdate = true;
	OwnerCanvas->SegmentRender(Pos, Size);
	DisableScissor();
	CurrentState = RenderState::STATE_Normal;
}

void Gridsubsystem::ResizeWidget(Widget& W, const Vect2& Delta)
{
	//Optimize this later
	//if( (W.Position + W.DrawSize).GetBlock() != (W.Position + NewSize).GetBlock() )
	//{
		//Figure out which blocks are affected
	//	Vect2 Delta = NewSize - W.DrawSize;

	//}

	if ( (W.Position + W.DrawSize).GetBlock() != (W.Position + W.DrawSize + Delta).GetBlock())
	{
		RemoveWidget(W);
		W.DrawSize += Delta;
		InsertWidget(W);
	}
	else
		W.DrawSize += Delta;
}

Widget* Block::GetWidget(const Vect2 Pos)
{
	Widget* Result = nullptr;
	for (unsigned char i = 0; i < Num; ++i)
	{
		//Vect2& Loc = Widgets[i]->Position;
		//Vect2 End = Loc + (Widgets[i]->Size);
		//Simple rectangular collision primitive
		//if (Pos >= Loc && Pos <= End)
		if (Widgets[i] && Widgets[i]->ZIndex > 0 && (!Result || Result->ZIndex < Widgets[i]->ZIndex) && Widgets[i]->TestCollision(Pos))
			Result = Widgets[i];//return Widgets[i];
	}
	return Result;
}

bool Block::AddWidget(Widget& W)
{
	//Check if there is a free block
	for (unsigned char i = 0; i < Num; ++i)
	{
		if (!Widgets[i])
		{
			Widgets[i] = &W;
			return true;
		}
	}
	if (Num == 255)
		return false;

	Widgets[Num++] = &W;
	return true;
}

void Block::RemoveWidget(Widget& W)
{
	for(unsigned char i = 0; i < Num; ++i)
	{
		if (Widgets[i] == &W)
			Widgets[i] = 0;
	}
}
