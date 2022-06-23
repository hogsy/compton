// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "GameMode.h"
#include "World.h"
#include "Random.h"
#include "Entity.h"

ct::World::World( const char *name, unsigned int seed ) : name_( name ),
														  seed_( seed )
{
}

ct::World::~World()
{
}

void ct::World::Tick()
{
	GameMode *gameMode = GetApp()->GetGameMode();
	if ( gameMode->GetState() == GameMode::GameState::PAUSED )
		return;

	numSeconds_ += gameMode->GetGameSpeed();

	for ( unsigned int i = 0; i < NUM_QUADRANTS; ++i )
		quadrants_[ i ].occupiers.clear();

	GameMode::GetEntityManager()->Tick();
}

void ct::World::Draw( const ct::Camera &camera )
{
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

	GameMode::GetEntityManager()->Draw( camera );

	END_MEASURE();
}

void ct::World::Deserialize( ct::Serializer *read )
{
	name_ = read->ReadString();
	seed_ = read->ReadI32();
	numSeconds_ = read->ReadI32();

	terrain_.Deserialize( read );
}

void ct::World::Serialize( Serializer *write )
{
	write->WriteString( name_.c_str() );
	write->WriteI32( seed_ );
	write->WriteI32( numSeconds_ );

	terrain_.Serialize( write );
}

void ct::World::Generate( unsigned int seed )
{
	Print( "Generating terrain...\n" );
	terrain_.Generate( seed );

	Print( "Populating world...\n" );
	unsigned int n = random::GenerateRandomInteger( 4, 8 );
	for ( unsigned int i = 0; i < n; ++i )
	{
		// Pick a random point in the world
		float x, y;
		for ( unsigned int j = 0; j < 16; ++j )
		{
			// Try at least 16 times before we give up
			x = random::GenerateRandomInteger( 0, Terrain::PIXEL_WIDTH );
			y = random::GenerateRandomInteger( 0, Terrain::PIXEL_HEIGHT );
			if ( !terrain_.IsWater( x, y ) )
				break;

			x = y = -1.0f;
		}

		// Never found a point without water, give up...
		if ( x == -1.0f && y == -1.0f )
			continue;

		// Now attempt to spawn in the territory
		Territory territory;
		territory.origin = hei::Vector2( x, y );

		// Spawn one Storehouse at the center
		Entity *hub = GameMode::GetEntityManager()->CreateEntity( "Tree" );
		hub->origin_ = hei::Vector2( x, y );

#define TERRITORY_BOUNDS 256

		unsigned int numAbodes = random::GenerateRandomInteger( 4, 16 );
		for ( unsigned int j = 0; j < numAbodes; ++j )
		{
			x = random::GenerateRandomInteger( territory.origin.x - TERRITORY_BOUNDS, territory.origin.x + TERRITORY_BOUNDS );
			y = random::GenerateRandomInteger( territory.origin.y - TERRITORY_BOUNDS, territory.origin.y + TERRITORY_BOUNDS );
			if ( terrain_.IsWater( x, y ) )
				continue;

			// Assign some citizens to the abode.
			int numCitizens = random::GenerateRandomInteger( 0, 4 );
			for ( unsigned int k = 0; k < numCitizens; ++k )
			{
				Entity *citizen = GameMode::GetEntityManager()->CreateEntity( "Tree" );
				if ( citizen == nullptr )
					continue;

				citizen->origin_ = hei::Vector2( x, y );
			}
		}

		territories_.push_back( territory );
	}

	for ( unsigned int i = 0; i < 20; ++i )
	{
		Entity *testEntity = GameMode::GetEntityManager()->CreateEntity( "HumanCreature" );
		testEntity->origin_ = hei::Vector2( rand() % Terrain::PIXEL_WIDTH, rand() % Terrain::PIXEL_HEIGHT );
	}
}
