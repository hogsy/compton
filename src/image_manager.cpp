// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include <allegro5/allegro.h>

#include "plcore/pl_image.h"

#include "app.h"
#include "image_manager.h"
#include "render/render.h"

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

#if 0// Pulled, this is part of Yin now
ALLEGRO_BITMAP *ImageBitmap_LoadPacked( const char *path, int flags )
{
	PLImage *image = Image_LoadPackedImage( path );
	if ( image == nullptr )
	{
		return nullptr;
	}

	ALLEGRO_BITMAP *bitmap = ImageBitmap_ConvertPlatformImageToAllegroBitmap( image );

	PlDestroyImage( image );

	return bitmap;
}
#endif

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

ImageManager::ImageManager( int argc, char **argv )
{
}

ImageManager::~ImageManager()
{
}

void ImageManager::PrecacheResources()
{
	CachePalettes();
	CacheSprites();
}

/**
 * Loads and caches each of the respective palettes
 * we need to use for day/night cycling.
 */
void ImageManager::CachePalettes()
{
	for ( unsigned int i = 0; i < NUM_PALETTES; ++i )
	{
		std::string path = std::to_string( i ) + PALETTE_EXTENSION;
		PLFile     *file = PlOpenFile( path.c_str(), false );
		if ( file == nullptr )
		{
			Warning( "failed to load palette: %s\n", PlGetError() );
			continue;
		}

		bool status;
		for ( auto &colour : colourGroups_[ i ].colours )
		{
			// Convert the palette from 6-bit to 8-bit as we load it
			colour.r = ( PlReadInt8( file, &status ) * 255 ) / 63;
			colour.g = ( PlReadInt8( file, &status ) * 255 ) / 63;
			colour.b = ( PlReadInt8( file, &status ) * 255 ) / 63;
			if ( !status )
			{
				Warning( "io error: %s\n", PlGetError() );
				break;
			}
		}

		PlCloseFile( file );
	}
}

/**
 * Loads and caches each of the respective image sets
 * that hold background and general sprite data.
 */
void ImageManager::CacheSprites()
{
	for ( unsigned int i = 0; i < 999; ++i )
	{
		std::string numb = std::to_string( i );
		std::string path = std::string( 3 - numb.length(), '0' ) + numb + SPRITE_EXTENSION;

		if ( !PlFileExists( path.c_str() ) )
		{
			break;
		}

		PLFile *file = PlOpenFile( path.c_str(), false );
		if ( file == nullptr )
		{
			Warning( "Failed to load sprite group: %s\n", PlGetError() );
			continue;
		}

		SpriteGroup group;

		bool status;
		group.numSprites = PlReadInt16( file, false, &status );
		group.sprites.resize( group.numSprites );
		for ( unsigned int j = 0; j < group.numSprites; ++j )
		{
			uint16_t offset    = ( uint16_t ) PlReadInt16( file, false, &status );
			group.sprites[ j ] = {
			        ( uint8_t ) PlReadInt8( file, &status ),// width
			        ( uint8_t ) PlReadInt8( file, &status ) // height
			};

			// Reserve pixel buffer size (w*h)
			unsigned int bufferSize = group.sprites[ j ].width *
			                          group.sprites[ j ].height;
			group.sprites[ j ].pixels.resize( bufferSize );
			group.sprites[ j ].palette = true;

			// Now save, read in pixels and restore
			PLFileOffset p = PlGetFileOffset( file );
			if ( !PlFileSeek( file, offset, PL_SEEK_SET ) )
			{
				Warning( "failed to seek to offset: %s\n", PlGetError() );
				break;
			}
			if ( PlReadFile( file, group.sprites[ j ].pixels.data(), sizeof( uint8_t ), bufferSize ) != bufferSize )
			{
				Warning( "failed to read in pixel buffer: %s\n", PlGetError() );
				break;
			}
			PlFileSeek( file, p, PL_SEEK_SET );
		}

		spriteGroups_.push_back( group );

		PlCloseFile( file );
	}
}

ImageManager::Sprite *ImageManager::CacheSprite( const std::string &path )
{
	PLImage *image = PlLoadImage( path.c_str() );
	if ( image == nullptr )
	{
		Warning( "Failed to load image: %s\n", PlGetError() );
		return nullptr;
	}

	unsigned int bufferSize = PlGetImageDataSize( image );

	Sprite *sprite = new Sprite();
	sprite->width  = image->width;
	sprite->height = image->height;
	sprite->pixels.resize( bufferSize );

	memcpy( sprite->pixels.data(), image->data[ 0 ], bufferSize );

	PlDestroyImage( image );

	return sprite;
}

void ImageManager::Sprite::Draw( int x, int y, bool alphaTest ) const
{
	render::DrawBitmap( pixels.data(), 4, x, y, width, height, alphaTest );
}
