// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Game.h"
#include "Terrain.h"

void ct::Terrain::Tile::Draw( const Camera &camera, int offsetX, int offsetY, unsigned int num )
{
	static const hei::Colour tileColour[ MAX_TERRAIN_TYPES ] = {
	        {0,    255, 0  }, // grass
	        { 205, 203, 74 }, // sand
	        { 216, 216, 216}, // snow
	        { 73,  77,  90 }, // stone
	        { 0,   0,   255}, // water
	};

	math::Vector2 iso = math::Vector2( offsetX, offsetY ).ToIso();
	int           x   = iso.x - ( int ) camera.position.x;
	int           y   = iso.y - ( int ) camera.position.y;

	if ( !render::IsVolumeVisible( x, y, TILE_WIDTH, TILE_HEIGHT ) )
		return;

	hei::Colour cornerColours[ 4 ];
	for ( unsigned int i = 0; i < 4; ++i )
	{
		cornerColours[ i ] = hei::Colour(
		        ( PlByteToFloat( tileColour[ type ].r ) * ( ( float ) height[ i ] + 0.1f ) ),
		        ( PlByteToFloat( tileColour[ type ].g ) * ( ( float ) height[ i ] + 0.1f ) ),
		        ( PlByteToFloat( tileColour[ type ].b ) * ( ( float ) height[ i ] + 0.1f ) ) );
	}

	static constexpr int HALF_H = TILE_HEIGHT / 2;

	srand( num );

	int c = 1;
	for ( int row = 0; row < HALF_H; ++row, c += 2 )
	{
		for ( int col = 0; col < ( c * 2 ); ++col )
		{
			if ( ( type == TERRAIN_GRASS ) && ( ( rand() % 10 ) == 1 ) )
			{
				render::DrawPixel( x - c + col, y + row, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 3U, cornerColours[ 0 ].b ) );
				render::DrawPixel( x - c + col, y + row - 1, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 4U, cornerColours[ 0 ].b ) );
				render::DrawPixel( x - c + col, y + row - 2, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 5U, cornerColours[ 0 ].b ) );
			}
			else
				render::DrawPixel( x - c + col, y + row, cornerColours[ 0 ] );
		}
	}

	c = TILE_WIDTH;
	for ( int row = 0; row < HALF_H; ++row, c -= 2 )
	{
		for ( int col = 0; col < ( c * 2 ); ++col )
		{
			render::DrawPixel( x - c + col, y + HALF_H + row, cornerColours[ 0 ] );
			if ( ( type == TERRAIN_GRASS ) && ( ( rand() % 10 ) == 1 ) )
			{
				render::DrawPixel( x - c + col, y + HALF_H + row, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 3U, cornerColours[ 0 ].b ) );
				render::DrawPixel( x - c + col, y + HALF_H + row - 1, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 4U, cornerColours[ 0 ].b ) );
				render::DrawPixel( x - c + col, y + HALF_H + row - 2, hei::Colour( cornerColours[ 0 ].r, cornerColours[ 0 ].g - 5U, cornerColours[ 0 ].b ) );
			}
			else
				render::DrawPixel( x - c + col, y + HALF_H + row, cornerColours[ 0 ] );
		}
	}

	render::drawStats.stats[ render::DrawStats::Type::DRAW_STATS_TILE ]++;
}

ct::Terrain::Terrain()  = default;
ct::Terrain::~Terrain() = default;

void ct::Terrain::Deserialize( ct::Serializer *read )
{
	unsigned int numTiles = read->ReadI32();
	if ( numTiles != NUM_TILES )
		Error( "Invalid number of tiles stored in save data!\n" );

	for ( auto &tile : tiles )
	{
		tile.corners[ 0 ].terrainType = static_cast< TerrainType >( read->ReadI32() );
		tile.corners[ 1 ].terrainType = static_cast< TerrainType >( read->ReadI32() );
		for ( auto &j : tile.height )
			j = read->ReadF32();
	}
}

void ct::Terrain::Serialize( ct::Serializer *write )
{
	write->WriteI32( NUM_TILES );
	for ( auto &tile : tiles )
	{
		write->WriteI32( tile.corners[ 0 ].terrainType );
		write->WriteI32( tile.corners[ 1 ].terrainType );
		for ( auto j : tile.height )
			write->WriteF32( j );
	}
}

void ct::Terrain::Draw( const Camera &camera )
{
	START_MEASURE();

	for ( int y = 0; y < NUM_TILES_COLUMN; ++y )
	{
		for ( int x = 0; x < NUM_TILES_ROW; ++x )
		{
			unsigned int tileNum = x + y * NUM_TILES_ROW;
			if ( tileNum >= NUM_TILES )
				break;

			tiles[ tileNum ].Draw( camera, x * TILE_WIDTH, y * TILE_HEIGHT, tileNum );
		}
	}

	END_MEASURE();
}

void ct::Terrain::Generate( int seed )
{
	ct::random::PerlinNoise perlinNoise( seed );

	double px = PIXEL_WIDTH / 3.0;
	double py = PIXEL_HEIGHT / 3.0;
	double pz = PlClamp( 0.0, PlGenerateRandomDouble( 100.0 ), 100.0 );

	for ( unsigned int y = 0, yOfs = 0; y < NUM_TILES_COLUMN; ++y, yOfs += TILE_HEIGHT )
	{
		for ( unsigned int x = 0, xOfs = 0; x < NUM_TILES_ROW; ++x, xOfs += TILE_WIDTH )
		{
			unsigned int tileNum = x + y * NUM_TILES_ROW;

			// Generate the heightmap based on perlin noise
			tiles[ tileNum ].height[ 0 ] = PlClamp( 0, perlinNoise.Noise( xOfs / px, yOfs / py, pz ), 1.0 );
			tiles[ tileNum ].height[ 1 ] = perlinNoise.Noise( ( xOfs + TILE_WIDTH ) / px, yOfs / py, pz );
			tiles[ tileNum ].height[ 2 ] = perlinNoise.Noise( xOfs / px, ( yOfs + TILE_HEIGHT ) / py, pz );
			tiles[ tileNum ].height[ 3 ] = perlinNoise.Noise( ( xOfs + TILE_WIDTH ) / px, ( yOfs + TILE_HEIGHT ) / py, pz );

			tiles[ tileNum ].heightBuffer.reserve( TILE_PIXEL_SIZE );

#if 0
			// Determine if it's underwater or not
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 1 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
				tiles[ tileNum ].corners[ 0 ].terrainType = TERRAIN_WATER;
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 2 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
				tiles[ tileNum ].corners[ 1 ].terrainType = TERRAIN_WATER;
#else
			if ( tiles[ tileNum ].height[ 0 ] <= 0.2 )
				tiles[ tileNum ].type = TERRAIN_WATER;
			else if ( tiles[ tileNum ].height[ 0 ] <= 0.3 )
				tiles[ tileNum ].type = TERRAIN_SAND;
#endif
		}
	}
}

const ct::Terrain::Tile *ct::Terrain::GetTile( int x, int y ) const
{
	if ( x < 0 || x >= PIXEL_WIDTH || y < 0 || y >= PIXEL_HEIGHT )
		return nullptr;

	unsigned int tile = x + y * NUM_TILES_ROW;
	if ( tile >= NUM_TILES )
		return nullptr;

	return &tiles[ tile ];
}

const ct::Terrain::Tile *ct::Terrain::GetTile( const ct::math::Vector2 &pos ) const
{
	return GetTile( pos.x, pos.y );
}
