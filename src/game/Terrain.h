// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Serializer.h"

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

		void Generate();

		bool IsWater( float x, float y );

		static constexpr unsigned int NUM_TILES_ROW = 32;
		static constexpr unsigned int NUM_TILES_COLUMN = 32;
		static constexpr unsigned int NUM_TILES = NUM_TILES_ROW * NUM_TILES_COLUMN;

		static constexpr unsigned int TILE_WIDTH = 64;
		static constexpr unsigned int TILE_HEIGHT = 64;
		static constexpr unsigned int TILE_PIXEL_SIZE = TILE_WIDTH * TILE_HEIGHT;

		static constexpr unsigned int PIXEL_WIDTH = TILE_WIDTH * NUM_TILES_COLUMN;
		static constexpr unsigned int PIXEL_HEIGHT = TILE_HEIGHT * NUM_TILES_ROW;
		static constexpr unsigned int PIXEL_SIZE = PIXEL_WIDTH * PIXEL_HEIGHT;

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

		struct TerrainTile
		{
			TerrainTileCorner corners[ 2 ];
			float height[ 4 ];

			std::vector< unsigned char > heightBuffer;
			std::vector< unsigned char > colourBuffer;

			void Draw( const Camera &camera, int offsetX, int offsetY );
		};

	private:
		TerrainTile tiles[ NUM_TILES ];
	};
}// namespace vc
