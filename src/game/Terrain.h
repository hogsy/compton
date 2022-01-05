// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace vc
{
	class Camera;
	class Serializer;

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
		float             height[ 4 ];

		void Draw( const Camera &camera, float offsetX, float offsetY );
	};

	class Terrain
	{
	public:
		Terrain();
		~Terrain();

		void Deserialize( Serializer *read );
		void Serialize( Serializer *write );

		void Draw( const Camera &camera );

		void Generate();

		bool IsWater( float x, float y );

		static constexpr unsigned int NUM_TILES_ROW    = 32;
		static constexpr unsigned int NUM_TILES_COLUMN = 32;
		static constexpr unsigned int NUM_TILES        = NUM_TILES_ROW * NUM_TILES_COLUMN;

		static constexpr unsigned int TILE_WIDTH      = 64;
		static constexpr unsigned int TILE_HEIGHT     = 64;
		static constexpr unsigned int TILE_PIXEL_SIZE = TILE_WIDTH * TILE_HEIGHT;

		static constexpr unsigned int PIXEL_WIDTH  = TILE_WIDTH * NUM_TILES_COLUMN;
		static constexpr unsigned int PIXEL_HEIGHT = TILE_HEIGHT * NUM_TILES_ROW;
		static constexpr unsigned int PIXEL_SIZE   = PIXEL_WIDTH * PIXEL_HEIGHT;

	private:
		TerrainTile tiles[ NUM_TILES ];
	};
}// namespace vc
