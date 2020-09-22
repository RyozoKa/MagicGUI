/*
	Main canvas widget.
	This widget covers the entire window and acts as the canvas on which other widgets can paint.

	Revision:
		Christopher Békési 2020-04-20

*/

#pragma once
#include "Widget.h"
#include "Texture.h"
#include "Gridsubsystem.h"
#include "RenderBuffer.h"
#include "Image.h"
#include "CallbackChain.h"

class MAGICGUIAPI Canvas : public Widget
{
public:

	//RenderObject Background;
	Image BackgroundImage;					//-- New background implementation, done with an independent widget to support Segmented rendering
	Vect2 Offset = Vect2(0.f, 0.f);			//-- Collision grid offset. Only used when applying scroll offsets to avoid moving the grid.
	Vect2 CalculatedSize;
	bool bPressed = false;
	//I/O Related internals
	Gridsubsystem LocalGridSystem;
	Vect2 CursorPos;
	Widget* LastHit = nullptr;
	Widget* Focused = nullptr;
	Canvas* ParentCanvas = nullptr;					//-- Support Canvas animations.
	Vect2 ItemCanvas;
	RenderBuffer Buffer;		//-- Actual framebuffer
	RenderObject Frame;			//-- Frame on which this buffer will be drawn

	Callbacks<Canvas*> OnCanvasResize;

	//Canvas interface
	void SetColor(const Color);		//-- Background color
	void SetImage(Texture*);		//-- Background image
	virtual void SetMode(TYPE);	

	virtual void RenderObjects();

	Canvas() : LocalGridSystem(this) , Buffer(&Frame)
	{
		//Background.Owner = this;
		Frame.Owner = this;
		Buffer.Owner = this;
		BackgroundImage.SetPosition(Vect2(0.f, 0.f));
		BackgroundImage.bCollide = false;
	}
	virtual void SetSize(const Vect2 Sz)
	{
		Widget::SetSize(Sz);
		//Background.SetPrimitive(Sz);

		/*if (DrawSize != Vect2(-1.f, -1.f) && DrawSize.GetBlock() != Sz.GetBlock() && Owner && GridSystem)
		{
			Owner->GridSystem->RemoveWidget(*this);
			Size = Sz;
			DrawSize = Sz;
			Owner->GridSystem->InsertWidget(*this);
		}
		else if (DrawSize != Vect2(-1.f, -1.f) && DrawSize.GetBlock() != Sz.GetBlock() && GridSystem)
		{
			GridSystem->RemoveWidget(*this);
			Size = Sz;
			DrawSize = Sz;
			GridSystem->InsertWidget(*this);
		}
		else
			DrawSize = Size = Sz;*/
	
		Update();	//Update all the children
		//bUpdate = true;
		
		Frame.SetPrimitive(Sz, Buffer.Size);
		if(!Frame.Image)
			Frame.SetImage(Buffer.GetTexture());
		if (Sz.X > CalculatedSize.X)
			CalculatedSize.X = Sz.X;
		if (Sz.Y > CalculatedSize.Y)
			CalculatedSize.Y = Sz.Y;
		if (Buffer.Size < CalculatedSize)
		{
			Buffer.SetSize(CalculatedSize);
			BackgroundImage.SetSize(CalculatedSize);

		}
		//Buffer.Size = Sz;
		
		
		//Frame.SetTextureClipping();
	}

	virtual void SetPosition(const Vect2 Pos)
	{
		Widget::SetPosition(Pos);
		/*if (Position != Vect2(-1.f, -1.f) && Position.GetBlock() != Pos.GetBlock() && Owner && Owner->GridSystem)
		{
			Owner->GridSystem->RemoveWidget(*this);
			Position = Pos;
			Owner->GridSystem->InsertWidget(*this);
		}
		else if (Position != Vect2(-1.f, -1.f) && Position.GetBlock() != Pos.GetBlock() && GridSystem)
		{
			GridSystem->RemoveWidget(*this);
			Position = Pos;
			GridSystem->InsertWidget(*this);
		}
		else
			Position = Pos;*/

		Keys[0].Pos = Pos;
		Keys[0].Rot.Rot = 0.f;
		Update();

		if(Owner && GridSystem)
			Buffer.AbsPosition = Owner->GridSystem->OwnerCanvas->Buffer.AbsPosition + Position;
	}
	//A Canvas will never collide

	bool ShouldDraw(Widget* W)
	{
		return (Position + Size) > W->Position;
	}

	virtual void Attached();

	virtual void AddItem(Widget*);

	virtual void OnCursor(double X, double Y);
	virtual void OnKeyPress(int Key, int Mod);
	virtual void OnKeyRelease(int Key, int Mod);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnScroll(float YOffset);
	//We need this in order to register leaves when a widget is by the edge.
	virtual void OnMouseLeave(float X, float Y);

	virtual void Draw();

	virtual void Show();

	void ScrollX(float XOffset);
	void ScrollY(float YOffset);

	virtual void Tick(double dt);
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);

	virtual void OnItemChanged(Vect2 Position, Vect2 Size);

	virtual void OnWindowResize(Vect2 Delta);

	Vect2 CalculateEndpoint();
};