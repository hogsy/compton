// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Game.h"

#include "engine/Input/InputManager.h"

#include "BaseCharacter.h"
#include "../Terrain.h"

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

	std::string str = read->ReadString();
	snprintf( name, sizeof( name ), "%s", str.c_str() );
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

	auto *gameMode = ( DSGameMode * ) App::GetGameMode();
	if ( gameMode == nullptr )
		return;

	static constexpr int SPEED = 2;

	if ( isPossessed_ )
	{
		math::Vector2 desiredMove = origin_;
		if ( gameMode->GetAction( DSGameMode::Action::ACTION_MOVE_UP )->CheckStatus( input::State::DOWN ) )
			desiredMove.y -= SPEED;
		else if ( gameMode->GetAction( DSGameMode::Action::ACTION_MOVE_DOWN )->CheckStatus( input::State::DOWN ) )
			desiredMove.y += SPEED;

		if ( gameMode->GetAction( DSGameMode::Action::ACTION_MOVE_LEFT )->CheckStatus( input::State::DOWN ) )
			desiredMove.x -= SPEED;
		else if ( gameMode->GetAction( DSGameMode::Action::ACTION_MOVE_RIGHT )->CheckStatus( input::State::DOWN ) )
			desiredMove.x += SPEED;

		if ( stepTime_ > GetApp()->GetNumOfTicks() )
			return;

		// Check if the move is valid before committing to it
		const Terrain::Tile *tile = gameMode->GetWorld()->GetTerrain()->GetTile( desiredMove.x, desiredMove.y );
		if ( tile != nullptr && tile->corners[ 0 ].terrainType == Terrain::TERRAIN_WATER )
			return;

		origin_ = desiredMove;

		stepTime_ = GetApp()->GetNumOfTicks() + 5;
	}
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

	brain_.active = false;

	player->controlTarget = this;
	return isPossessed_;
}

/**
 * Release control of the character.
 */
void ct::BaseCharacter::ReleaseControl()
{
	isPossessed_       = false;
	controllingPlayer_ = -1;

	brain_.active = true;

	// Ensure that the player is no longer controlling
}

bool ct::BaseCharacter::IsMoving() const
{
	if ( !isPossessed_ )
		return SuperClass::IsMoving();


}
