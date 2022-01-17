// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../../shared.h"

// Draw Routines

void ClearDisplay( void ) {
	ALLEGRO_LOCKED_REGION *region = vc::GetApp()->region_;
	PL_ZERO( region->data, DISPLAY_HEIGHT * DISPLAY_WIDTH * region->pixel_size );
}

void DrawPixel( int x, int y, const hei::Colour &colour )
{
	if ( x < 0 || x > DISPLAY_WIDTH )
	{
		return;
	}

	if ( y < 0 || y > DISPLAY_HEIGHT )
	{
		return;
	}

	if ( colour.a != 255 )
	{
		al_put_blended_pixel( x, y, al_map_rgba( colour.r, colour.g, colour.b, colour.a ) );
	}
	else
	{
		al_put_pixel( x, y, al_map_rgb( colour.r, colour.g, colour.b ) );
	}
}

void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection )
{
	if ( ( x + w < 0 || x >= DISPLAY_WIDTH ) || ( y + h < 0 || y >= DISPLAY_HEIGHT ) )
	{
		return;
	}

	struct RGBA8
	{
		uint8_t r, g, b, a;
	};

	int dw = w;
	if ( x + dw > DISPLAY_WIDTH )
	{
		dw = DISPLAY_WIDTH - x;
	}
	int dh = h;
	if ( y + dh > DISPLAY_HEIGHT )
	{
		dh = DISPLAY_HEIGHT - y;
	}

	bool hasAlpha = ( pixelSize == 4 );
	ALLEGRO_LOCKED_REGION *region = vc::GetApp()->region_;
	uint8_t *dst = ( uint8_t * ) region->data + x * region->pixel_size + region->pitch * y;
	for ( unsigned int c = 0; c < dh; ++c )
	{
		for ( unsigned int r = 0; r < dw; ++r )
		{
			RGBA8 pixel;
			//if ( flipDirection == vc::render::FlipDirection::FLIP_VERTICAL )
			{
				pixel = ( const struct RGBA8 & ) pixels[ ( (dh - c - 1) * w ) * pixelSize ];
			}
			//else
			//{
			//	pixel = ( const struct RGBA8 & ) pixels[ ( r + c * w ) * pixelSize ];
			//}
			if ( alphaTest && hasAlpha && ( pixel.a != 255 ) )
			{
				if ( pixel.a == 0 )
				{
					continue;
				}

				al_put_blended_pixel( x, y, al_map_rgba( pixel.r, pixel.g, pixel.b, pixel.a ) );
				continue;
			}

			dst[ r * 3 + 0 ] = pixel.b;
			dst[ r * 3 + 1 ] = pixel.g;
			dst[ r * 3 + 2 ] = pixel.r;
		}

		dst += DISPLAY_WIDTH * region->pixel_size;
	}
}

void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour )
{
	if ( colour.a == 0 )
	{
		return;
	}

	for ( int row = 0; row < w; ++row )
	{
		for ( int column = 0; column < h; ++column )
		{
			DrawPixel( x + row, y + column, colour );
		}
	}
}
