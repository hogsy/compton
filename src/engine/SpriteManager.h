// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Sprite.h"

namespace vc
{
	class SpriteManager
	{
	public:
		SpriteManager( int argc, char **argv );
		~SpriteManager();

		void PrecacheResources();

		enum
		{
			SPRITE_GROUP_GUI,
			SPRITE_GROUP_CHARACTERS,
			SPRITE_GROUP_BACKGROUND,

			MAX_SPRITE_GROUPS
		};

		enum
		{
			COLOUR_GROUP_DAWN,
			COLOUR_GROUP_DAY,
		};

		const Sprite *GetSprite( const char *path, unsigned int group );
		void          DrawSprite( const char *path, unsigned int group, int x, int y, bool alphaTest = false );

	private:
		struct SpriteGroup
		{
			std::map< std::string, Sprite > sprites;
		};
		SpriteGroup spriteGroups_[ MAX_SPRITE_GROUPS ];
	};
}// namespace vc

ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags );
