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
	class ImageManager
	{
	public:
		ImageManager( int argc, char **argv );
		~ImageManager();

		void PrecacheResources();

		void ConvertAndExportImage( unsigned int set, unsigned int sNum, const std::string &path );

		enum
		{
			// Backgrounds
			SPR_GROUP_BACKGROUND_START = 0,
			SPR_GROUP_BACKGROUND_END   = 127,

			// Creatures
			SPR_GROUP_NORN_DEFAULT    = 128,
			SPR_GROUP_GRENDAL_DEFAULT = 135,

			// Other
			SPR_GROUP_OBJECTS_0 = 130,
			SPR_GROUP_FRONTEND  = 133,
			SPR_GROUP_BALLOON   = 134,
		};

		enum
		{
			SPR_CREATURE_HEAD_START        = 0,
			SPR_CREATURE_HEAD_END          = 23,
			SPR_CREATURE_TORSO_RIGHT_START = 24,
			SPR_CREATURE_TORSO_RIGHT_END   = 27,
			SPR_CREATURE_TORSO_LEFT_START  = 28,
			SPR_CREATURE_TORSO_LEFT_END    = 31,
			SPR_CREATURE_TORSO_START       = 24,
			SPR_CREATURE_TORSO_END         = 33,

			SPR_CURSOR = 0,

			SPR_BALLOON_HANDLE_UP   = 2,
			SPR_BALLOON_HANDLE_DOWN = 3,
		};

		enum
		{
			COLOUR_GROUP_DAWN,
			COLOUR_GROUP_DAY,
		};

	private:
		void CachePalettes();
		void CacheSprites();

	protected:
	private:
		static constexpr const char  *PALETTE_EXTENSION = ".PAL";
		static constexpr unsigned int NUM_PALETTES      = 5;
		// todo: multiple palettes likely for different tod, establish link...
	public:
		struct Palette
		{
			struct Index
			{
				int8_t r{ 0 };
				int8_t g{ 0 };
				int8_t b{ 0 };
			};
			Index colours[ 256 ];
		};
		inline const Palette *GetPalette( uint8_t index )
		{
			if ( index >= NUM_PALETTES )
			{
				Warning( "Invalid palette index: %u\n", index );
				return nullptr;
			}

			return &colourGroups_[ index ];
		}

	private:
		Palette colourGroups_[ NUM_PALETTES ];

	public:
		struct Sprite
		{
			uint8_t                width{ 0 };
			uint8_t                height{ 0 };
			std::vector< uint8_t > pixels;

			void Draw( int x, int y, bool alphaTest ) const;
		};

		/**
		 * Fetch a sprite from the cache.
		 * If it doesn't exist, returns null.
		 */
		inline const Sprite *GetSprite( uint16_t group, uint16_t id )
		{
			if ( group >= spriteGroups_.size() )
			{
				Warning( "Invalid sprite group %su specified!\n", group );
				return nullptr;
			}

			if ( id >= spriteGroups_[ group ].numSprites )
			{
				Warning( "Invalid sprite index %su specified!\n", id );
				return nullptr;
			}

			return &spriteGroups_[ group ].sprites[ id ];
		}

		void DrawSprite( uint16_t group, uint16_t id, int x, int y, bool alphaTest = false );

	private:
		static constexpr const char  *SPRITE_EXTENSION  = ".SPR";
		static constexpr unsigned int NUM_SPRITE_GROUPS = 136;
		struct SpriteGroup
		{
			uint16_t              numSprites{ 0 };
			std::vector< Sprite > sprites;
		};
		std::vector< SpriteGroup > spriteGroups_;
	};
}// namespace vc

ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags );
