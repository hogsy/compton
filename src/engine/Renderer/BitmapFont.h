// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

namespace ct
{
	class BitmapFont
	{
	public:
		inline BitmapFont() = default;
		~BitmapFont();

		bool LoadFromImage( uint8_t cw, uint8_t ch, uint16_t start, const char *path );

		void DrawCharacter( int x, int y, unsigned char c, const hei::Colour &colour = hei::Colour( 255, 255, 255 ) ) const;
		void DrawString( int *x, int *y, const char *sentence, const hei::Colour &colour = hei::Colour( 255, 255, 255 ), bool shadow = false ) const;

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
}// namespace ct
