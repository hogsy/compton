// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "Sprite.h"

void vc::Sprite::Draw( int x, int y, bool alpha ) const
{
	if ( ( x + width < 0 || x >= DISPLAY_WIDTH ) || ( y + height < 0 || y >= DISPLAY_HEIGHT ) )
	{
		return;
	}

	struct RGBA8
	{
		uint8_t r, g, b, a;
	};

	unsigned int stride = hasAlpha ? 4 : 3;
	if ( alpha && hasAlpha )
	{
		for ( unsigned int row = 0; row < width; ++row )
		{
			for ( unsigned int column = 0; column < height; ++column )
			{
				RGBA8 pixel = ( const struct RGBA8 & ) pixels[ ( row + column * width ) * stride ];
				if ( pixel.a == 0 )
				{
					continue;
				}

				DrawPixel( x + row, y + column, { pixel.r, pixel.g, pixel.b, pixel.a } );
			}
		}
	}
	else
	{
		int dw = width;
		if ( x + dw > DISPLAY_WIDTH )
		{
			dw = DISPLAY_WIDTH - x;
		}
		int dh = height;
		if ( y + dh > DISPLAY_HEIGHT )
		{
			dh = DISPLAY_HEIGHT - y;
		}

		ALLEGRO_LOCKED_REGION *region = vc::GetApp()->region_;
		if ( region == nullptr )
		{
			return;
		}

		unsigned int rw   = dw * region->pixel_size;// total byte width of row
		uint8_t     *rowb = new uint8_t[ rw ];
		uint8_t     *dst  = ( uint8_t      *) region->data + x * region->pixel_size + region->pitch * y;
		for ( unsigned int row = 0; row < dh; ++row )
		{
			for ( unsigned int column = 0; column < dw; ++column )
			{
				RGBA8 pixel = ( const struct RGBA8 & ) pixels[ ( column + row * width ) * stride ];

				rowb[ column * 3 + 0 ] = pixel.b;
				rowb[ column * 3 + 1 ] = pixel.g;
				rowb[ column * 3 + 2 ] = pixel.r;
			}

			memcpy( dst, rowb, rw );

			dst += DISPLAY_WIDTH * region->pixel_size;
		}
		delete[] rowb;
	}
}
