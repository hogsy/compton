/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <PL/platform_filesystem.h>

#include "SimGame.h"
#include "SpriteSheet.h"

vc::SpriteSheet::SpriteSheet() = default;
vc::SpriteSheet::~SpriteSheet() = default;

bool vc::SpriteSheet::LoadSpriteDefinitionFile( const char *path ) {
	Print( "Loading sprite definition file, \"%s\"\n", path );


	// Begin parsing it
	while ( *bufPos != '\0' ) {
		// Comment
		if ( *bufPos == ';' ) {
			SkipLine();
			continue;
		}

		if ( *bufPos == '\n' && bufPos[ 1 ] == '\r' ) {
			SkipLine();
			continue;
		}

		SpriteRect spriteRect;

		if ( GetToken( spriteRect.identifier, sizeof( spriteRect.identifier ) ) == nullptr ) {
			SkipLine();
			continue;
		}

		char token[ 4 ];

		sprites.emplace( std::pair< std::string, SpriteRect >( spriteRect.identifier, spriteRect ) );
	}

	return true;
}

bool vc::SpriteSheet::GetSpriteCoordinates( const char *spriteName, int *x, int *y, int *w, int *h ) {
	SpriteRect *spriteRect = GetSpriteRect( spriteName );
	if ( spriteRect == nullptr ) {
		*x = *y = *w = *h = 0;
		return false;
	}

	*x = spriteRect->x;
	*y = spriteRect->y;
	*w = spriteRect->w;
	*h = spriteRect->h;

	return true;
}

vc::SpriteSheet::SpriteRect *vc::SpriteSheet::GetSpriteRect( const char *spriteName ) {
	const auto &key = sprites.find( spriteName );
	if ( key == sprites.end() ) {
		Warning( "Failed to find sprite index, \"%s\"!\n", spriteName );
		return nullptr;
	}

	return &key->second;
}


