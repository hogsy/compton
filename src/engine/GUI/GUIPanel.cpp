// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <allegro5/allegro.h>

#include "Compton.h"
#include "GUIPanel.h"

#include "../../shared.h"

vc::GUIPanel::GUIPanel( vc::GUIPanel *parent, int x, int y, int w, int h, vc::GUIPanel::Background background, vc::GUIPanel::Border border )
	: myBackground( background ), border_( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h )
{
	if ( parent == nullptr )
	{
		return;
	}

	// Push it onto the list of children
	parent->children.push_back( this );

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
	for ( auto i : children )
	{
		i->Draw();
	}
}

void vc::GUIPanel::DrawBackground()
{
	if ( myBackground == Background::NONE )
	{
		return;
	}

	int dx, dy, dw, dh;
	GetContentPosition( &dx, &dy );
	GetContentSize( &dw, &dh );

	switch ( myBackground )
	{
		default: break;
		case Background::SOLID:
			DrawFilledRectangle( dx, dy, dw, dh, backgroundColour );
			break;
		case Background::TEXTURE:
		{
			if ( styleSheet_ == nullptr )
			{
				return;
			}

#if 0
			ALLEGRO_BITMAP *bmp = styleSheet_->GetBitmap();
			if ( bmp == nullptr )
			{
				return;
			}

			float sx, sy, sw, sh;
			sx = styleSheet_->backgrounds[ 0 ].x;
			sy = styleSheet_->backgrounds[ 0 ].y;
			sw = styleSheet_->backgrounds[ 0 ].w;
			sh = styleSheet_->backgrounds[ 0 ].h;

			al_draw_tinted_scaled_rotated_bitmap_region(
					bmp,
					sx, sy,
					sw, sh,
					al_map_rgba(
							backgroundColour.r,
							backgroundColour.g,
							backgroundColour.b,
							backgroundColour.a ),
					0.0f, 0.0f,
					dx, dy, dw / sw, dh / sh,
					0.0f,
					0 );
#endif
			break;
		}
	}
}

void vc::GUIPanel::DrawBorder()
{
#if 0
	const GUIStyleSheet::GUIBorderStyle *borderStyle;
	switch ( border_ )
	{
		default:
			return;
		case Border::INSET:
			borderStyle = &styleSheet_->inset;
			break;
		case Border::OUTSET:
			borderStyle = &styleSheet_->outset;
			break;
	}

	// Top
	DrawBorderEdge( x, y, w, borderStyle->u.h, borderStyle->u );
	// Bottom
	DrawBorderEdge( x, y + h - borderStyle->d.h, w, borderStyle->d.h, borderStyle->d );
	// Left
	DrawBorderEdge( x, y, borderStyle->l.w, h, borderStyle->l );
	// Right
	DrawBorderEdge( x + w - borderStyle->r.w, y, borderStyle->r.w, h, borderStyle->r );

	DrawBorderCorner( x, y, borderStyle->lu );
	DrawBorderCorner( x + w - borderStyle->ru.w, y, borderStyle->ru );
	DrawBorderCorner( x, y + h - borderStyle->ll.h, borderStyle->ll );
	DrawBorderCorner( x + w - borderStyle->lr.w, y + h - borderStyle->lr.h, borderStyle->lr );
#endif
}

void vc::GUIPanel::DrawBorderCorner( int dx, int dy )
{
	if ( styleSheet_ == nullptr )
	{
		return;
	}


}

void vc::GUIPanel::DrawBorderEdge( int dx, int dy, int dw, int dh )
{
#if 0
	ALLEGRO_BITMAP *bmp = styleSheet_->GetBitmap();
	if ( bmp == nullptr )
	{
		return;
	}

	al_draw_tinted_scaled_rotated_bitmap_region(
			bmp,
			tileCoord.x, tileCoord.y,
			tileCoord.w, tileCoord.h,
			al_map_rgba(
					backgroundColour.r,
					backgroundColour.g,
					backgroundColour.b,
					backgroundColour.a ),
			0.0f, 0.0f,
			dx, dy, dw / tileCoord.w, dh / tileCoord.h,
			0.0f,
			0 );
#endif
}

void vc::GUIPanel::Tick()
{
	isDrawing = ShouldDraw();

	// Tick all of the children
	for ( auto i : children )
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

	for ( auto i : children )
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
	for ( auto i : children )
	{
		// If the child handles the event, return true
		if ( i->HandleKeyboardEvent( button, buttonUp ) )
		{
			return true;
		}
	}

	return false;
}
