/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <allegro5/allegro_primitives.h>

#include "SimGame.h"
#include "Terrain.h"
#include "Serializer.h"
#include "Camera.h"
#include "Random.h"

void vc::TerrainTile::Draw( const Camera &camera, float offsetX, float offsetY ) {
	static const PLColour tileColour[ MAX_TERRAIN_TYPES ] = {
			{ 0, 255, 0 },
			{ 205, 203, 74 },
			{ 216, 216, 216 },
			{ 73, 77, 90 },
			{ 0, 0, 255 }
	};

	if ( offsetX + TERRAIN_TILE_WIDTH > camera.position.x + DISPLAY_WIDTH ||
	     offsetX < camera.position.x - DISPLAY_WIDTH ||
	     offsetY + TERRAIN_TILE_HEIGHT > camera.position.y + DISPLAY_HEIGHT ||
	     offsetY < camera.position.y - DISPLAY_HEIGHT ) {
		return;
	}

	ALLEGRO_VERTEX vertices[ 6 ];
	memset( vertices, 0, sizeof ( ALLEGRO_VERTEX ) * 6 );

	vertices[ 0 ].x = offsetX;
	vertices[ 0 ].y = offsetY;
	vertices[ 0 ].color = al_map_rgb(
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );
	vertices[ 1 ].x = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 1 ].y = offsetY;
	vertices[ 1 ].color = al_map_rgb(
			( plFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( plFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( plFloatToByte( height[ 1 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );
	vertices[ 2 ].x = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 2 ].y = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 2 ].color = al_map_rgb(
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );

	vertices[ 3 ].x = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 3 ].y = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 3 ].color = al_map_rgb(
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 4 ].x = offsetX;
	vertices[ 4 ].y = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 4 ].color = al_map_rgb(
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 5 ].x = offsetX;
	vertices[ 5 ].y = offsetY;
	vertices[ 5 ].color = al_map_rgb(
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );

	al_draw_prim( vertices, nullptr, nullptr, 0, 6, ALLEGRO_PRIM_TRIANGLE_LIST );
	al_draw_rectangle( offsetX, offsetY, offsetX + TERRAIN_TILE_WIDTH, offsetY + TERRAIN_TILE_HEIGHT, al_map_rgba( 255, 255, 255, 50 ), 4.0f );
}

vc::Terrain::Terrain() {}
vc::Terrain::~Terrain() {}

void vc::Terrain::Deserialize( vc::Serializer *read ) {
	unsigned int numTiles = read->ReadInteger();
	if ( numTiles != TERRAIN_NUM_TILES ) {
		Error( "Invalid number of tiles stored in save data!\n" );
	}

	for ( unsigned int i = 0; i < TERRAIN_NUM_TILES; ++i ) {
		tiles[ i ].corners[ 0 ].terrainType = static_cast< TerrainType >( read->ReadInteger() );
		tiles[ i ].corners[ 1 ].terrainType = static_cast< TerrainType >( read->ReadInteger() );
		for ( unsigned int j = 0; j < 4; ++j ) {
			tiles[ i ].height[ j ] = read->ReadFloat();
		}
	}
}

void vc::Terrain::Serialize( vc::Serializer *write ) {
	write->WriteInteger( TERRAIN_NUM_TILES );
	for ( unsigned int i = 0; i < TERRAIN_NUM_TILES; ++i ) {
		write->WriteInteger( tiles[ i ].corners[ 0 ].terrainType );
		write->WriteInteger( tiles[ i ].corners[ 1 ].terrainType );
		for ( unsigned int j = 0; j < 4; ++j ) {
			write->WriteFloat( tiles[ i ].height[ j ] );
		}
	}
}

void vc::Terrain::Tick() {
}

void vc::Terrain::Draw( const Camera &camera ) {
	START_MEASURE();

	al_draw_rectangle( 0.0f, 0.0f, TERRAIN_PIXEL_WIDTH, TERRAIN_PIXEL_HEIGHT, al_map_rgb( 0, 0, 255 ), 16.0f );

	al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );

	for ( unsigned int y = 0; y < TERRAIN_NUM_TILES_COLUMN; ++y ) {
		for ( unsigned int x = 0; x < TERRAIN_NUM_TILES_ROW; ++x ) {
			unsigned int tileNum = x + y * TERRAIN_NUM_TILES_ROW;
			if ( tileNum >= TERRAIN_NUM_TILES ) {
				break;
			}

			float offsetX = x * TERRAIN_TILE_WIDTH;
			float offsetY = y * TERRAIN_TILE_HEIGHT;
			tiles[ tileNum ].Draw( camera, offsetX, offsetY );
		}
	}

	al_set_blender( ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA );

	END_MEASURE();
}

void vc::Terrain::Generate() {
	vc::random::PerlinNoise perlinNoise( (int)time( nullptr ) );

	float fx = TERRAIN_PIXEL_WIDTH / 128.0f;
	float fy = TERRAIN_PIXEL_HEIGHT / 128.0f;

	double pz = plClamp( 0.0f, plGenerateRandomf( 64.0f ), 100.0f );

	for ( unsigned int y = 0, yOfs = 0; y < TERRAIN_NUM_TILES_COLUMN; ++y, yOfs += TERRAIN_TILE_HEIGHT ) {
		for ( unsigned int x = 0, xOfs = 0; x < TERRAIN_NUM_TILES_ROW; ++x, xOfs += TERRAIN_TILE_WIDTH ) {
			unsigned int tileNum = x + y * TERRAIN_NUM_TILES_ROW;

			// Generate the heightmap based on perlin noise
			tiles[ tileNum ].height[ 0 ] = perlinNoise.Noise( xOfs / fx, yOfs / fy, pz );
			tiles[ tileNum ].height[ 1 ] = perlinNoise.Noise( ( xOfs + TERRAIN_TILE_WIDTH ) / fx, yOfs / fy, pz );
			tiles[ tileNum ].height[ 2 ] = perlinNoise.Noise( xOfs / fx, ( yOfs + TERRAIN_TILE_HEIGHT ) / fy , pz );
			tiles[ tileNum ].height[ 3 ] = perlinNoise.Noise( ( xOfs + TERRAIN_TILE_WIDTH ) / fx, ( yOfs + TERRAIN_TILE_HEIGHT ) / fy, pz );

#if 1
			// Determine if it's underwater or not
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 1 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f ) {
				tiles[ tileNum ].corners[ 0 ].terrainType = TERRAIN_WATER;
			}
			if ( tiles[ tileNum ].height[ 0 ] <= 0.0f || tiles[ tileNum ].height[ 2 ] <= 0.0f || tiles[ tileNum ].height[ 3 ] <= 0.0f ) {
				tiles[ tileNum ].corners[ 1 ].terrainType = TERRAIN_WATER;
			}
#endif
		}
	}
}