/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <allegro5/allegro_primitives.h>

#include "Serializer.h"
#include "SimGame.h"
#include "Terrain.h"

void vc::TerrainTile::Draw( float offsetX, float offsetY ) {
	static const PLColour tileColour[ MAX_TERRAIN_TYPES ] = {
			{ 0, 255, 0 },
			{ 205, 203, 74 },
			{ 216, 216, 216 },
			{ 73, 77, 90 },
			{ 0, 0, 255 }
	};

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
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 0 ].terrainType ].r ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 0 ].terrainType ].g ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 0 ].terrainType ].b ) );

	vertices[ 3 ].x = offsetX + TERRAIN_TILE_WIDTH;
	vertices[ 3 ].y = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 3 ].color = al_map_rgb(
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 2 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 4 ].x = offsetX;
	vertices[ 4 ].y = offsetY + TERRAIN_TILE_HEIGHT;
	vertices[ 4 ].color = al_map_rgb(
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 3 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );
	vertices[ 5 ].x = offsetX;
	vertices[ 5 ].y = offsetY;
	vertices[ 5 ].color = al_map_rgb(
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].r ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].g ),
			( plFloatToByte( height[ 0 ] ) * tileColour[ corners[ 1 ].terrainType ].b ) );

	al_draw_prim( vertices, nullptr, nullptr, 0, 6, ALLEGRO_PRIM_TRIANGLE_LIST );
}

vc::Terrain::Terrain() {
	// For testing...
	for ( unsigned int i = 0; i < TERRAIN_NUM_TILES; ++i ) {
		tiles[ i ].height[ 0 ] =
		tiles[ i ].height[ 1 ] =
		tiles[ i ].height[ 2 ] =
		tiles[ i ].height[ 3 ] = 0.5f;
	}
}

vc::Terrain::~Terrain() {
}

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

void vc::Terrain::Draw() {
	// Figure out how many tiles are actually going to fit on screen
	static const unsigned int numTilesWide = ( DISPLAY_WIDTH / TERRAIN_TILE_WIDTH ) + TERRAIN_TILE_WIDTH;
	static const unsigned int numTilesTall = ( DISPLAY_HEIGHT / TERRAIN_TILE_HEIGHT ) + TERRAIN_TILE_HEIGHT;

	for ( unsigned int y = 0; y < numTilesTall; ++y ) {
		for ( unsigned int x = 0; x < numTilesWide; ++x ) {
			tiles[ x + y * TERRAIN_NUM_TILES_ROW ].Draw(
			        x * TERRAIN_TILE_WIDTH,
			        y * TERRAIN_TILE_HEIGHT );
		}
	}
}
