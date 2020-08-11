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
	if ( myBackground == Background::NONE ) {
		return;
	}
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
