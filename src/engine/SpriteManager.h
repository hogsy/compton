// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Sprite.h"
#include "SpriteSheet.h"

namespace ct
{
	class SpriteManager
	{
	public:
		SpriteManager( int argc, char **argv );
		~SpriteManager();

		void Precache();

		enum
		{
			SPRITE_GROUP_GUI,
			SPRITE_GROUP_ENTITY,

			MAX_SPRITE_GROUPS
		};

		const Sprite *GetSprite( const char *path, unsigned int group );
		void DrawSprite( const char *path, unsigned int group, int x, int y, bool alphaTest = false );

		const SpriteSheet *GetSpriteSheet( const char *path );

	private:
		struct SpriteGroup
		{
			std::map< std::string, Sprite > sprites;
		};
		SpriteGroup spriteGroups_[ MAX_SPRITE_GROUPS ];

		std::map< std::string, SpriteSheet * > spriteSheets_;
	};
}// namespace ct
