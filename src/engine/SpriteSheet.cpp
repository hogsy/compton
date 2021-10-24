/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Compton.h"
#include "SpriteSheet.h"

vc::SpriteSheet::SpriteSheet( const char *path, ALLEGRO_BITMAP *bitmap ) : ScriptParser( path ), bitmap( bitmap )
{
	Print( "Parsing sprite definition file, \"%s\"\n", path );

	while ( !IsEndOfFile() )
	{
		char token[ 4096 ];
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			continue;
		}

		if ( token[ 0 ] == ';' )
		{// Comment
			SkipLine();
			continue;
		}
		else if ( token[ 0 ] == '$' )
		{
			SkipLine();
			continue;
		}

		// Otherwise, assume it's an index into the sheet!

		SpriteRect spriteRect;

		if ( strlen( token ) >= sizeof( spriteRect.identifier ) )
		{
			Warning( "Token, \"%s\", is longer than maximum identifier length!\n", token );
		}

		strncpy( spriteRect.identifier, token, sizeof( spriteRect.identifier ) );

		// Width
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		spriteRect.w = atoi( token );

		// Height
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		spriteRect.h = atoi( token );

		// X
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		spriteRect.x = atoi( token );

		// Y
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		spriteRect.y = atoi( token );

		sprites.emplace( std::pair< std::string, SpriteRect >( spriteRect.identifier, spriteRect ) );

		Print( "Added \"%s\" to sprite table\n", spriteRect.identifier );
	}

	Print( "Done!\n" );
}

vc::SpriteSheet::~SpriteSheet() = default;

bool vc::SpriteSheet::GetSpriteCoordinates( const char *spriteName, int *x, int *y, int *w, int *h ) const
{
	const SpriteRect *spriteRect = GetSpriteRect( spriteName );
	if ( spriteRect == nullptr )
	{
		*x = *y = *w = *h = 0;
		return false;
	}

	*x = spriteRect->x;
	*y = spriteRect->y;
	*w = spriteRect->w;
	*h = spriteRect->h;

	return true;
}

const vc::SpriteSheet::SpriteRect *vc::SpriteSheet::GetSpriteRect( const char *spriteName ) const
{
	const auto &key = sprites.find( spriteName );
	if ( key == sprites.end() )
	{
		Warning( "Failed to find sprite index, \"%s\"!\n", spriteName );
		return nullptr;
	}

	return &key->second;
}
