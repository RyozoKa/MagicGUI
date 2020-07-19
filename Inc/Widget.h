#pragma once

#include "Texture.h"
#include <vector>
#include "CallbackChain.h"

/*
	Base class for all elements drawn in a window.
	A widget primitive will always be a square or a rectangle by default
	 
*/


//Keypoints are absolute positions
struct Keypoint
{
	Vect2 Pos;
	Rotator Rot;
	float Duration;
};

enum ECollision
{
	ECollision_Rect,
	ECollision_Radi
};

enum ExpandMode
{
	Ex_Expanded = 0,
	Ex_Collapsed

};

class MAGICGUIAPI Widget
{
public:

	virtual void Tick(double);

	Vect2 Position = { -1.f, -1.f };
	Vect2 OriginalPos;
	Vect2 Size;
	Vect2 DrawSize;
	Rotator Rotation;

	//Internal
	Mat2 RotMatrix;
	ECollision CollisionType = ECollision_Rect;
	bool bUpdate;

	Keypoint Keys[16];
	unsigned char CurrentKey = 1;
	unsigned char NumKeys = 1;

	//Callback events
	Callbacks<Widget*> OnFinishedAnim;
	Callbacks<Widget*> OnFinishedAnimReturn;
	Callbacks<Widget*> OnResize;				//-- For draw segmentation or just a general resize
	Callbacks<Widget*> OnVerticalExpanded;
	Callbacks<Widget*> OnVerticalCollapsed;
	Callbacks<Widget*> OnHorizontalExpanded;
	Callbacks<Widget*> OnHorizontalCollapsed;

	//Radius collision data
	float Radius = 0.f;
	std::vector<Widget*> Items;			//-- Child widgets inside this widget
	class Window* Window;				//-- Window owner
	Widget* Owner;						//-- Parent widget	

	//Absolute Z index, reassembles the order in which widgets were added
	unsigned int ZIndex;

	//Tab index
	unsigned int TabIndex;

	//True if this widget contains any transparency
	bool bTransparent = false;
	bool bEnabled = true;				//-- Enable or disable widget
	bool bHidden = false;				//-- Visibility
	bool bCollide = true;
	//Animation internals
	bool bAnimating = false;			//-- Move object interpolated according to keypoints
	bool bReturnAnim = false;			//-- Return object to starting position
	bool bExpandVert = false;			//-- Expand vertical draw (Drawing parts of the object)
	bool bCollapseVert = false;			//-- Collapse vertical draw
	bool bExpandHor = false;			//-- Expand horizontal draw
	bool bCollapseHor = false;			//-- Collapse horizontal draw

	//Properties that must be set
	float VerticalDuration = 0.f;		//-- Vertical expansion duration
	float HorizontalDuration = 0.f;		//-- Horizontal expansion duration
	float HorMin = 0.f;					//-- Horizontal minimal percentage
	float VertMin = 0.f;				//-- Vertical minimal percentage

	//ExpandMode ExMode = Ex_Expanded;	//-- Draw method, either begin as fully expanded (default), or collapsed

	void AddKeypoint(Keypoint Point);


	//Collision detection against this Widget
	virtual bool TestCollision(const Vect2&);

	//Control functions for animations
	virtual void Animate()
	{
		bUpdate = true;
		bAnimating = true; 
		bReturnAnim = false;
	}
	virtual void PauseAnim()
	{
		bUpdate = false;
		bAnimating = false; 
		bReturnAnim = false;
	}
	virtual void ReturnAnim()
	{
		bUpdate = true;
		bAnimating = false; 
		bReturnAnim = true;
	}

	void SetInitialSize(Vect2 Size)
	{
		DrawSize = Size;
	}
	virtual void VerticalExpand()
	{
		bExpandVert = true;
		bCollapseVert = false;
	}
	virtual void VerticalCollapse()
	{
		bExpandVert = false;
		bCollapseVert = true;
	}
	virtual void HorizontalExpand()
	{
		bExpandHor = true;
		bCollapseHor = false;
	}
	virtual void HorizontalCollapse()
	{
		bExpandHor = false;
		bCollapseHor = true;
	}

	//Registry interface
	virtual void AddItem(Widget*);

	//Setters
	virtual void SetSize(const Vect2 Sz);
	virtual void SetRotation(const Rotator Rot);
	virtual void SetRadius(const float F);
	virtual void SetPosition(const Vect2 Pos);		//-- !!!OBS SetPosition does not automatically update the collision grid
													//-- If you want to move a few children inside a Canvas, you manually have to call GridSystem->MoveWidget
	virtual void Move(const Vect2 Offset);
	//Etc
	virtual void Draw();
	virtual void RenderObjects() = 0;	//-- Override this and set the objects you wish to draw in a custom widget.
	virtual void Update();				//-- Set to redraw in the next tick
	virtual void Show();
	virtual void Hide();
	virtual void SegmentRender(Vect2 Pos, Vect2 Size);

	//Input Events
	virtual void OnMouseEnter(float X, float Y);
	virtual void OnMouseLeave(float X, float Y);
	virtual void OnMouseLeftClick(float X, float Y);
	virtual void OnMouseRightClick(float X, float Y);
	virtual void OnMouseLeftReleased(float X, float Y);
	virtual void OnMouseRightReleased(float X, float Y);
	virtual void OnKeyPressed(int Key, int Mod);
	virtual void OnScroll(float YOffset);
	virtual void OnCursor(double X, double Y);

	//Other events
	virtual void Attached();		//--	Called when this widget is attached to another widget

	class Gridsubsystem* GridSystem;	//--	Grid to which this widget belongs. This could be the main window, or a scroll pane

};