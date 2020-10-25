/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc {
	class Camera;
	class Serializer;

	enum TerrainType {
		TERRAIN_GRASS,
		TERRAIN_SAND,
		TERRAIN_SNOW,
		TERRAIN_STONE,
		TERRAIN_WATER,

		MAX_TERRAIN_TYPES
	};

	struct TerrainTileCorner {
		TerrainType terrainType{ TERRAIN_GRASS };
	};

	struct TerrainTile {
		TerrainTileCorner corners[ 2 ];
		float height[ 4 ];

		void Draw( const Camera &camera, float offsetX, float offsetY );
	};

	// And here we go...
#define TERRAIN_NUM_TILES_ROW 1024
#define TERRAIN_NUM_TILES_COLUMN 1024
#define TERRAIN_NUM_TILES ( TERRAIN_NUM_TILES_ROW * TERRAIN_NUM_TILES_COLUMN )
#define TERRAIN_TILE_WIDTH 64
#define TERRAIN_TILE_HEIGHT 64
#define TERRAIN_PIXEL_WIDTH ( TERRAIN_TILE_WIDTH * TERRAIN_NUM_TILES_ROW )
#define TERRAIN_PIXEL_HEIGHT ( TERRAIN_TILE_HEIGHT * TERRAIN_NUM_TILES_COLUMN )
#define TERRAIN_PIXEL_SIZE ( TERRAIN_PIXEL_WIDTH * TERRAIN_PIXEL_HEIGHT )

	class Terrain {
	public:
		Terrain();
		~Terrain();

		void Deserialize( Serializer *read );
		void Serialize( Serializer *write );

		void Tick();
		void Draw( const Camera &camera );

		void Generate();

	private:
		TerrainTile tiles[ TERRAIN_NUM_TILES ];
	};
}// namespace vc
