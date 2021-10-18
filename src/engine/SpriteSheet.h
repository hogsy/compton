/*
SimGame Engine
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

#include "ScriptParser.h"

struct ALLEGRO_BITMAP;

namespace vc
{
	class SpriteSheet : public ScriptParser
	{
	public:
		SpriteSheet( const char *path, ALLEGRO_BITMAP *bitmap );
		~SpriteSheet();

		bool GetSpriteCoordinates( const char *spriteName, int *x, int *y, int *w, int *h ) const;

		ALLEGRO_BITMAP *GetBitmap() const { return bitmap; }

	private:
		struct SpriteRect
		{
			char identifier[ 32 ];
			int	 x, y;
			int	 w, h;
		};
		std::map< std::string, SpriteRect > sprites;

		const SpriteRect *GetSpriteRect( const char *spriteName ) const;

		ALLEGRO_BITMAP *bitmap;
	};
}// namespace vc
