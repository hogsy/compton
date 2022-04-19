// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Game.h"
#include "BaseCharacter.h"
#include "Terrain.h"
#include "Input/InputManager.h"

namespace ct
{
	/*
	enum class CharacterDirective {
		IDLE, // Do nothing
		EAT,  // Search for a food source
		DRINK,// Search for water or source of drink
		DEAD, // Do nothing
		CHAT, // Chat with other characters
		MOURN,// Cry over a character that died
		SLEEP,// Find somewhere to sleep

		MAX_DIRECTIVES
	};
	 */
}// namespace vc

REGISTER_ENTITY( BaseCharacter, ct::BaseCharacter )

void ct::BaseCharacter::Spawn()
{
	SuperClass::Spawn();

	// Cache the sprites we need
	CacheAnimationSet( "sprites/creatures/human/human.ani" );
	SetAnimation( nullptr );

	char firstName[ 8 ], lastName[ 8 ];
	random::GenerateRandomName( firstName, sizeof( firstName ) );
	random::GenerateRandomName( lastName, sizeof( lastName ) );
	snprintf( name, sizeof( name ), "%s %s %s", firstName, lastName, UTIL_GetRomanNumeralForNum( generation_ ) );
}

void ct::BaseCharacter::Deserialize( ct::Serializer *read )
{
	SuperClass::Deserialize( read );

	read->ReadString( name, sizeof( name ) );
}

void ct::BaseCharacter::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteString( name );
}

void ct::BaseCharacter::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	//al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 0, 255, 255 ) );
	//al_draw_pixel( origin.x, origin.y, al_map_rgb( 255, 0, 0 ) );

	//al_draw_filled_circle( debugGoal.x, debugGoal.y, 5.0f, al_map_rgb( 255, 0, 0 ) );
	//al_draw_line( origin.x, origin.y, debugGoal.x, debugGoal.y, al_map_rgb( 255, 0, 0 ), 1.0f );

	//al_draw_text( GetApp()->GetDefaultFont(), al_map_rgb( 255, 255, 255 ), origin.x, origin.y, 0, name );
}

void ct::BaseCharacter::Tick()
{
	SuperClass::Tick();

#if 0
	Terrain *terrainManager = App::GetGameMode()->GetTerrainManager();
	if ( terrainManager == nullptr )
	{
		return;
	}

	// Setup a goal for us to move to.
	if ( ( debugGoal.x == 0.0f && debugGoal.y == 0.0f ) ||
	     hei::Vector2( origin ) == debugGoal ||
	     debugGoalDelay < GetApp()->GetNumOfTicks() )
	{
		// Attempt to find a debug waypoint
		EntityManager::EntitySlot slot = GameMode::GetEntityManager()->FindEntityByClassName( "DebugWaypoint", nullptr );
		if ( slot.entity != nullptr )
		{
			debugGoal.x = random::GenerateRandomInteger( slot.entity->origin.x - 64, slot.entity->origin.x + 64 );
			debugGoal.y = random::GenerateRandomInteger( slot.entity->origin.y - 64, slot.entity->origin.y + 64 );
		}
		else
		{// otherwise fallback
			debugGoal.x = random::GenerateRandomInteger( origin.x - 64, origin.x + 64 );
			debugGoal.y = random::GenerateRandomInteger( origin.y - 64, origin.y + 64 );
		}

		// If our goal is in water, return so we can try again next tick
		if ( terrainManager->IsWater( debugGoal.x, debugGoal.y ) )
		{
			debugGoalDelay = 0;
			return;
		}

		debugGoalDelay = GetApp()->GetNumOfTicks() + random::GenerateRandomInteger( 50, 200 );
	}

	hei::Vector2 direction = debugGoal - origin;
	velocity += direction * direction.Length();
	velocity = PlClampVector2( &velocity, -1.0f, 1.0f );

	origin += velocity;
#endif
}
