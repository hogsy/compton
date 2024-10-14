// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

namespace vc
{
	class BitmapFont
	{
	public:
		inline BitmapFont() = default;
		~BitmapFont();

		bool LoadFromFile( uint8_t cw, uint8_t ch, const char *path );

		void DrawCharacter( int x, int y, char c, const hei::Colour &colour = hei::Colour( 255, 255, 255 ) );
		void DrawString( int *x, int *y, const char *sentence, const hei::Colour &colour = hei::Colour( 255, 255, 255 ), bool shadow = false );

	private:
		static constexpr unsigned int MAX_CHARS = 128;

		uint8_t cw_{ 0 };
		uint8_t ch_{ 0 };

	public:
		inline uint8_t GetCharacterWidth() const { return cw_; }
		inline uint8_t GetCharacterHeight() const { return ch_; }

	private:
		struct Character
		{
			std::vector< uint8_t > pixels;
		} characters_[ MAX_CHARS ];
	};
}// namespace vc
