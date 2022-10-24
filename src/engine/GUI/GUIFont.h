// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <vector>

#include "../Renderer/BitmapFont.h"

namespace ct
{
	class GUIFont
	{
	public:
		inline GUIFont()  = default;
		inline ~GUIFont() = default;

		bool LoadFile( const char *path, float scale );

		void DrawCharacter( int c, int x, int y );
		void DrawString( const char *string, int x, int y );

		struct Glyph
		{
			int                          w, h;
			int                          bw, bh;
			std::vector< unsigned char > buffer;
		};

		bool GetCharacterSize( int c, int *w, int *h, int *bw, int *bh ) const;

	private:
		BitmapFont *font;
	};
}// namespace ct
