// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"

#include "GUIPanel.h"
#include "GUIStyleSheet.h"

#include "../../shared.h"

vc::GUIPanel::GUIPanel( vc::GUIPanel *parent, int x, int y, int w, int h, vc::GUIPanel::Background background, vc::GUIPanel::Border border )
	: background_( background ), border_( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h )
{
	if ( parent == nullptr )
	{
		return;
	}

	// Push it onto the list of children
	parent->children_.push_back( this );

	// Style should be the same as the parent
	styleSheet_ = parent->styleSheet_;

	this->x += parent->x;
	this->y += parent->y;
}

vc::GUIPanel::~GUIPanel() = default;

void vc::GUIPanel::Draw()
{
	if ( !isDrawing )
	{
		return;
	}

	DrawBackground();
	DrawBorder();

	// Draw all of the children
	for ( auto i : children_ )
	{
		i->Draw();
	}
}

void vc::GUIPanel::DrawBackground()
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

void vc::GUIPanel::DrawBorder()
{
	if ( styleSheet_ == nullptr )
	{
		return;
	}

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

	DrawBorderEdge( x, y, w, 0, u );                                             // top
	DrawBorderEdge( x, y + h - styleSheet_->frameSprites[ d ]->height, w, 0, d );// bottom
	DrawBorderEdge( x, y, 0, h, l );// left
	DrawBorderEdge( x + w - styleSheet_->frameSprites[ r ]->width, y, 0, h, r );// right

	DrawBorderCorner( x, y, ul );
	DrawBorderCorner( x + w - styleSheet_->frameSprites[ ur ]->width, y, ur );
	DrawBorderCorner( x, y + h - styleSheet_->frameSprites[ ll ]->height, ll );
	DrawBorderCorner( x + w - styleSheet_->frameSprites[ lr ]->width, y + h - styleSheet_->frameSprites[ lr ]->height, lr );
}

void vc::GUIPanel::DrawBorderCorner( int dx, int dy, unsigned int index )
{
	const Sprite *sprite = styleSheet_->frameSprites[ index ];
	sprite->Draw( dx, dy, false );
}

void vc::GUIPanel::DrawBorderEdge( int dx, int dy, int dw, int dh, unsigned int index )
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
			{
				break;
			}
		}
		else
		{
			dy += sprite->height;
			if ( ( dy - oy ) >= dh )
			{
				break;
			}
		}
	}
}

void vc::GUIPanel::Tick()
{
	isDrawing = ShouldDraw();

	// Tick all of the children
	for ( auto i : children_ )
	{
		i->Tick();
	}
}

void vc::GUIPanel::SetStyleSheet( GUIStyleSheet *styleSheet )
{
	styleSheet_ = styleSheet;
}

void vc::GUIPanel::GetContentPosition( int *xd, int *yd ) const
{
	if ( border_ == Border::NONE )
	{
		GetPosition( xd, yd );
		return;
	}

	// Assume border is 2 pixels (idiotic)
	*xd = x + 2;
	*yd = y + 2;
}

void vc::GUIPanel::GetContentSize( int *wd, int *hd ) const
{
	if ( border_ == Border::NONE )
	{
		GetSize( wd, hd );
		return;
	}

	// Assume border is 2 pixels (idiotic)
	*wd = w - 2;
	*hd = h - 2;
}

bool vc::GUIPanel::IsMouseOver( int mx, int my ) const
{
	return !( mx < x || mx > x + w || my < y || my > y + h );
}

bool vc::GUIPanel::IsMouseOver() const
{
	int mx, my;
	GetApp()->GetCursorPosition( &mx, &my );
	return IsMouseOver( mx, my );
}

bool vc::GUIPanel::HandleMouseEvent( int mx, int my, int wheel, int button, bool buttonUp )
{
	if ( !IsMouseOver( mx, my ) )
	{
		return false;
	}

	for ( auto i : children_ )
	{
		// If the child handles the event, return true
		if ( i->HandleMouseEvent( mx, my, wheel, button, buttonUp ) )
		{
			return true;
		}
	}

	// If the user is clicking, their mouse is over, but we're not handling it, return true(?)
	//if ( !buttonUp && ( button == MOUSE_BUTTON_LEFT ) || ( button == MOUSE_BUTTON_MIDDLE ) || ( button == MOUSE_BUTTON_RIGHT ) ) {
	//	return true;
	//}

	return false;
}

bool vc::GUIPanel::HandleKeyboardEvent( int button, bool buttonUp )
{
	for ( auto i : children_ )
	{
		// If the child handles the event, return true
		if ( i->HandleKeyboardEvent( button, buttonUp ) )
		{
			return true;
		}
	}

	return false;
}
