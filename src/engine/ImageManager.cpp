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

#include "../shared.h"
#include "ImageManager.h"

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

	int					   pixelFormat	= ImageBitmap_PlatformPixelFormatToAllegroPixelFormat( image->format );
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
			sPos[ 3 ]	  = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size ];
			sPos[ 2 ]	  = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size + 1 ];
			sPos[ 1 ]	  = image->data[ 0 ][ ( y * image->width + x ) * bitmapRegion->pixel_size + 2 ];
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

vc::ImageManager::ImageManager( int argc, char **argv )
{
}

vc::ImageManager::~ImageManager()
{
}

void vc::ImageManager::PrecacheResources()
{
	CachePalettes();
	CacheSprites();

	for ( unsigned int i = 0; i < NUM_SPRITE_GROUPS; ++i )
	{
		for ( unsigned int j = 0; j < spriteGroups_[ i ].numSprites; ++j )
		{
			ConvertAndExportImage( i, j, "sprites/spr" + std::to_string( i ) + "_" + std::to_string( j ) + ".png" );
		}
	}
}

void vc::ImageManager::ConvertAndExportImage( unsigned int set, unsigned int sNum, const std::string &path )
{
	if ( set >= NUM_SPRITE_GROUPS )
	{
		Warning( "Invalid image set: %lu/%lu\n", set, NUM_SPRITE_GROUPS );
		return;
	}

	if ( sNum >= spriteGroups_[ set ].numSprites )
	{
		Warning( "Invalid sprite: %lu/%lu\n", sNum, spriteGroups_[ set ].numSprites );
		return;
	}

	const SpriteGroup::Sprite *sprite = &spriteGroups_[ set ].sprites[ sNum ];

	PLImage *image = PlCreateImage( nullptr, sprite->width, sprite->height, PL_COLOURFORMAT_RGBA, PL_IMAGEFORMAT_RGBA8 );
	if ( image == nullptr )
	{
		Warning( "Failed to create image for export: %s\n", PlGetError() );
		return;
	}

	unsigned int size = sprite->width * sprite->height;
	for ( unsigned int i = 0, j = 0; j < size; i += 4, j++ )
	{
		image->data[ 0 ][ i ]	  = colourGroups_[ 0 ].colours[ sprite->pixels[ j ] ].r;
		image->data[ 0 ][ i + 1 ] = colourGroups_[ 0 ].colours[ sprite->pixels[ j ] ].g;
		image->data[ 0 ][ i + 2 ] = colourGroups_[ 0 ].colours[ sprite->pixels[ j ] ].b;
		image->data[ 0 ][ i + 3 ] = ( sprite->pixels[ j ] == 0 ) ? 0 : 255;
	}

	PlWriteImage( image, path.c_str() );

	PlDestroyImage( image );
}

/**
 * Loads and caches each of the respective palettes
 * we need to use for day/night cycling.
 */
void vc::ImageManager::CachePalettes()
{
	for ( unsigned int i = 0; i < NUM_PALETTES; ++i )
	{
		Print( "Caching palette %d/%d...", i + 1, NUM_PALETTES );

		std::string path = std::to_string( i ) + PALETTE_EXTENSION;
		PLFile	   *file = PlOpenFile( path.c_str(), false );
		if ( file == nullptr )
		{
			Warning( "failed to load palette: %s\n", PlGetError() );
			continue;
		}

		bool status;
		for ( unsigned int j = 0; j < 256; ++j )
		{
			// Convert the palette from 6-bit to 8-bit as we load it
			colourGroups_[ i ].colours[ j ].r = ( PlReadInt8( file, &status ) * 255 ) / 63;
			colourGroups_[ i ].colours[ j ].g = ( PlReadInt8( file, &status ) * 255 ) / 63;
			colourGroups_[ i ].colours[ j ].b = ( PlReadInt8( file, &status ) * 255 ) / 63;
			if ( !status )
			{
				Warning( "io error: %s\n", PlGetError() );
				break;
			}
		}

		PlCloseFile( file );

		if ( status )
		{
			Print( "done!\n" );
		}
	}
}

/**
 * Loads and caches each of the respective image sets
 * that hold background and general sprite data.
 */
void vc::ImageManager::CacheSprites()
{
	for ( unsigned int i = 0; i < NUM_SPRITE_GROUPS; ++i )
	{
		Print( "Caching sprite group %d/%d...", i + 1, NUM_SPRITE_GROUPS );

		std::string numb = std::to_string( i );
		std::string path = std::string( 3 - numb.length(), '0' ) + numb + SPRITE_EXTENSION;
		PLFile	   *file = PlOpenFile( path.c_str(), false );
		if ( file == nullptr )
		{
			Warning( "failed to load sprite group: %s\n", PlGetError() );
			continue;
		}

		bool status;
		spriteGroups_[ i ].numSprites = PlReadInt16( file, false, &status );
		spriteGroups_[ i ].sprites.reserve( spriteGroups_[ i ].numSprites );
		for ( unsigned int j = 0; j < spriteGroups_[ i ].numSprites; ++j )
		{
			spriteGroups_[ i ].sprites[ j ] = {
					( uint16_t ) PlReadInt16( file, false, &status ),// offset
					( uint8_t ) PlReadInt8( file, &status ),		 // width
					( uint8_t ) PlReadInt8( file, &status )			 // height
			};
			// Reserve pixel buffer size (w*h)
			unsigned int bufferSize = spriteGroups_[ i ].sprites[ j ].width *
									  spriteGroups_[ i ].sprites[ j ].height;
			spriteGroups_[ i ].sprites[ j ].pixels.reserve( bufferSize );
			// Now save, read in pixels and restore
			size_t p = PlGetFileOffset( file );
			if ( !PlFileSeek( file, spriteGroups_[ i ].sprites[ j ].offset, PL_SEEK_SET ) )
			{
				Warning( "failed to seek to offset: %s\n", PlGetError() );
				break;
			}
			if ( PlReadFile( file, spriteGroups_[ i ].sprites[ j ].pixels.data(), sizeof( uint8_t ), bufferSize ) != bufferSize )
			{
				Warning( "failed to read in pixel buffer: %s\n", PlGetError() );
				break;
			}
			PlFileSeek( file, p, PL_SEEK_SET );
		}

		PlCloseFile( file );

		if ( status )
		{
			Print( "done!\n" );
		}
	}
}
