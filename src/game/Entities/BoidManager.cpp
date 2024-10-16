/*
Compton, 2D Game Engine
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
/*--------------------------------------------------------------------------------------
 * BoidManager.cpp
 *  It manages boids...
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "Serializer.h"
#include "GameMode.h"
#include "Boid.h"

namespace vc
{
	class BoidManager : public Entity
	{
		DECLARE_ENTITY_CLASS( BoidManager, Entity )

	public:
		void Spawn() override;
		void Tick() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

	private:
		struct Flock
		{
			enum
			{
				MOVE_DIRECTION_LEFT,
				MOVE_DIRECTION_RIGHT,
			};
			int                     direction{ MOVE_DIRECTION_RIGHT };
			std::vector< Entity * > boids;
			GameMode::Room         *room;
		};
		std::vector< Flock > flocks_;

		void SpawnFlock( const GameMode::Room *room );
		void SimulateFlock( Flock &flock );
	};
}// namespace vc

REGISTER_ENTITY( BoidManager, vc::BoidManager )

vc::BoidManager::BoidManager()  = default;
vc::BoidManager::~BoidManager() = default;

void vc::BoidManager::Spawn()
{
	SuperClass::Spawn();

	// Go ahead and determine where each flock will be spawned
	unsigned int          i = 0;
	const GameMode::Room *room;
	while ( ( room = App::GetGameMode()->GetRoomByType( GameMode::Room::Type::ROOM_TYPE_EXTERIOR, i++ ) ) != nullptr )
	{
		// Spawn the flock
		SpawnFlock( room );
	}
}

void vc::BoidManager::Tick()
{
	SuperClass::Tick();

	for ( auto i : flocks_ )
	{
		SimulateFlock( i );
	}
}

void vc::BoidManager::Deserialize( vc::Serializer *read )
{
	SuperClass::Deserialize( read );
}

void vc::BoidManager::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( ( int ) flocks_.size() );
	for ( const auto &i : flocks_ )
	{
		write->WriteInteger( i.direction );
		write->WriteInteger( ( int ) i.room->id );
	}
}

void vc::BoidManager::SpawnFlock( const vc::GameMode::Room *room )
{
}

void vc::BoidManager::SimulateFlock( vc::BoidManager::Flock &flock )
{
}
