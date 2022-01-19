// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_image.h>
#include <plcore/pl_parse.h>

#include <memory>

#include "Compton.h"
#include "SpriteSheet.h"

vc::SpriteSheet::~SpriteSheet() = default;

/// Loads in and parses the given file, caching any sprites within.
/// \param path Path to an SDF/TXT file.
/// \return True on success, false on fail.
bool vc::SpriteSheet::LoadFile( const char *path )
{
	PLFile *file = PlOpenFile( path, true );
	if ( file == nullptr )
	{
		Warning( "Failed to load sprite sheet: %s\n", PlGetError() );
		return false;
	}

	Print( "Parsing sprite sheet, \"%s\"\n", path );

	unsigned int size = PlGetFileSize( file );
	char *buffer = new char[ size + 1 ];
	memcpy( buffer, PlGetFileData( file ), size );
	PlCloseFile( file );

	bool status = ParseFile( buffer );

	delete[] buffer;

	if ( status )
	{
		SetupElementTable();
	}

	return status;
}

/// Extracts a sprite from the given image, based on the coordinates provided.
/// \param image Pointer to the image handle. Assumed to be valid.
/// \param name Name that's given to the extracted sprite, for lookup.
/// \param x
/// \param y
/// \param w
/// \param h
void vc::SpriteSheet::ExtractSprite( const PLImage *image, const std::string &name, int x, int y, int w, int h )
{
	// Validate it
	if ( x + w > PlGetImageWidth( image ) || y + h > PlGetImageHeight( image ) )
	{
		Warning( "Invalid coordinates provided for element %u (%s)!\n", elements_.size(), name.c_str() );
		return;
	}

	PLImageFormat format = PlGetImageFormat( image );
	unsigned int pixelSize = PlGetImageFormatPixelSize( format );

	Sprite sprite( w, h );
	sprite.pixels.resize( w * h * pixelSize );
	sprite.hasAlpha = PlImageHasAlpha( image );

	// We're now going to copy the image pixels into our sprite
	unsigned int imageW = PlGetImageWidth( image );
	const uint8_t *src = PlGetImageData( image, 0 );
	src += ( x + y * imageW ) * pixelSize;
	for ( unsigned int py = 0; py < h; ++py )
	{
		memcpy( &sprite.pixels[ ( py * w ) * pixelSize ], src, w * pixelSize );
		src += imageW * pixelSize;
	}

	elements_.emplace( name, sprite );

	Print( "Added to sprite table (\"%s\" %d %d %d %d)\n", name.c_str(), x, y, w, h );
}

/// Parse the specific sprite.
/// \param p Pointer to buffer.
/// \param image Image handle, assumed to already be valid.
/// \return True on success, false on fail.
bool vc::SpriteSheet::ParseSprite( const char **p, const PLImage *image )
{
	char name[ 64 ];
	if ( PlParseEnclosedString( p, name, sizeof( name ) ) == nullptr )
	{
		Warning( "Invalid name encountered for sprite, skipping!\n" );
		return false;
	}

	char token[ 64 ];

	unsigned int w = 0, h = 0;
	unsigned int x = 0, y = 0;

	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		w = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		h = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		x = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		y = strtoul( token, nullptr, 10 );

	ExtractSprite( image, name, x, y, w, h );

	return true;
}

/// Allow us to parse in a group of sprites.
/// \param p Pointer to buffer.
/// \param image Image handle, assumed to already be valid.
/// \return True on success, false on fail.
bool vc::SpriteSheet::ParseGroup( const char **p, const PLImage *image )
{
	char name[ 64 ];
	if ( PlParseEnclosedString( p, name, sizeof( name ) ) == nullptr )
	{
		Warning( "Invalid name encountered for sprite, skipping!\n" );
		return false;
	}

	char token[ 64 ];

	unsigned int w = 0, h = 0;
	unsigned int x = 0, y = 0;
	unsigned int nx = 0, ny = 0;

	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		w = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		h = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		x = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		y = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		nx = strtoul( token, nullptr, 10 );
	if ( PlParseToken( p, token, sizeof( token ) ) != nullptr )
		ny = strtoul( token, nullptr, 10 );

	// Determine the real width and height
	unsigned int rw = w / nx;
	unsigned int rh = h / ny;

	unsigned int n = 0;
	for ( unsigned int row = 0; row < ny; ++row )
	{
		for ( unsigned int col = 0; col < nx; ++col )
		{
			std::string tmpName = std::string( name ) + std::to_string( n++ );

			// Determine the real x and y
			unsigned int rx = x + ( rw * col );
			unsigned int ry = y + ( rh * row );

			ExtractSprite( image, tmpName, rx, ry, rw, rh );
		}
	}

	return true;
}

bool vc::SpriteSheet::ParseFile( const char *buffer )
{
	bool status = false;
	PLImage *image = nullptr;
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
		if ( pl_strcasecmp( token, "$image" ) == 0 )
		{
			const char *path = PlParseEnclosedString( &p, token, sizeof( token ) );

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

			PlSkipLine( &p );
			continue;
		}

		if ( image == nullptr )
		{
			Warning( "Attempted to specify elements before providing sheet!\n" );
			break;
		}

		if ( ( pl_strcasecmp( token, "$sprite" ) == 0 ) )
			ParseSprite( &p, image );
		else if ( pl_strcasecmp( token, "$group" ) == 0 )
			ParseGroup( &p, image );
		else
			Warning( "Unknown tag type: \"%s\"!\n", token );

		PlSkipLine( &p );
	}

	PlDestroyImage( image );

	return status;
}

/// Looks up the given sprite. If it's not been cached, returns null.
/// \param spriteName Name of the sprite to find.
/// \return Pointer to the cached sprite, otherwise null.
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
