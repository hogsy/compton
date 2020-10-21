/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "SimGame.h"
#include "GUIPanel.h"

#include "../../shared.h"

vc::GUIPanel::GUIPanel( vc::GUIPanel *parent, int x, int y, int w, int h, vc::GUIPanel::Background background, vc::GUIPanel::Border border )
    : myBackground( background ), myBorder( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h ) {
	if ( parent == nullptr ) {
		return;
	}

	// Push it onto the list of children
	parent->children.push_back( this );

	// Style should be the same as the parent
	myStyleSheet = parent->myStyleSheet;

	this->x += parent->x;
	this->y += parent->y;
}

vc::GUIPanel::~GUIPanel() = default;

void vc::GUIPanel::Draw() {
	if ( !shouldDraw ) {
		return;
	}

	DrawBackground();
	DrawBorder();

	// Draw all of the children
	for ( auto i : children ) {
		i->Draw();
	}
}

void vc::GUIPanel::DrawBackground() {
	if ( myBackground == Background::NONE ) {
		return;
	}

	int dx, dy, dw, dh;
	GetContentPosition( &dx, &dy );
	GetContentSize( &dw, &dh );

	switch( myBackground ) {
		default: break;
		case Background::SOLID:
			al_draw_filled_rectangle( dx, dy, dx + dw, dy + dh, al_map_rgba(
					backgroundColour.r,
					backgroundColour.g,
					backgroundColour.b,
					backgroundColour.a
			) );
			break;
		case Background::TEXTURE: {
			ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
			if ( bmp == nullptr ) {
				return;
			}

			float sx, sy, sw, sh;
			sx = myStyleSheet->backgrounds[ 0 ].x;
			sy = myStyleSheet->backgrounds[ 0 ].y;
			sw = myStyleSheet->backgrounds[ 0 ].w;
			sh = myStyleSheet->backgrounds[ 0 ].h;

			al_draw_tinted_scaled_rotated_bitmap_region(
					bmp,
					sx, sy,
					sw, sh,
					al_map_rgba(
			                backgroundColour.r,
			                backgroundColour.g,
			                backgroundColour.b,
			                backgroundColour.a
			                ),
					0.0f, 0.0f,
					dx, dy, dw / sw, dh / sh,
					0.0f,
			        0
			);
			break;
		}
	}
}

void vc::GUIPanel::DrawBorder() {
	const GUIStyleSheet::GUIBorderStyle *borderStyle;
	switch( myBorder ) {
		default:
			return;
		case Border::INSET:
			borderStyle = &myStyleSheet->inset;
			break;
		case Border::OUTSET:
			borderStyle = &myStyleSheet->outset;
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
}

void vc::GUIPanel::DrawBorderCorner( int dx, int dy, const vc::RectangleCoord &tileCoord ) {
	ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
	if ( bmp == nullptr ) {
		return;
	}

	al_draw_bitmap_region(
	        bmp,
	        tileCoord.x, tileCoord.y,
	        tileCoord.w, tileCoord.h,
	        dx, dy,
	        0
	        );
}

void vc::GUIPanel::DrawBorderEdge( int dx, int dy, int dw, int dh, const vc::RectangleCoord &tileCoord ) {
	ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
	if ( bmp == nullptr ) {
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
					backgroundColour.a
			),
			0.0f, 0.0f,
			dx, dy, dw / tileCoord.w, dh / tileCoord.h,
			0.0f,
			0
	);
}

void vc::GUIPanel::Tick() {
	if ( x + w < 0 || x > 640 || y + h < 0 || y > 480 ) {
		shouldDraw = false;
	} else {
		shouldDraw = true;
	}

	// Tick all of the children
	for ( auto i : children ) {
		i->Tick();
	}
}

void vc::GUIPanel::SetStyleSheet( GUIStyleSheet *styleSheet ) {
	myStyleSheet = styleSheet;
}

void vc::GUIPanel::GetContentPosition( int *xd, int *yd ) const {
	if ( myBorder == Border::NONE ) {
		GetPosition( xd, yd );
		return;
	}

	// Assume border is 2 pixels (idiotic)
	*xd = x + 2;
	*yd = y + 2;
}

void vc::GUIPanel::GetContentSize( int *wd, int *hd ) const {
	if ( myBorder == Border::NONE ) {
		GetSize( wd, hd );
		return;
	}

	// Assume border is 2 pixels (idiotic)
	*wd = w - 2;
	*hd = h - 2;
}

bool vc::GUIPanel::IsMouseOver() const {
	int mx, my;
	GetApp()->GetCursorPosition( &mx, &my );
	return !( mx < x || mx > x + w || my < y || my > y + h );
}

bool vc::GUIPanel::HandleMouseEvent( int mx, int my, int wheel, int button, bool buttonUp ) {
	for ( auto i : children ) {
		// If the child handles the event, return true
		if ( i->HandleMouseEvent( mx, my, wheel, button, buttonUp ) ) {
			return true;
		}
	}

	return false;
}
