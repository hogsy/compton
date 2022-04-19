// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Terrain.h"
#include "Serializer.h"
#include "Camera.h"
#include "Random.h"
#include "World.h"
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

	int x = offsetX - camera.position.x;
	int y = offsetY - camera.position.y;

	if ( x > DISPLAY_WIDTH || ( x + TILE_WIDTH ) < 0 || y > DISPLAY_HEIGHT || ( y + TILE_HEIGHT ) < 0 )
	{
		return;
	}

	render::DrawFilledRectangle( x, y, TILE_WIDTH, TILE_HEIGHT,
	                             hei::Colour(
										 ( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].r ) * ( height[ 0 ] + 0.1f ) ),
										 ( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].g ) * ( height[ 0 ] + 0.1f ) ),
										 ( PlByteToFloat( tileColour[ corners[ 0 ].terrainType ].b ) * ( height[ 0 ] + 0.1f ) ) ) );

#if 0// todo: rewrite this, we we decide to retain it

	ALLEGRO_VERTEX vertices[ 6 ];
	memset( vertices, 0, sizeof( ALLEGRO_VERTEX ) * 6 );

	vertices[ 0 ].x     = offsetX;
	vertices[ 0 ].y     = offsetY;
	vertices[ 0 ].color = al_map_rgb(
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );
	vertices[ 1 ].x     = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 1 ].y     = offsetY;
	vertices[ 1 ].color = al_map_rgb(
			( PlFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( PlFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( PlFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );
	vertices[ 2 ].x     = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 2 ].y     = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 2 ].color = al_map_rgb(
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );

	vertices[ 3 ].x     = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 3 ].y     = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 3 ].color = al_map_rgb(
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( PlFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 4 ].x     = offsetX;
	vertices[ 4 ].y     = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 4 ].color = al_map_rgb(
			( PlFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( PlFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( PlFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 5 ].x     = offsetX;
	vertices[ 5 ].y     = offsetY;
	vertices[ 5 ].color = al_map_rgb(
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( PlFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );

	al_draw_prim( vertices, nullptr, nullptr, 0, 6, ALLEGRO_PRIM_TRIANGLE_LIST );

#	if 0
	float rectThick = camera.zoom / 4.0f;
	unsigned char rectAlpha = PlFloatToByte( camera.zoom ) * 50;
	al_draw_rectangle( offsetX, offsetY, offsetX + TERRAIN_TILE_WIDTH, offsetY + TERRAIN_TILE_HEIGHT, al_map_rgba( 255, 255, 255, rectAlpha ), rectThick );
#	endif
#endif
}

ct::Terrain::Terrain() {}
ct::Terrain::~Terrain() {}

void ct::Terrain::Deserialize( ct::Serializer *read )
{
	unsigned int numTiles = read->ReadInteger();
	if ( numTiles != NUM_TILES )
	{
		Error( "Invalid number of tiles stored in save data!\n" );
	}

	for ( unsigned int i = 0; i < NUM_TILES; ++i )
	{
		tiles[ i ].corners[ 0 ].terrainType = static_cast< TerrainType >( read->ReadInteger() );
		tiles[ i ].corners[ 1 ].terrainType = static_cast< TerrainType >( read->ReadInteger() );
		for ( unsigned int j = 0; j < 4; ++j )
		{
			tiles[ i ].height[ j ] = read->ReadFloat();
		}
	}
}

void ct::Terrain::Serialize( ct::Serializer *write )
{
	write->WriteInteger( NUM_TILES );
	for ( unsigned int i = 0; i < NUM_TILES; ++i )
	{
		write->WriteInteger( tiles[ i ].corners[ 0 ].terrainType );
		write->WriteInteger( tiles[ i ].corners[ 1 ].terrainType );
		for ( unsigned int j = 0; j < 4; ++j )
		{
			write->WriteFloat( tiles[ i ].height[ j ] );
		}
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
			{
				break;
			}

			float offsetX = x * TILE_WIDTH;
			float offsetY = y * TILE_HEIGHT;
			tiles[ tileNum ].Draw( camera, offsetX, offsetY );
		}
	}

	END_MEASURE();
}

void ct::Terrain::Generate()
{
	ct::random::PerlinNoise perlinNoise( ( int ) time( nullptr ) );

	float fx = PIXEL_WIDTH / 3.0f;
	float fy = PIXEL_HEIGHT / 3.0f;

	double pz = PlClamp( 0.0f, PlGenerateRandomFloat( 100.0f ), 100.0f );

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

#if 1
			// Determine if it's underwater or not
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 1 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
			{
				tiles[ tileNum ].corners[ 0 ].terrainType = TERRAIN_WATER;
			}
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 2 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f )
			{
				tiles[ tileNum ].corners[ 1 ].terrainType = TERRAIN_WATER;
			}
#endif
		}
	}
}

bool ct::Terrain::IsWater( float x, float y )
{
	if ( x <= 0.0f || y <= 0.0f )
	{
		return true;
	}

	unsigned int xr = PlRoundUp( x * TILE_WIDTH / PIXEL_WIDTH, 1 );
	unsigned int yr = PlRoundUp( y * TILE_HEIGHT / PIXEL_HEIGHT, 1 );
	unsigned int tileNum = xr + yr * NUM_TILES_ROW;
	if ( tileNum >= NUM_TILES )
	{
		return false;
	}

	return ( ( tiles[ tileNum ].corners[ 0 ].terrainType == TERRAIN_WATER ) && ( tiles[ tileNum ].corners[ 1 ].terrainType == TERRAIN_WATER ) );
}
