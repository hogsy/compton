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

#include "Entity.h"

namespace vc
{
	class Boid : public Entity
	{
		DECLARE_ENTITY_CLASS( Boid, Entity )

	public:
		enum class Type
		{
			SMALL_BIRD,
			BIG_BIRD,

			NUM_TYPES
		};

		void Spawn() override;
		void Draw( const Camera &camera ) override;
		void Tick() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

	private:
		static constexpr unsigned int numFrames[ ( unsigned int ) Type::NUM_TYPES ] =
				{
						3,// small bird
						4 // big bird
				};

		enum
		{
			MOVE_DIRECTION_LEFT,
			MOVE_DIRECTION_RIGHT,
		};
		int moveDirection_{ MOVE_DIRECTION_RIGHT };

		Type type_{ Type::SMALL_BIRD };
	};
}// namespace vc
