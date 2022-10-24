// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Serializer.h"

namespace ct
{
	class Camera;
	class Serializer;

	class Terrain : public ISerializable
	{
	public:
		Terrain();
		~Terrain();

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera );

		void Generate( int seed );

		static constexpr int          NUM_TILES_ROW    = 64;
		static constexpr int          NUM_TILES_COLUMN = 64;
		static constexpr unsigned int NUM_TILES        = NUM_TILES_ROW * NUM_TILES_COLUMN;

		static constexpr int          TILE_WIDTH      = 16;
		static constexpr int          TILE_HEIGHT     = 16;
		static constexpr unsigned int TILE_PIXEL_SIZE = TILE_WIDTH * TILE_HEIGHT;

		static constexpr unsigned int PIXEL_WIDTH  = TILE_WIDTH * NUM_TILES_COLUMN;
		static constexpr unsigned int PIXEL_HEIGHT = TILE_HEIGHT * NUM_TILES_ROW;
		static constexpr unsigned int PIXEL_SIZE   = PIXEL_WIDTH * PIXEL_HEIGHT;

		enum TerrainType
		{
			TERRAIN_GRASS,
			TERRAIN_SAND,
			TERRAIN_SNOW,
			TERRAIN_STONE,
			TERRAIN_WATER,

			MAX_TERRAIN_TYPES
		};

		struct TerrainTileCorner
		{
			TerrainType terrainType{ TERRAIN_GRASS };
		};

		struct Tile
		{
			TerrainTileCorner corners[ 2 ];

			double height[ 4 ];

			TerrainType type{ TERRAIN_GRASS };

			std::vector< unsigned char > heightBuffer;
			std::vector< unsigned char > colourBuffer;

			void Draw( const Camera &camera, int offsetX, int offsetY, unsigned int num );
		};

		const Tile *GetTile( int x, int y ) const;
		const Tile *GetTile( const math::Vector2 &pos ) const;

	private:
		Tile tiles[ NUM_TILES ];
	};
}// namespace ct
