// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "SpriteManager.h"
#include "GameMode.h"

static int ImageBitmap_PlatformPixelFormatToAllegroPixelFormat( PLImageFormat imageFormat )
{
	switch ( imageFormat )
	{
		default: break;
		case PL_IMAGEFORMAT_RGBA8:
			return ALLEGRO_PIXEL_FORMAT_RGBA_8888;
	}

	Error( "Unexpected pixel format, %d!\n", imageFormat );
}

static ALLEGRO_BITMAP *ImageBitmap_ConvertPlatformImageToAllegroBitmap( PLImage *image )
{
	al_set_new_bitmap_flags( ALLEGRO_ALPHA_TEST );

	ALLEGRO_BITMAP *bitmap = al_create_bitmap( image->width, image->height );
	if ( bitmap == nullptr )
	{
		Error( "Failed to create bitmap!\n" );
	}

	int                    pixelFormat  = ImageBitmap_PlatformPixelFormatToAllegroPixelFormat( image->format );
	ALLEGRO_LOCKED_REGION *bitmapRegion = al_lock_bitmap( bitmap, pixelFormat, ALLEGRO_LOCK_WRITEONLY );
	if ( bitmapRegion == nullptr )
	{
		Error( "Failed to lock bitmap for writing, possibly not unlocked?!\n" );
	}

	for ( int y = 0; y < image->height; ++y )
	{
		for ( int x = 0; x < image->width; ++x )
		{
			uint8_t *sPos = ( uint8_t * ) ( bitmapRegion->data ) + ( bitmapRegion->pitch * y ) + ( x * bitmapRegion->pixel_size );
			sPos[ 3 ]     = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size ];
			sPos[ 2 ]     = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size + 1 ];
			sPos[ 1 ]     = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size + 2 ];
			if ( bitmapRegion->pixel_size > 3 )
			{
				sPos[ 0 ] = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size + 3 ];
			}
		}
	}

	al_unlock_bitmap( bitmap );

	return bitmap;
}

ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags )
{
	PLImage *image = PlLoadImage( path );
	if ( image == NULL )
	{
		Warning( "Failed to load specified image, \"%s\" (%s)!", path, PlGetError() );
		return nullptr;
	}

	ALLEGRO_BITMAP *bitmap = ImageBitmap_ConvertPlatformImageToAllegroBitmap( image );

	PlDestroyImage( image );

	return bitmap;
}

///////////////////////////////////////////////////////////////////

vc::SpriteManager::SpriteManager( int argc, char **argv )
{
}

vc::SpriteManager::~SpriteManager()
{
}

void vc::SpriteManager::PrecacheResources()
{
}

const vc::Sprite *vc::SpriteManager::GetSprite( const char *path, unsigned int group )
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
	sprite.width    = PlGetImageWidth( image );
	sprite.height   = PlGetImageHeight( image );
	sprite.hasAlpha = PlImageHasAlpha( image );

	// We're now going to copy the image pixels into our sprite
	unsigned int   size = PlGetImageDataSize( image );
	const uint8_t *data = PlGetImageData( image, 0 );
	sprite.pixels.insert( sprite.pixels.end(), data, data + size );

	PlDestroyImage( image );

	// And now plop it into the group
	return &spriteGroups_[ group ].sprites.emplace( path, sprite ).first->second;
}

void vc::SpriteManager::DrawSprite( const char *path, unsigned int group, int x, int y, bool alphaTest )
{
	const Sprite *sprite = GetSprite( path, group );
	assert( sprite != nullptr );
	if ( sprite == nullptr )
	{
		return;
	}

	sprite->Draw( x, y, alphaTest );
}
