// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "PlayerManager.h"

/**
 * Adds a brand new player to the game.
 */
int ct::PlayerManager::AddPlayer( const char *name, bool local )
{
	players_.emplace_back( name, local );
	return GetNumPlayers();
}

void ct::PlayerManager::HandleInput()
{
	for ( auto i : players_ )
	{
		if ( !i.isLocal )
			continue;

		// TODO: get input device etc etc etc
	}
}

/**
 * Return a pointer to the specified player.
 */
ct::PlayerManager::Player *ct::PlayerManager::GetPlayer( int id )
{
	if ( id < 0 || id >= players_.size() )
		return nullptr;

	return &players_[ id ];
}
