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

#pragma once

#include "Entity.h"
#include "AIBrain.h"

namespace vc
{
	class BaseCreature : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseCreature, Entity )

	public:
		enum class Sex
		{
			MALE,
			FEMALE,
			INTERSEX,

			MAX_SEXES
		};

		virtual void Spawn() override;

		bool CanBreed( BaseCreature *other );

		virtual void Deserialize( Serializer *read ) override;
		virtual void Serialize( Serializer *write ) override;

		virtual void Draw( const Camera &camera ) override;
		virtual void Tick() override;

	protected:
		Sex mySex{ Sex::INTERSEX };

		AIBrain myBrain;

		unsigned int age{ 0 };
		unsigned int maxAge{ 100 };
		unsigned int generation{ 0 };

		bool		 isPregnant{ false };
		unsigned int timePregnant{ 0 };

		int stamina{ 100 };
		int maxStamina{ 100 };

		int myHunger{ 0 };
		int myThirst{ 0 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };
		unsigned int currentLevel{ 0 };
	};
}// namespace vc
