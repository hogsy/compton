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

	private:
		void CachePalettes();
		void CacheSprites();

		enum
		{
			// Backgrounds
			IMAGESET_BACKGROUND_START = 0,
			IMAGESET_BACKGROUND_END	  = 127,

			// Creatures
			IMAGESET_NORN_DEFAULT = 128,

			// Other
			IMAGESET_UI			 = 133,
			IMAGESET_BALLOON	 = 134,
			IMAGESET_UNKNOWN_135 = 135,
		};

		enum
		{
			COLOUR_GROUP_DAWN,
			COLOUR_GROUP_DAY,

		};

	protected:
	private:
		static constexpr const char	*PALETTE_EXTENSION = ".PAL";
		static constexpr unsigned int NUM_PALETTES		= 5;
		// todo: multiple palettes likely for different tod, establish link...
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
		Palette colourGroups_[ NUM_PALETTES ];

		static constexpr const char	*SPRITE_EXTENSION	= ".SPR";
		static constexpr unsigned int NUM_SPRITE_GROUPS = 135;
		struct SpriteGroup
		{
			uint16_t numSprites{ 0 };
			struct Sprite
			{
				uint16_t			   offset{ 0 };
				uint8_t				   width{ 0 };
				uint8_t				   height{ 0 };
				std::vector< uint8_t > pixels;
			};
			std::vector< Sprite > sprites;
		};
		SpriteGroup spriteGroups_[ NUM_SPRITE_GROUPS ];
	};
}// namespace vc

ALLEGRO_BITMAP *ImageBitmap_LoadPacked( const char *path, int flags );
ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags );
