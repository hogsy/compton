// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Terrain.h"
#include "Serializer.h"
#include "Camera.h"
#include "Random.h"
#include "GameMode.h"

void ct::Terrain::TerrainTile::Draw( const Camera &camera, int offsetX, int offsetY )
{
	static const hei::Colour tileColour[ MAX_TERRAIN_TYPES ] = {
			{ 0, 255, 0 },
			{ 205, 203, 74 },
			{ 216, 216, 216 },
			{ 73, 77, 90 },
			{ 0, 0, 255 },
	};

	int isoX, isoY;
	render::TransformToIso( offsetX, offsetY, &isoX, &isoY );

	int x = isoX - camera.position.x;
	int y = isoY - camera.position.y;

#if 0
	if ( x - TILE_WIDTH > DISPLAY_WIDTH || ( x + TILE_WIDTH ) < 0 || y > DISPLAY_HEIGHT || ( y + TILE_HEIGHT ) < 0 )
		return;
#else
	static constexpr int HALF_H = TILE_HEIGHT / 2;

	if ( !render::IsVolumeVisible( x, y, TILE_WIDTH, TILE_HEIGHT ) )
		return;
#endif

	hei::Colour cornerColours[ 4 ];
	for ( unsigned int i = 0; i < 4; ++i )
	{
		cornerColours[ i ] = hei::Colour(
				( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].r ) * ( height[ i ] + 0.1f ) ),
				( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].g ) * ( height[ i ] + 0.1f ) ),
				( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].b ) * ( height[ i ] + 0.1f ) ) );
	}

	int c = 1;
	for ( unsigned int row = 0; row < HALF_H; ++row, c += 2 )
	{
		for ( unsigned int col = 0; col < ( c * 2 ); ++col )
		{
			render::DrawPixel( x - c + col, y + row, cornerColours[ 0 ] );
		}
	}

	c = TILE_WIDTH;
	for ( unsigned int row = 0; row < HALF_H; ++row, c -= 2 )
	{
		for ( unsigned int col = 0; col < ( c * 2 ); ++col )
		{
			render::DrawPixel( x - c + col, y + HALF_H + row, cornerColours[ 0 ] );
		}
	}
}

ct::Terrain::Terrain() {}
ct::Terrain::~Terrain() {}

void ct::Terrain::Deserialize( ct::Serializer *read )
{
	unsigned int numTiles = read->ReadI32();
	if ( numTiles != NUM_TILES )
		Error( "Invalid number of tiles stored in save data!\n" );

	for ( unsigned int i = 0; i < NUM_TILES; ++i )
	{
		tiles[ i ].corners[ 0 ].terrainType = static_cast< TerrainType >( read->ReadI32() );
		tiles[ i ].corners[ 1 ].terrainType = static_cast< TerrainType >( read->ReadI32() );
		for ( unsigned int j = 0; j < 4; ++j )
			tiles[ i ].height[ j ] = read->ReadF32();
	}
}

void ct::Terrain::Serialize( ct::Serializer *write )
{
	write->WriteI32( NUM_TILES );
	for ( unsigned int i = 0; i < NUM_TILES; ++i )
	{
		write->WriteI32( tiles[ i ].corners[ 0 ].terrainType );
		write->WriteI32( tiles[ i ].corners[ 1 ].terrainType );
		for ( unsigned int j = 0; j < 4; ++j )
			write->WriteF32( tiles[ i ].height[ j ] );
	}
}

void ct::Terrain::Draw( const Camera &camera )
{
	START_MEASURE();

	for ( unsigned int y = 0; y < NUM_TILES_COLUMN; ++y )
	{
		for ( unsigned int x = 0; x < NUM_TILES_ROW; ++x )
		{
			unsigned int tileNum = x + y * NUM_TILES_ROW;
			if ( tileNum >= NUM_TILES )
				break;

			float offsetX = x * TILE_WIDTH;
			float offsetY = y * TILE_HEIGHT;

			tiles[ tileNum ].Draw( camera, offsetX, offsetY );
		}
	}

	END_MEASURE();
}

void ct::Terrain::Generate( int seed )
{
	ct::random::PerlinNoise perlinNoise( seed );

	float fx = PIXEL_WIDTH / 3.0f;
	float fy = PIXEL_HEIGHT / 3.0f;

	double pz = PlClamp( 0.0f, PlGenerateRandomFloat( 100.0f ), 100.0f );

#if 0
	for ( unsigned int y = 0, yOfs = 0; y < NUM_TILES_COLUMN; ++y, yOfs += TILE_HEIGHT )
	{
		for ( unsigned int x = 0, xOfs = 0; x < NUM_TILES_ROW; ++x, xOfs += TILE_WIDTH )
		{
			unsigned int tileNum = x + y * NUM_TILES_ROW;

			// Generate the heightmap based on perlin noise
			tiles[ tileNum ].height[ 0 ] = perlinNoise.Noise( xOfs / fx, yOfs / fy, pz );
			tiles[ tileNum ].height[ 1 ] = perlinNoise.Noise( ( xOfs + TILE_WIDTH ) / fx, yOfs / fy, pz );
			tiles[ tileNum ].height[ 2 ] = perlinNoise.Noise( xOfs / fx, ( yOfs + TILE_HEIGHT ) / fy, pz );
			tiles[ tileNum ].height[ 3 ] = perlinNoise.Noise( ( xOfs + TILE_WIDTH ) / fx, ( yOfs + TILE_HEIGHT ) / fy, pz );

#	if 1
			// Determine if it's underwater or not
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 1 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
				tiles[ tileNum ].corners[ 0 ].terrainType = TERRAIN_WATER;
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 2 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
				tiles[ tileNum ].corners[ 1 ].terrainType = TERRAIN_WATER;
#	endif
		}
	}
#else
	for ( unsigned int y = 0; y < PIXEL_HEIGHT; ++y )
	{
		for ( unsigned int x = 0; x < PIXEL_WIDTH; ++x )
		{
			unsigned int xr = PlRoundUp( x * TILE_WIDTH / PIXEL_WIDTH, 1 );
			unsigned int yr = PlRoundUp( y * TILE_HEIGHT / PIXEL_WIDTH, 1 );

			unsigned int i = xr + yr * NUM_TILES_ROW;

			//Print( "%d\n", i );

			//tiles[ i ].heightBuffer.reserve( TILE_PIXEL_SIZE );
			//tiles[ i ].heightBuffer[ ]
		}
	}
#endif
}

bool ct::Terrain::IsWater( float x, float y )
{
	if ( x <= 0.0f || y <= 0.0f )
		return true;

	unsigned int xr = PlRoundUp( x * TILE_WIDTH / PIXEL_WIDTH, 1 );
	unsigned int yr = PlRoundUp( y * TILE_HEIGHT / PIXEL_HEIGHT, 1 );
	unsigned int tileNum = xr + yr * NUM_TILES_ROW;
	if ( tileNum >= NUM_TILES )
		return false;

	return ( ( tiles[ tileNum ].corners[ 0 ].terrainType == TERRAIN_WATER ) && ( tiles[ tileNum ].corners[ 1 ].terrainType == TERRAIN_WATER ) );
}
