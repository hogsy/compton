// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

class ImageManager
{
public:
	ImageManager( int argc, char **argv );
	~ImageManager();

	void PrecacheResources();

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
		unsigned int           width{ 0 };
		unsigned int           height{ 0 };
		std::vector< uint8_t > pixels;
		bool                   palette;

		void Draw( int x, int y, bool alphaTest ) const;
	};

	static Sprite *CacheSprite( const std::string &path );

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

ALLEGRO_BITMAP *ImageBitmap_LoadGeneric( const char *path, int flags );
