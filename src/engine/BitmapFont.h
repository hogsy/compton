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

#pragma once

#include "../shared.h"

namespace vc
{
	class BitmapFont
	{
	public:
		inline BitmapFont() = default;
		~BitmapFont();

		bool LoadFromImage( uint8_t cw, uint8_t ch, uint16_t start, const char *path );
		bool LoadFromPSF( const char *path );

		void DrawCharacter( int x, int y, char c, const hei::Colour &colour = hei::Colour( 255, 255, 255 ) );
		void DrawString( int *x, int *y, const char *sentence, const hei::Colour &colour = hei::Colour( 255, 255, 255 ), bool shadow = false );

	private:
		uint8_t  cw_{ 0 };
		uint8_t  ch_{ 0 };
		uint16_t start_{ 0 };

	public:
		inline uint8_t GetCharacterWidth() const { return cw_; }
		inline uint8_t GetCharacterHeight() const { return ch_; }

	private:
		struct Glyph
		{
			std::vector< uint8_t > pixels;
		};
		std::vector< Glyph > glyphs_;
	};
}// namespace vc
