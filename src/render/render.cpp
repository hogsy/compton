// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"

#include "render.h"

// Draw Routines

static int scissorX = 0;
static int scissorY = 0;
static int scissorW = 128;
static int scissorH = 128;

render::DrawStats        render::drawStats;
static render::DrawStats lastDrawStats;

/**
 * Should be called before anything else is drawn.
 */
void render::BeginDraw()
{
	drawStats.Clear();
}

/**
 * And this should be called after everything has drawn.
 */
void render::EndDraw()
{
	//lastDrawStats = drawStats;
}

void render::ClearDisplay()
{
	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->GetDrawHeight();

	ALLEGRO_LOCKED_REGION *region = GetApp()->region_;
	//PL_ZERO( region->data, DISPLAY_HEIGHT * DISPLAY_WIDTH * region->pixel_size );
	memset( region->data, 0x8F, sh * sw * region->pixel_size );
}

void render::DrawPixel( int x, int y, const PLColour &colour )
{
	if ( x < scissorX || x > scissorW || y < scissorY || y > scissorH )
		return;

	if ( colour.a != 255 )
		al_put_blended_pixel( x, y, al_map_rgba( colour.r, colour.g, colour.b, colour.a ) );
	else
		al_put_pixel( x, y, al_map_rgb( colour.r, colour.g, colour.b ) );
}

void render::DrawLine( int sx, int sy, int ex, int ey, const hei::Colour &colour )
{
	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->GetDrawHeight();

	if ( sx < scissorX && ex < scissorX || sx > sw && ex > sw )
		return;
	if ( sy < scissorY && ey < scissorY || sy > sh && ey > sh )
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

void render::DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, FlipDirection flipDirection )
{
	if ( !render::IsVolumeVisible( x, y, w, h ) )
	{
		return;
	}

	struct RGBA8
	{
		uint8_t r{ 255 }, g{ 255 }, b{ 255 }, a{ 255 };
	};

	bool                   hasAlpha = ( pixelSize == 4 );
	ALLEGRO_LOCKED_REGION *region   = GetApp()->region_;

	uint8_t *dst = ( uint8_t * ) region->data + x * region->pixel_size + region->pitch * y;
	for ( unsigned int row = 0; row < h; ++row )
	{
		if ( y + row >= scissorY && y + row < scissorY + scissorH )
		{
			for ( unsigned int column = 0; column < w; ++column )
			{
				if ( x + column >= scissorX && x + column < scissorX + scissorW )
				{
					RGBA8 pixel;
					if ( flipDirection == FlipDirection::FLIP_VERTICAL )
					{
						pixel = ( const struct RGBA8 & ) pixels[ ( ( h - 1 - row ) * w + column ) * pixelSize ];
					}
					else if ( flipDirection == FlipDirection::FLIP_HORIZONTAL )
					{
						pixel = ( const struct RGBA8 & ) pixels[ ( ( w - 1 - column ) + row * w ) * pixelSize ];
					}
					else
					{
						pixel = ( const struct RGBA8 & ) pixels[ ( column + row * w ) * pixelSize ];
					}

					if ( alphaTest && hasAlpha && ( pixel.a != 255 ) )
					{
						if ( pixel.a == 0 )
						{
							continue;
						}
						al_put_blended_pixel( x + column, y + row, al_map_rgba( pixel.r, pixel.g, pixel.b, pixel.a ) );
						continue;
					}

					dst[ column * 3 + 0 ] = pixel.b;
					dst[ column * 3 + 1 ] = pixel.g;
					dst[ column * 3 + 2 ] = pixel.r;
				}
			}
		}
		dst += GetApp()->GetDrawWidth() * region->pixel_size;
	}
}

void render::DrawFilledRectangle( int x, int y, int w, int h, const PLColour &colour )
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

void render::DrawPanel( int x, int y, int w, int h, const PLColour &colour )
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

void render::SetScissor( int x, int y, int w, int h )
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
bool render::IsVolumeVisible( int x, int y, int w, int h )
{
	return !( x > scissorW || x + w < scissorX || y > scissorH || y + h < scissorY );
}
