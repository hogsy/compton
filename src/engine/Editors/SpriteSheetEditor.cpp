// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "engine/Compton.h"
#include "SpriteSheetEditor.h"

void ct::SpriteSheetEditor::NewGame( const char *path )
{
}

void ct::SpriteSheetEditor::SaveGame( const char *path )
{
	Print( "Saving...\n" );

	Print( "Done!\n" );
}

void ct::SpriteSheetEditor::RestoreGame( const char *path )
{
}

void ct::SpriteSheetEditor::Tick()
{
}

void ct::SpriteSheetEditor::Draw()
{
}

void ct::SpriteSheetEditor::Precache()
{
}

bool ct::SpriteSheetEditor::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	return false;
}

bool ct::SpriteSheetEditor::HandleKeyboardEvent( int key, bool keyUp )
{
	switch ( key )
	{
		default:
			break;

		case ALLEGRO_KEY_Q:
			exit( 0 );
		case ALLEGRO_KEY_S:
		{
			SaveGame( std::string( "sprites/" + document_ ).c_str() );
			return true;
		}
	}

	return false;
}
