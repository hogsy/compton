// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "render.h"

// sigh...
#include "../shared.h"

// Draw Routines

static int scissorX = 0;
static int scissorY = 0;
static int scissorW = DISPLAY_WIDTH;
static int scissorH = DISPLAY_HEIGHT;

vc::render::DrawStats        vc::render::drawStats;
static vc::render::DrawStats lastDrawStats;

/**
 * Should be called before anything else is drawn.
 */
void vc::render::BeginDraw()
{
	drawStats.Clear();
}

/**
 * And this should be called after everything has drawn.
 */
void vc::render::EndDraw()
{
	//lastDrawStats = drawStats;
}

void vc::render::ClearDisplay()
{
	ALLEGRO_LOCKED_REGION *region = vc::GetApp()->region_;
	//PL_ZERO( region->data, DISPLAY_HEIGHT * DISPLAY_WIDTH * region->pixel_size );
	memset( region->data, 0x8F, DISPLAY_HEIGHT * DISPLAY_WIDTH * region->pixel_size );
}

void vc::render::DrawPixel( int x, int y, const PLColour &colour )
{
	if ( x < scissorX || x > scissorW || y < scissorY || y > scissorH )
		return;

	if ( colour.a != 255 )
		al_put_blended_pixel( x, y, al_map_rgba( colour.r, colour.g, colour.b, colour.a ) );
	else
		al_put_pixel( x, y, al_map_rgb( colour.r, colour.g, colour.b ) );
}

void vc::render::DrawLine( int sx, int sy, int ex, int ey, const hei::Colour &colour )
{
	if ( sx < scissorX && ex < scissorX || sx > DISPLAY_WIDTH && ex > DISPLAY_WIDTH )
		return;
	if ( sy < scissorY && ey < scissorY || sy > DISPLAY_HEIGHT && ey > DISPLAY_HEIGHT )
		return;

#define sign( x ) ( ( x ) > 0 ? 1 : ( ( x ) == 0 ? 0 : ( -1 ) ) )

	int dx = ex - sx;
	int dy = ey - sy;

	int sdx = sign( dx );
	int sdy = sign( dy );

	int dxa = std::abs( dx );
	int dya = std::abs( dy );

	int x = 0;
	int y = 0;

	int px = sx;
	int py = sy;

	if ( dxa >= dya )
	{
		for ( int i = 0; i < dxa; ++i )
		{
			y += dya;
			if ( y >= dxa )
			{
				y -= dxa;
				py += sdy;
			}

			DrawPixel( px, py, colour );

			px += sdx;
		}
	}
	else
	{
		for ( int i = 0; i < dya; ++i )
		{
			x += dxa;
			if ( x >= dya )
			{
				x -= dya;
				px += sdx;
			}

			DrawPixel( px, py, colour );

			py += sdy;
		}
	}
}

void vc::render::DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection )
{
	if ( !render::IsVolumeVisible( x, y, w, h ) )
	{
		return;
	}

	struct RGBA8
	{
		uint8_t r{ 255 }, g{ 255 }, b{ 255 }, a{ 255 };
	};

	int dw = w;
	if ( x + dw > scissorW )
	{
		dw = scissorW - x;
	}

	int dh = h;
	if ( y + dh > scissorH )
	{
		dh = scissorH - y;
	}

	bool                   hasAlpha = ( pixelSize == 4 );
	ALLEGRO_LOCKED_REGION *region   = vc::GetApp()->region_;
	uint8_t               *dst      = ( uint8_t                    *) region->data + x * region->pixel_size + region->pitch * y;
	for ( unsigned int row = 0; row < dh; ++row )
	{
		if ( y + row > scissorH )
			break;

		for ( unsigned int column = 0; column < dw; ++column )
		{
			if ( x + column > scissorW )
				continue;

			RGBA8 pixel;
			if ( flipDirection == vc::render::FlipDirection::FLIP_VERTICAL )
				pixel = ( const struct RGBA8 & ) pixels[ ( ( ( h - 1 ) - row ) * w + column ) * pixelSize ];
			else if ( flipDirection == vc::render::FlipDirection::FLIP_HORIZONTAL )
				pixel = ( const struct RGBA8 & ) pixels[ ( ( w - 1 - column ) + row * w ) * pixelSize ];
			else
				pixel = ( const struct RGBA8 & ) pixels[ ( column + row * w ) * pixelSize ];

			if ( alphaTest && hasAlpha && ( pixel.a != 255 ) )
			{
				if ( pixel.a == 0 )
					continue;

				al_put_blended_pixel( x, y, al_map_rgba( pixel.r, pixel.g, pixel.b, pixel.a ) );
				continue;
			}

			dst[ column * 3 + 0 ] = pixel.b;
			dst[ column * 3 + 1 ] = pixel.g;
			dst[ column * 3 + 2 ] = pixel.r;
		}

		dst += DISPLAY_WIDTH * region->pixel_size;
	}
}

void vc::render::DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, const vc::ImageManager::Palette *palette, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection )
{
	if ( !render::IsVolumeVisible( x, y, w, h ) )
	{
		return;
	}

	for ( int row = 0; row < w; ++row )
	{
		for ( int column = 0; column < h; ++column )
		{
			uint8_t pixel = pixels[ row + column * w ];
			if ( pixel == 0 )
			{
				continue;
			}

			PLColour c;
			c.r = palette->colours[ pixel ].r;
			c.g = palette->colours[ pixel ].g;
			c.b = palette->colours[ pixel ].b;
			c.a = 255;

			DrawPixel( x + row, y + column, c );
		}
	}
}

void vc::render::DrawFilledRectangle( int x, int y, int w, int h, const PLColour &colour )
{
	if ( colour.a == 0 )
		return;

	if ( x > scissorW || ( x + w ) < scissorX || y > scissorH || ( y + h ) < scissorY )
		return;

	for ( int row = 0; row < w; ++row )
	{
		for ( int column = 0; column < h; ++column )
		{
			DrawPixel( x + row, y + column, colour );
		}
	}
}

void vc::render::DrawPanel( int x, int y, int w, int h, const PLColour &colour )
{
	static constexpr unsigned int BORDER_WIDTH = 3;

	// draw top border

	// draw body
	DrawFilledRectangle( x + BORDER_WIDTH, y + BORDER_WIDTH, w - ( BORDER_WIDTH * 2 ), h - ( BORDER_WIDTH * 2 ), colour );

	// draw bottom border
	PLColour darkSide;
	if ( ( ( int ) colour.r ) - 5 > 0 )
		darkSide.r = colour.r - 5;
	if ( ( ( int ) colour.g ) - 5 > 0 )
		darkSide.g = colour.g - 5;
	if ( ( ( int ) colour.b ) - 5 > 0 )
		darkSide.b = colour.b - 5;
	for ( unsigned int row = 0; row < BORDER_WIDTH; ++row )
		DrawFilledRectangle( x - row, y + ( h - BORDER_WIDTH ) + row, w + row * 2, 1, darkSide );
}

void vc::render::SetScissor( int x, int y, int w, int h )
{
	scissorX = x;
	scissorY = y;
	scissorW = x + w;
	scissorH = y + h;
}

/**
 * Checks the given coordinates against the scissor volume.
 * If it returns false, it means that the given coordinates
 * are outside of the scissor volume.
 */
bool vc::render::IsVolumeVisible( int x, int y, int w, int h )
{
	return !( x > scissorW || x + w < scissorX || y > scissorH || y + h < scissorY );
}
