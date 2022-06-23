// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Input/InputManager.h"

#include "GUIPanel.h"
#include "GUIStyleSheet.h"
#include "GUILabel.h"

const hei::Colour ct::GUIPanel::INSET_COLOUR = { 122, 122, 122, 255 };
const hei::Colour ct::GUIPanel::OUTSET_COLOUR = { 192, 192, 192, 255 };

ct::GUIPanel::GUIPanel( ct::GUIPanel *parent, int x, int y, int w, int h, ct::GUIPanel::Background background, ct::GUIPanel::Border border )
	: background_( background ), border_( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h )
{
	if ( parent == nullptr )
		return;

	// Push it onto the list of children
	parent->children_.push_back( this );

	// Style should be the same as the parent
	styleSheet_ = parent->styleSheet_;

	//this->x += parent->x;
	//this->y += parent->y;
}

ct::GUIPanel::~GUIPanel() = default;

void ct::GUIPanel::Draw()
{
	if ( !isDrawing )
		return;

	DrawBackground();
	DrawBorder();

	// Draw all of the children
	for ( auto i : children_ )
	{
		// Draw the tooltip last
		if ( i == tooltip_ )
			continue;

		i->Draw();
	}

	if ( tooltip_ != nullptr )
		tooltip_->Draw();
}

void ct::GUIPanel::DrawBackground()
{
	hei::Colour colour;
	switch ( background_ )
	{
		case Background::NONE:
			return;
		case Background::DEFAULT:
		{
			if ( border_ == Border::INSET )
			{
				colour = INSET_COLOUR;
				break;
			}

			colour = OUTSET_COLOUR;
			break;
		}
		case Background::SOLID:
			colour = backgroundColour_;
			break;
	}

	int dx, dy, dw, dh;
	GetContentPosition( &dx, &dy );
	GetContentSize( &dw, &dh );

	render::DrawFilledRectangle( dx, dy, dw, dh, colour );
}

void ct::GUIPanel::DrawBorder()
{
	if ( styleSheet_ == nullptr )
		return;

	unsigned int ul, ur, ll, lr;
	unsigned int u, r, d, l;
	switch ( border_ )
	{
		case Border::INSET:
			ul = GUIStyleSheet::GUI_FRAME_IUL;
			ur = GUIStyleSheet::GUI_FRAME_IUR;
			ll = GUIStyleSheet::GUI_FRAME_ILL;
			lr = GUIStyleSheet::GUI_FRAME_ILR;
			u  = GUIStyleSheet::GUI_FRAME_IU;
			r  = GUIStyleSheet::GUI_FRAME_IR;
			d  = GUIStyleSheet::GUI_FRAME_ID;
			l  = GUIStyleSheet::GUI_FRAME_IL;
			break;
		case Border::OUTSET:
			ul = GUIStyleSheet::GUI_FRAME_UL;
			ur = GUIStyleSheet::GUI_FRAME_UR;
			ll = GUIStyleSheet::GUI_FRAME_LL;
			lr = GUIStyleSheet::GUI_FRAME_LR;
			u  = GUIStyleSheet::GUI_FRAME_U;
			r  = GUIStyleSheet::GUI_FRAME_R;
			d  = GUIStyleSheet::GUI_FRAME_D;
			l  = GUIStyleSheet::GUI_FRAME_L;
			break;
		case Border::NONE:
			return;
	}
	
	int dx, dy;
	GetPosition( &dx, &dy );

	DrawBorderEdge( dx, dy, w, 0, u );                                             // top
	DrawBorderEdge( dx, dy + h - styleSheet_->frameSprites[ d ]->height, w, 0, d );// bottom
	DrawBorderEdge( dx, dy, 0, h, l );// left
	DrawBorderEdge( dx + w - styleSheet_->frameSprites[ r ]->width, dy, 0, h, r );// right

	DrawBorderCorner( dx, dy, ul );
	DrawBorderCorner( dx + w - styleSheet_->frameSprites[ ur ]->width, dy, ur );
	DrawBorderCorner( dx, dy + h - styleSheet_->frameSprites[ ll ]->height, ll );
	DrawBorderCorner( dx + w - styleSheet_->frameSprites[ lr ]->width, dy + h - styleSheet_->frameSprites[ lr ]->height, lr );
}

void ct::GUIPanel::DrawBorderCorner( int dx, int dy, unsigned int index )
{
	const Sprite *sprite = styleSheet_->frameSprites[ index ];
	sprite->Draw( dx, dy, false );
}

void ct::GUIPanel::DrawBorderEdge( int dx, int dy, int dw, int dh, unsigned int index )
{
	const Sprite *sprite = styleSheet_->frameSprites[ index ];

	// God this is awful...

	int ox = dx;
	int oy = dy;
	while ( true )
	{
		sprite->Draw( dx, dy, false );
		if ( dw != 0 )
		{
			dx += sprite->width;
			if ( ( dx - ox ) >= dw )
				break;
		}
		else
		{
			dy += sprite->height;
			if ( ( dy - oy ) >= dh )
				break;
		}
	}
}

void ct::GUIPanel::Tick()
{
	isDrawing = ShouldDraw();

	// Tick all of the children
	for ( auto i : children_ )
		i->Tick();

	if ( tooltip_ != nullptr )
	{
		if ( IsMouseOver() )
		{
			tooltipHoverTime_++;
			if ( tooltipHoverTime_ >= TOOLTIP_MAX_HOVER )
			{
				tooltip_->Show();

				int mx, my;
				input::inputManager->GetMousePosition( &mx, &my );
				tooltip_->x = mx - tooltip_->w;
				tooltip_->y = my - tooltip_->h;
			}
		}
		else
		{
			tooltip_->Hide();
			tooltipHoverTime_ = 0;
		}
	}
}

void ct::GUIPanel::SetStyleSheet( GUIStyleSheet *styleSheet )
{
	styleSheet_ = styleSheet;
}

void ct::GUIPanel::GetContentPosition( int *xd, int *yd ) const
{
	int px = 0, py = 0;
	if ( parentPtr != nullptr )
		parentPtr->GetContentPosition( &px, &py );

	if ( border_ == Border::NONE )
	{
		GetPosition( xd, yd );
		xd += px;
		yd += py;
		return;
	}

	*xd = px + x + GUIStyleSheet::GUI_BORDER_SIZE;
	*yd = py + y + GUIStyleSheet::GUI_BORDER_SIZE;
}

void ct::GUIPanel::GetContentSize( int *wd, int *hd ) const
{
	if ( border_ == Border::NONE )
	{
		GetSize( wd, hd );
		return;
	}

	*wd = w - GUIStyleSheet::GUI_BORDER_SIZE;
	*hd = h - GUIStyleSheet::GUI_BORDER_SIZE;
}

bool ct::GUIPanel::IsMouseOver( int mx, int my ) const
{
	return !( mx < x || mx > x + w || my < y || my > y + h );
}

bool ct::GUIPanel::IsMouseOver() const
{
	int mx, my;
	input::inputManager->GetMousePosition( &mx, &my );
	return IsMouseOver( mx, my );
}

bool ct::GUIPanel::HandleMouseEvent( int mx, int my, int wheel, int button, bool buttonUp )
{
	if ( !IsMouseOver( mx, my ) )
		return false;

	for ( auto i : children_ )
	{
		// If the child handles the event, return true
		if ( i->HandleMouseEvent( mx, my, wheel, button, buttonUp ) )
			return true;
	}

	// If the user is clicking, their mouse is over, but we're not handling it, return true(?)
	//if ( !buttonUp && ( button == MOUSE_BUTTON_LEFT ) || ( button == MOUSE_BUTTON_MIDDLE ) || ( button == MOUSE_BUTTON_RIGHT ) ) {
	//	return true;
	//}

	return false;
}

bool ct::GUIPanel::HandleKeyboardEvent( int button, bool buttonUp )
{
	for ( auto i : children_ )
	{
		// If the child handles the event, return true
		if ( i->HandleKeyboardEvent( button, buttonUp ) )
			return true;
	}

	return false;
}

void ct::GUIPanel::SetTooltip( const char *description )
{
	assert( tooltip_ == nullptr );
	tooltip_ = new GUIPanel( this, 0, 0, 100, 16, Background::SOLID, Border::OUTSET );
	new GUILabel( tooltip_, description );
}
