/*
SimGame Engine
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
		TERRAIN_WATER_DEEP,

		MAX_TERRAIN_TYPES
	};

	struct TerrainTileCorner
	{
		TerrainType terrainType{ TERRAIN_GRASS };
	};

	struct TerrainTile
	{
		TerrainTileCorner corners[ 2 ];
		float			  height[ 4 ];

		void Draw( const Camera &camera, float offsetX, float offsetY );
	};

	// And here we go...
#define TERRAIN_NUM_TILES_ROW	 32
#define TERRAIN_NUM_TILES_COLUMN 32
#define TERRAIN_NUM_TILES		 ( TERRAIN_NUM_TILES_ROW * TERRAIN_NUM_TILES_COLUMN )
#define TERRAIN_TILE_WIDTH		 64
#define TERRAIN_TILE_HEIGHT		 64
#define TERRAIN_PIXEL_WIDTH		 ( TERRAIN_TILE_WIDTH * TERRAIN_NUM_TILES_ROW )
#define TERRAIN_PIXEL_HEIGHT	 ( TERRAIN_TILE_HEIGHT * TERRAIN_NUM_TILES_COLUMN )
#define TERRAIN_PIXEL_SIZE		 ( TERRAIN_PIXEL_WIDTH * TERRAIN_PIXEL_HEIGHT )

	class Terrain
	{
	public:
		Terrain();
		~Terrain();

		void Deserialize( Serializer *read );
		void Serialize( Serializer *write );

		void Tick();
		void Draw( const Camera &camera );

		void Generate();

		bool IsWater( float x, float y );

	private:
		TerrainTile tiles[ TERRAIN_NUM_TILES ];
	};
}// namespace vc
