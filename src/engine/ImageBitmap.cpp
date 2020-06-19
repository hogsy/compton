/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "ImageBitmap.h"

static int ImageBitmap_PlatformPixelFormatToAllegroPixelFormat( PLImageFormat imageFormat ) {
	switch( imageFormat ) {
		default: break;
		case PL_IMAGEFORMAT_RGB8: return ALLEGRO_PIXEL_FORMAT_RGB_888;
		case PL_IMAGEFORMAT_RGBA8: return ALLEGRO_PIXEL_FORMAT_RGBA_8888;
	}

	Error( "Unexpected pixel format, %d!\n", imageFormat );
}

ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags ) {
	PLImage image;
	if ( !plLoadImage( path, &image ) ) {
		Warning( "Failed to load specified image, \"%s\" (%s)!", path, plGetError() );
		return nullptr;
	}

	ALLEGRO_BITMAP *bitmap = al_create_bitmap( image.width, image.height );
	if ( bitmap == nullptr ) {
		Error( "Failed to create bitmap!\n" );
	}

	int pixelFormat = ImageBitmap_PlatformPixelFormatToAllegroPixelFormat( image.format );
	ALLEGRO_LOCKED_REGION *bitmapRegion = al_lock_bitmap( bitmap, pixelFormat, ALLEGRO_LOCK_WRITEONLY );
	if ( bitmapRegion == nullptr ) {
		Error( "Failed to lock bitmap for writing, possibly not unlocked?!\n" );
	}

	for ( int y = 0; y < image.height; ++y ) {
		for ( int x = 0; x < image.width; ++x ) {
			uint8_t *sPos = ( uint8_t * ) ( bitmapRegion->data ) + ( bitmapRegion->pitch * y ) + ( x * bitmapRegion->pixel_size );
			sPos[ 0 ] = image.data[ 0 ][ ( y * image.width + x ) * bitmapRegion->pixel_size + 0 ];
			sPos[ 1 ] = image.data[ 0 ][ ( y * image.width + x ) * bitmapRegion->pixel_size + 2 ];
			sPos[ 2 ] = image.data[ 0 ][ ( y * image.width + x ) * bitmapRegion->pixel_size + 1 ];
			if ( bitmapRegion->pixel_size > 3 ) {
				sPos[ 3 ] = image.data[ 0 ][ ( y * image.width + x ) * bitmapRegion->pixel_size + 3 ];
			}
		}
	}

	al_unlock_bitmap( bitmap );

	plFreeImage( &image );

	return bitmap;
}
