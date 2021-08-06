/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

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
