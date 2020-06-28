/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "ScriptParser.h"

namespace vc {
	class SpriteSheet : public ScriptParser {
	public:
		SpriteSheet();
		~SpriteSheet();

		bool LoadSpriteDefinitionFile( const char *path );

		bool GetSpriteCoordinates( const char *spriteName, int *x, int *y, int *w, int *h );

	private:
		struct SpriteRect {
			char identifier[ 32 ];
			int x, y;
			int w, h;
		};
		std::map< std::string, SpriteRect > sprites;

		SpriteRect *GetSpriteRect( const char *spriteName );
	};
}// namespace vc
