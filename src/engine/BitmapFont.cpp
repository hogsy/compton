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

#include "BitmapFont.h"

bool vc::BitmapFont::LoadFromFile( uint8_t cw, uint8_t ch, const char *path )
{
	PLFile *file = PlOpenFile( path, false );
	if ( file == nullptr )
	{
		Warning( "Failed to load charset: %s\n", PlGetError() );
		return false;
	}

	cw_ = cw;
	ch_ = ch;
	for ( unsigned int i = 0; i < MAX_CHARS; ++i )
	{
		characters_[ i ].pixels.resize( cw_ * ch_ );
		if ( PlReadFile( file, characters_[ i ].pixels.data(), sizeof( uint8_t ), characters_[ i ].pixels.size() ) != characters_[ i ].pixels.size() )
		{
			Warning( "Failed to load character %lu: %s\n", i, PlGetError() );
			break;
		}
	}

	PlCloseFile( file );

	return true;
}

void vc::BitmapFont::DrawCharacter( int x, int y, char c, const hei::Colour &colour )
{
	if ( c < 0 )
	{
		return;
	}

	// Don't bother drawing it if it's outside visible bounds
	if ( x + cw_ < 0 || x > DISPLAY_WIDTH || y + ch_ < 0 || y > DISPLAY_HEIGHT )
	{
		return;
	}

	for ( unsigned int row = 0; row < cw_; ++row )
	{
		for ( unsigned int column = 0; column < ch_; ++column )
		{
			uint8_t pixel = characters_[ c ].pixels[ row + column * cw_ ];
			if ( pixel == 0 )
			{
				continue;
			}

			al_put_pixel( x + row, y + column, al_map_rgba( pixel * colour.r, pixel * colour.g, pixel * colour.b, 255 ) );
		}
	}
}

void vc::BitmapFont::DrawString( int *x, int *y, const char *sentence, const hei::Colour &colour, bool shadow )
{
	ALLEGRO_BITMAP *buffer = GetApp()->buffer;
	al_lock_bitmap( buffer, al_get_bitmap_format( buffer ), ALLEGRO_LOCK_READWRITE );

	if ( shadow )
	{
		int xx = *x + 1, yy = *y + 1;
		DrawString( &xx, &yy, sentence, hei::Colour( 0, 0, 0 ), false );
	}

	int			ox = *x;
	const char *p  = sentence;
	while ( *p != '\0' )
	{
		if ( *p == '\n' )
		{
			*y += ch_;
			*x = ox;
			p++;
			continue;
		}
		else if ( *p == '\t' )
		{
			*x += ( cw_ * 2 );
			p++;
			continue;
		}

		DrawCharacter( *x, *y, *p++, colour );

		*x += cw_;
	}

	al_unlock_bitmap( buffer );
}
