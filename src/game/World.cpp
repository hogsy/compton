// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Game.h"
#include "World.h"

#include "engine/Entity.h"

ct::World::World( const char *name, unsigned int seed ) : name_( name ),
                                                          seed_( seed )
{
}

ct::World::~World() = default;

void ct::World::Tick()
{
	IGameMode *gameMode = App::GetGameMode();
	assert( gameMode != nullptr );
	if ( gameMode == nullptr )
		return;

	if ( gameMode->IsPaused() )
		return;

	numSeconds_ += gameMode->GetSpeed();

	for ( auto &quadrant : quadrants_ )
		quadrant.occupiers.clear();

	gameMode->GetEntityManager()->Tick();
}

void ct::World::Draw( const ct::Camera &camera )
{
	IGameMode *gameMode = App::GetGameMode();
	assert( gameMode != nullptr );
	if ( gameMode == nullptr )
		return;

	START_MEASURE();

#if 1
	terrain_.Draw( camera );
#else
	srand( 5 );
	for ( unsigned int col = 0; col < NUM_QUADRANTS_COL; ++col )
	{
		for ( unsigned int row = 0; row < NUM_QUADRANTS_ROW; ++row )
		{
			int x = ( row * QUADRANT_WIDTH ) - camera.position.x;
			int y = ( col * QUADRANT_HEIGHT ) - camera.position.y;

			hei::Colour colour = hei::Colour( rand() % 255, rand() % 255, rand() % 255, 255 );

			render::DrawFilledRectangle( x, y, QUADRANT_WIDTH, QUADRANT_HEIGHT, colour );
		}
	}
#endif

	gameMode->GetEntityManager()->Draw( camera );

	END_MEASURE();
}

void ct::World::Deserialize( ct::Serializer *read )
{
	name_       = read->ReadString();
	seed_       = ( unsigned int ) read->ReadI32();
	numSeconds_ = ( unsigned int ) read->ReadI32();

	terrain_.Deserialize( read );
}

void ct::World::Serialize( Serializer *write )
{
	write->WriteString( name_.c_str() );
	write->WriteI32( ( int ) seed_ );
	write->WriteI32( ( int ) numSeconds_ );

	terrain_.Serialize( write );
}

void ct::World::Generate( unsigned int seed )
{
	IGameMode *gameMode = ct::App::GetGameMode();
	assert( gameMode != nullptr );
	if ( gameMode == nullptr )
		return;

	Print( "Generating terrain...\n" );
	terrain_.Generate( seed );

	Print( "Populating world...\n" );
	unsigned int n = random::GenerateRandomInteger( 4, 8 );
	for ( unsigned int i = 0; i < n; ++i )
	{
		// Pick a random point in the world
		int x, y;
		for ( unsigned int j = 0; j < 16; ++j )
		{
			// Try at least 16 times before we give up
			x = random::GenerateRandomInteger( 0, Terrain::PIXEL_WIDTH );
			y = random::GenerateRandomInteger( 0, Terrain::PIXEL_HEIGHT );

			const Terrain::Tile *tile = terrain_.GetTile( x, y );
			if ( tile != nullptr && tile->type == Terrain::TERRAIN_WATER )
				continue;

			x = y = -1.0f;
		}

		// Never found a point without water, give up...
		if ( x == -1 && y == -1 )
			continue;

		// Now attempt to spawn in the territory
		Settlement settlement;
		settlement.origin = math::Vector2( x, y );

		char tmp[ 32 ];
		random::GenerateRandomName( tmp, sizeof( tmp ) );
		settlement.name = tmp;

		// Spawn one Storehouse at the center
		Entity *hub  = gameMode->GetEntityManager()->CreateEntity( "Tree" );
		hub->origin_ = math::Vector2( x, y );

		unsigned int numAbodes = random::GenerateRandomInteger( 4, 16 );
		for ( unsigned int j = 0; j < numAbodes; ++j )
		{
			x = random::GenerateRandomInteger( settlement.origin.x - Settlement::MAX_BOUNDARY, settlement.origin.x + Settlement::MAX_BOUNDARY );
			y = random::GenerateRandomInteger( settlement.origin.y - Settlement::MAX_BOUNDARY, settlement.origin.y + Settlement::MAX_BOUNDARY );

			const Terrain::Tile *tile = terrain_.GetTile( x, y );
			if ( tile != nullptr && tile->type == Terrain::TERRAIN_WATER )
				continue;

			// Assign some citizens to the abode.
			int numCitizens = random::GenerateRandomInteger( 0, 4 );
			for ( unsigned int k = 0; k < numCitizens; ++k )
			{
				Entity *citizen = gameMode->GetEntityManager()->CreateEntity( "Pawn" );
				if ( citizen == nullptr )
					continue;

				citizen->origin_ = math::Vector2( x, y );
			}
		}

		settlements.push_back( settlement );
	}

	Entity *test    = gameMode->GetEntityManager()->CreateEntity( "Pawn" );
	test->origin_.x = random::GenerateRandomInteger( 0, Terrain::PIXEL_WIDTH );
	test->origin_.y = random::GenerateRandomInteger( 0, Terrain::PIXEL_HEIGHT );

	for ( unsigned int i = 0; i < 100; ++i )
	{
		Entity *test    = gameMode->GetEntityManager()->CreateEntity( "HumanCreature" );
		test->origin_.x = random::GenerateRandomInteger( 0, Terrain::PIXEL_WIDTH );
		test->origin_.y = random::GenerateRandomInteger( 0, Terrain::PIXEL_HEIGHT );
	}
}
