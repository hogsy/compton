// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Sensor.cpp
 *  For simple detection of external elements. Passes any information back to brain.
 *------------------------------------------------------------------------------------*/

#include "engine_private.h"

#include "game_mode.h"

#include "Sensor.h"

void vc::ai::Sensor::Tick()
{
	GameMode *gameMode = ( ( vc::GameMode * ) App::GetGameMode() );
	assert( gameMode != nullptr );
	if ( gameMode == nullptr )
	{
		return;
	}

	switch ( type_ )
	{
		case Type::SIGHT:
		{
			//std::vector< Entity * > entities = gameMode->GetEntityManager()->GetEntitiesInRange( brain_->owner_->origin_, 32.0f );
			//if ( entities.empty() )
			//{}
			break;
		}
		case Type::SOUND:
		{
			break;
		}
		case Type::SMELL:
		{
			break;
		}
		case Type::TASTE:
		{
			break;
		}
		case Type::TOUCH:
		{
			break;
		}
		case Type::PAIN:
		{
			break;
		}
		case Type::TIME:
		{
			break;
		}
		default: break;
	}
}
