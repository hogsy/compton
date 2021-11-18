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

#pragma once

#include "Boid.h"

namespace vc
{
	class BoidManager : public Entity
	{
		DECLARE_ENTITY_CLASS( BoidManager, Entity )

	public:
		void Spawn() override;
		void Tick() override;

		//void Deserialize( Serializer *read ) override;
		//void Serialize( Serializer *write ) override;

	private:
		struct Flock
		{
			enum
			{
				MOVE_DIRECTION_LEFT,
				MOVE_DIRECTION_RIGHT,
			};
			int direction{ MOVE_DIRECTION_RIGHT };
			std::vector< Entity * > boids;
		};
		std::vector< Flock > flocks_;

		void SpawnFlock( );
		void SimulateFlock( Flock &flock );
	};
}// namespace vc
