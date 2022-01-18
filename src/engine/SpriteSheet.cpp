// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_image.h>
#include <plcore/pl_parse.h>

#include <memory>

#include "Compton.h"
#include "SpriteSheet.h"

vc::SpriteSheet::~SpriteSheet() = default;

bool vc::SpriteSheet::LoadFile( const char *path )
{
	PLFile *file = PlOpenFile( path, true );
	if ( file == nullptr )
	{
		Warning( "Failed to load sprite sheet: %s\n", PlGetError() );
		return false;
	}

	Print( "Parsing sprite sheet, \"%s\"\n", path );

	unsigned int size   = PlGetFileSize( file );
	char        *buffer = new char[ size + 1 ];
	memcpy( buffer, PlGetFileData( file ), size );
	PlCloseFile( file );

	bool status = ParseFile( buffer, size );

	delete[] buffer;

	if ( status )
	{
		SetupElementTable();
	}

	return status;
}

bool vc::SpriteSheet::ParseFile( const char *buffer, unsigned int bufferSize )
{
	bool status = false;

	PLImage     *image = nullptr;
	unsigned int pixelSize;

	const char *p = buffer;
	while ( true )
	{
		if ( *p == '\0' )
		{
			// Made it to the end, yay!
			status = true;
			break;
		}

		char token[ 256 ];
		if ( PlParseToken( &p, token, sizeof( token ) ) == nullptr )
		{
			break;
		}

		if ( *token == ';' || PlIsEndOfLine( p ) )// Comment
		{
			PlSkipLine( &p );
			continue;
		}

		// Check for built-in commands
		if ( pl_strcasecmp( token, "$sheet" ) == 0 )
		{
			const char *path = PlParseToken( &p, token, sizeof( token ) );

			image = PlLoadImage( path );
			if ( image == nullptr )
			{
				Warning( "Failed to open sprite sheet: %s\n", PlGetError() );
				break;
			}

			PLImageFormat format = PlGetImageFormat( image );
			if ( format != PL_IMAGEFORMAT_RGBA8 && format != PL_IMAGEFORMAT_RGB8 )
			{
				Warning( "Unsupported pixel format!\n" );
				break;
			}

			pixelSize = ( format == PL_IMAGEFORMAT_RGBA8 ) ? 4 : 3;

			PlSkipLine( &p );
			continue;
		}

		if ( image == nullptr )
		{
			Warning( "Attempted to specify elements before providing sheet!\n" );
			break;
		}

		std::string elementName = token;

		if ( PlParseToken( &p, token, sizeof( token ) ) == nullptr )
		{
			break;
		}
		unsigned int w = strtoul( token, nullptr, 10 );

		if ( PlParseToken( &p, token, sizeof( token ) ) == nullptr )
		{
			break;
		}
		unsigned int h = strtoul( token, nullptr, 10 );

		if ( PlParseToken( &p, token, sizeof( token ) ) == nullptr )
		{
			break;
		}
		unsigned int x = strtoul( token, nullptr, 10 );

		if ( PlParseToken( &p, token, sizeof( token ) ) == nullptr )
		{
			break;
		}
		unsigned int y = strtoul( token, nullptr, 10 );

		// Validate it
		if ( x + w > PlGetImageWidth( image ) || y + h > PlGetImageHeight( image ) )
		{
			Warning( "Invalid coordinates provided for element %u (%s)!\n", elements_.size(), elementName.c_str() );
			PlSkipLine( &p );
			continue;
		}

		Sprite sprite( w, h );
		sprite.pixels.resize( w * h * pixelSize );
		sprite.hasAlpha = PlImageHasAlpha( image );

		// We're now going to copy the image pixels into our sprite
		unsigned int   imageW = PlGetImageWidth( image );
		const uint8_t *src    = PlGetImageData( image, 0 );
		src += ( x + y * imageW ) * pixelSize;
		for ( unsigned int py = 0; py < h; ++py )
		{
			memcpy( &sprite.pixels[ ( py * w ) * pixelSize ], src, w * pixelSize );
			src += imageW * pixelSize;
		}

		elements_.emplace( elementName, sprite );

		Print( "Added \"%s\" to sprite table\n", elementName.c_str() );
	}

	PlDestroyImage( image );

	return status;
}

const vc::Sprite *vc::SpriteSheet::LookupElement( const char *spriteName ) const
{
	const auto &key = elements_.find( spriteName );
	if ( key == elements_.end() )
	{
		Warning( "Failed to find sprite index, \"%s\"!\n", spriteName );
		return nullptr;
	}

	return &key->second;
}
