/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "GUIPanel.h"
#include "SpriteSheet.h"

vc::GUIPanel::GUIPanel( vc::GUIPanel *parent, int x, int y, int w, int h, vc::GUIPanel::Background background, vc::GUIPanel::Border border )
    : myBackground( background ), myBorder( border ), parentPtr( parent ), x( x ), y( y ), w( w ), h( h ) {}

vc::GUIPanel::~GUIPanel() = default;

void vc::GUIPanel::Draw() {
	DrawBackground();

	// Draw all of the children
	for ( auto i : children ) {
		i->Draw();
	}
}

void vc::GUIPanel::DrawBackground() {
	if ( myBackground == Background::NONE ) {
		return;
	}
}

void vc::GUIPanel::Tick() {
}

void vc::GUIPanel::SetStyleSheet( const GUIStyleSheet *styleSheet ) {
	myStyleSheet = styleSheet;
}
