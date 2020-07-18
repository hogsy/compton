/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "ScriptParser.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

namespace vc {
	class SpriteSheet : public ScriptParser {
	public:
		SpriteSheet( const char *path, ALLEGRO_BITMAP *bitmap );
		~SpriteSheet();

		bool GetSpriteCoordinates( const char *spriteName, int *x, int *y, int *w, int *h );

		ALLEGRO_BITMAP *GetBitmap() const { return bitmap; }

	private:
		struct SpriteRect {
			char identifier[ 32 ];
			int x, y;
			int w, h;
		};
		std::map< std::string, SpriteRect > sprites;

		SpriteRect *GetSpriteRect( const char *spriteName );

		ALLEGRO_BITMAP *bitmap;
	};
}// namespace vc
