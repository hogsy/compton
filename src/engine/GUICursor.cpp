/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"
#include "GUICursor.h"

#include <allegro5/allegro.h>

vc::GUICursor::GUICursor( GUIPanel *parent, int x, int y, int w, int h )
	: GUIPanel( parent, x, y, w, h, Background::NONE, Border::NONE ) {}

vc::GUICursor::~GUICursor() {
}

void vc::GUICursor::DrawBackground() {
	if ( !shouldDraw || myStyleSheet == nullptr ) {
		return;
	}

	ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
	if ( bmp == nullptr ) {
		return;
	}

	float sx, sy, sw, sh;
	sx = myStyleSheet->mouseStyles[ currentCursorMode ].x;
	sy = myStyleSheet->mouseStyles[ currentCursorMode ].y;
	sw = myStyleSheet->mouseStyles[ currentCursorMode ].w;
	sh = myStyleSheet->mouseStyles[ currentCursorMode ].h;

	al_draw_bitmap_region( bmp, sx, sy, sw, sh, x, y, 0 );
	al_draw_pixel( x, y, al_map_rgba( 255, 255, 255, 255 ) );
}

void vc::GUICursor::Tick() {
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state( &state );

	int wW, wH;
	GetApp()->GetWindowSize( &wW, &wH );

	x = state.x * 640 / wW;
	y = state.y * 480 / wH;

	int pX, pY, pW, pH;
	GUIPanel *panel = GetParent();
	panel->GetPosition( &pX, &pY );
	panel->GetSize( &pW, &pH );

	// Check the cursor is in the bounds of the parent
	if ( x + w < pX || x > pX + pW || y + h < pY || y > pY + pH ) {
		shouldDraw = false;
	} else {
		shouldDraw = true;
	}
}
