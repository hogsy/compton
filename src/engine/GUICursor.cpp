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
}

void vc::GUICursor::Tick() {
	GetApp()->GetCursorPosition( &x, &y );

	int pX, pY, pW, pH;
	GUIPanel *panel = GetParent();
	if ( panel == nullptr ) {
		return;
	}

	panel->GetPosition( &pX, &pY );
	panel->GetSize( &pW, &pH );

	// Check the cursor is in the bounds of the parent
	if ( x + w < pX || x > pX + pW || y + h < pY || y > pY + pH ) {
		shouldDraw = false;
	} else {
		shouldDraw = true;
	}
}
