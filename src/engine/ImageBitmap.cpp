/*
SimGame Engine
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

#include "ImageBitmap.h"
#include "LoaderGfx.h"

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
