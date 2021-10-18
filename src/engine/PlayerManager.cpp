/*
SimGame Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SimGame.h"
#include "Entity.h"

namespace vc
{
	class PlayerManager
	{
	public:
		struct Player
		{
			Player( const std::string &sName ) : name( sName ) {}

			std::string name;
			Entity *	controlTarget{ nullptr };
		};

		unsigned int AddPlayer( const char *name );
		void		 RemovePlayer( unsigned int id );

		void HandleInput();

		typedef std::vector< Player > PlayerList;
		inline const PlayerList *	  GetPlayers() const { return &players; }
		inline unsigned int			  GetNumPlayers() const { return players.size(); }

		Player *GetPlayer( unsigned int id );

	private:
		PlayerList players;
	};
}// namespace vc

/**
 * Adds a brand new player to the game.
 */
unsigned int vc::PlayerManager::AddPlayer( const char *name )
{
	players.push_back( Player( name ) );
	return players.size();
}

void vc::PlayerManager::HandleInput()
{
}

/**
 * Return a pointer to the specified player.
 */
vc::PlayerManager::Player *vc::PlayerManager::GetPlayer( unsigned int id )
{
	if ( id >= players.size() )
	{
		return nullptr;
	}

	return &players[ id ];
}
