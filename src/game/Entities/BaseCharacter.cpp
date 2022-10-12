// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Game.h"

#include "engine/Input/InputManager.h"

#include "BaseCharacter.h"
#include "../Terrain.h"

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
	char firstName[ 8 ], lastName[ 8 ];
	random::GenerateRandomName( firstName, sizeof( firstName ) );
	random::GenerateRandomName( lastName, sizeof( lastName ) );
	snprintf( name, sizeof( name ), "%s %s %s", firstName, lastName, UTIL_GetRomanNumeralForNum( generation_ ) );
}

void ct::BaseCharacter::Deserialize( ct::Serializer *read )
{
	SuperClass::Deserialize( read );

	//name = read->ReadString();
}

void ct::BaseCharacter::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteString( name );
}

void ct::BaseCharacter::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );
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

/**
 * Take control of the character.
 */
bool ct::BaseCharacter::TakeControl( int playerNum )
{
	IGameMode *gameMode = App::GetGameMode();
	assert( gameMode != nullptr );

	PlayerManager::Player *player = gameMode->GetPlayerManager()->GetPlayer( playerNum );
	if ( player == nullptr )
	{
		Warning( "Invalid player for entity, possible player is no longer playing!\n" );
		return false;
	}

	if ( player->controlTarget != nullptr )
	{
		Warning( "Player is already controlling a target!\n" );
		return false;
	}

	if ( playerNum == controllingPlayer_ )
		Warning( "Player is already controlling this entity!\n" );

	isPossessed_ = true;

	player->controlTarget = this;
	return isPossessed_;
}

/**
 * Release control of the character.
 */
void ct::BaseCharacter::ReleaseControl()
{
	isPossessed_ = false;

	// Ensure that the player is no longer controlling

}
