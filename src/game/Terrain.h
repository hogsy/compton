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
