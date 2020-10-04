/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Entity.h"

namespace vc {
	class PlayerManager {
	public:
		struct Player {
			char name[ 64 ];
			Entity *controlTarget{ nullptr };
		};

		unsigned int AddPlayer( const char *name );
		void RemovePlayer( unsigned int id );

		void HandleInput();

		typedef std::vector< Player > PlayerList;
		const PlayerList *GetPlayers() const { return &players; }
		unsigned int GetNumPlayers() const { return players.size(); }

	private:
		PlayerList players;
	};
}
