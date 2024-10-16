// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../script_parser.h"

struct ALLEGRO_BITMAP;

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
		int  x, y;
		int  w, h;
	};
	std::map< std::string, SpriteRect > sprites;

	const SpriteRect *GetSpriteRect( const char *spriteName ) const;

	ALLEGRO_BITMAP *bitmap;
};
