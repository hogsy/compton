// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"

namespace ct
{
	class PlayerManager
	{
	public:
		struct Player
		{
			Player( const std::string &sName ) : name( sName ) {}

			bool isLocal{ true };
			std::string name;
			Entity *controlTarget{ nullptr };
		};

		unsigned int AddPlayer( const char *name );
		void RemovePlayer( unsigned int id );

		void HandleInput();

		inline const std::vector< Player > *GetPlayers() const { return &players; }
		inline unsigned int GetNumPlayers() const { return players.size(); }

		Player *GetPlayer( unsigned int id );

	private:
		std::vector< Player > players;
	};
}// namespace ct

/**
 * Adds a brand new player to the game.
 */
unsigned int ct::PlayerManager::AddPlayer( const char *name )
{
	players.push_back( Player( name ) );
	return players.size();
}

void ct::PlayerManager::HandleInput()
{
}

/**
 * Return a pointer to the specified player.
 */
ct::PlayerManager::Player *ct::PlayerManager::GetPlayer( unsigned int id )
{
	if ( id >= players.size() )
	{
		return nullptr;
	}

	return &players[ id ];
}
