// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "SpriteManager.h"
#include "GameMode.h"

ct::SpriteManager::SpriteManager( int argc, char **argv )
{
}

ct::SpriteManager::~SpriteManager()
{
}

void ct::SpriteManager::PrecacheResources()
{
}

const ct::Sprite *ct::SpriteManager::GetSprite( const char *path, unsigned int group )
{
	// First check if it's cached already

	if ( group >= MAX_SPRITE_GROUPS )
	{
		Error( "Invalid sprite group, %u!\n", group );
	}

	auto i = spriteGroups_[ group ].sprites.find( path );
	if ( i != spriteGroups_[ group ].sprites.end() )
	{
		return &i->second;
	}

	// Nope? Okay, let's load it in

	PLImage *image = PlLoadImage( path );
	if ( image == nullptr )
	{
		Error( "Failed to cache image: %s\n", PlGetError() );
	}

	PLImageFormat format = PlGetImageFormat( image );
	if ( format != PL_IMAGEFORMAT_RGBA8 && format != PL_IMAGEFORMAT_RGB8 )
	{
		Error( "Unsupported pixel format!\n" );
	}

	// Set up the sprite handle
	Sprite sprite;
	sprite.width = PlGetImageWidth( image );
	sprite.height = PlGetImageHeight( image );
	sprite.hasAlpha = PlImageHasAlpha( image );

	// We're now going to copy the image pixels into our sprite
	unsigned int size = PlGetImageDataSize( image );
	const uint8_t *data = PlGetImageData( image, 0 );
	sprite.pixels.insert( sprite.pixels.end(), data, data + size );

	PlDestroyImage( image );

	// And now plop it into the group
	return &spriteGroups_[ group ].sprites.emplace( path, sprite ).first->second;
}

void ct::SpriteManager::DrawSprite( const char *path, unsigned int group, int x, int y, bool alphaTest )
{
	const Sprite *sprite = GetSprite( path, group );
	assert( sprite != nullptr );
	if ( sprite == nullptr )
	{
		return;
	}

	sprite->Draw( x, y, alphaTest );
}

const ct::SpriteSheet *ct::SpriteManager::GetSpriteSheet( const char *path )
{
	auto i = spriteSheets_.find( path );
	if ( i != spriteSheets_.end() )
	{
		return i->second;
	}

	auto sheet = new SpriteSheet();
	if ( !sheet->LoadFile( path ) )
	{
		delete sheet;
		return nullptr;
	}

	spriteSheets_.emplace( path, sheet );

	return sheet;
}
