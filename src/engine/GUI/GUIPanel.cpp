/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "GUIPanel.h"

vc::GUIPanel::GUIPanel( vc::GUIPanel *parent, int x, int y, int w, int h, vc::GUIPanel::Background background, vc::GUIPanel::Border border )
    : myBackground( background ), myBorder( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h ) {
	if ( parent == nullptr ) {
		return;
	}

	// Push it onto the list of children
	parent->children.push_back( this );

	// Style should be the same as the parent
	myStyleSheet = parent->myStyleSheet;
}

vc::GUIPanel::~GUIPanel() = default;

void vc::GUIPanel::Draw() {
	DrawBackground();

	// Draw all of the children
	for ( auto i : children ) {
		i->Draw();
	}
}

void vc::GUIPanel::DrawBackground() {
	if ( myBackground == Background::NONE || !shouldDraw || myStyleSheet == nullptr ) {
		return;
	}

	ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
	if ( bmp == nullptr ) {
		return;
	}

	float sx, sy, sw, sh;
	switch( myBorder ) {
		default: break;
		case Border::INSET:
			break;
		case Border::OUTSET:
			break;
	}

#if 0
	sx = myStyleSheet->.x;
	sy = myStyleSheet->mouseStyles[ currentCursorMode ].y;
	sw = myStyleSheet->mouseStyles[ currentCursorMode ].w;
	sh = myStyleSheet->mouseStyles[ currentCursorMode ].h;

	al_draw_bitmap_region( bmp, sx, sy, sw, sh, x, y, 0 );
#endif
}

void vc::GUIPanel::Tick() {
	// Tick all of the children
	for ( auto i : children ) {
		i->Tick();
	}
}

void vc::GUIPanel::SetStyleSheet( GUIStyleSheet *styleSheet ) {
	myStyleSheet = styleSheet;
}
