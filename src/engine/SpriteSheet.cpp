// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_image.h>

#include "Compton.h"
#include "SpriteSheet.h"

vc::SpriteSheet::SpriteSheet( const char *path, const PLImage *bitmap ) : ScriptParser( path )
{
	Print( "Parsing sprite definition file, \"%s\"\n", path );

	PLImageFormat format      = PlGetImageFormat( bitmap );
	unsigned int  pixelSize   = PlGetImageFormatPixelSize( format );
	unsigned int  colourCount = PlGetNumImageFormatChannels( format );

	// This is dumb, but only support explicit pixel sizes relative to colour count for now
	if ( colourCount != pixelSize )
	{
		Warning( "Unsupported bitmap pixel/colour size for spritesheet!\n" );
		return;
	}

	unsigned int imageW = PlGetImageWidth( bitmap );
	unsigned int imageH = PlGetImageHeight( bitmap );

	bool hasAlpha = PlImageHasAlpha( bitmap );

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

		char id[ 16 ];

		if ( strlen( token ) >= sizeof( id ) )
		{
			Warning( "Token, \"%s\", is longer than maximum identifier length!\n", token );
		}

		strncpy( id, token, sizeof( id ) );

		// Width
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		int w = strtol( token, nullptr, 10 );

		// Height
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		int h = strtol( token, nullptr, 10 );

		// X
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		int x = strtol( token, nullptr, 10 );

		// Y
		if ( GetToken( token, sizeof( token ) ) == nullptr )
		{
			Warning( "Unexpected end of line at %d:%d!\n", GetLineNumber(), GetLinePosition() );
			continue;
		}
		int y = strtol( token, nullptr, 10 );

		Sprite sprite( w, h );
		sprite.pixels.reserve( w * h * pixelSize );
		sprite.hasAlpha = hasAlpha;



		sprites.emplace( id, sprite );

		Print( "Added \"%s\" to sprite table\n", id );
	}

	Print( "Done!\n" );
}

vc::SpriteSheet::~SpriteSheet() = default;

const vc::Sprite *vc::SpriteSheet::GetSprite( const char *spriteName ) const
{
	const auto &key = sprites.find( spriteName );
	if ( key == sprites.end() )
	{
		Warning( "Failed to find sprite index, \"%s\"!\n", spriteName );
		return nullptr;
	}

	return &key->second;
}
