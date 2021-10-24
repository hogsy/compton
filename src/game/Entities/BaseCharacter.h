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
#include "BaseCreature.h"

namespace vc
{
#define MAX_CHARACTER_NAME 64

	class BaseCharacter : public BaseCreature
	{
	public:
		IMPLEMENT_SUPER( BaseCreature )

		void Spawn() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;
		void Tick() override;

	protected:
	private:
		char name[ MAX_CHARACTER_NAME ];

		hei::Vector2 debugGoal;
		hei::Vector2 debugRoutes[ 16 ];
		unsigned int debugNumRoutes{ 0 };
		unsigned int debugCurRoute{ 0 };
		unsigned int debugGoalDelay{ 2000 };

		float myInfluence{ 0.0f };
	};
}// namespace vc
