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
#include "Entities/ai/Brain.h"
#include "Entities/ai/Sensor.h"

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
		Sex sex_{ Sex::INTERSEX };

		ai::Brain brain_;
		ai::Sensor sensors_[ ai::Sensor::MAX_SENSOR_TYPES ];

		unsigned int age_{ 0 };
		unsigned int maxAge_{ 100 };
		unsigned int generation_{ 0 };

		bool isPregnant_{ false };
		unsigned int timePregnant_{ 0 };

		int stamina_{ 100 };
		int maxStamina_{ 100 };

		int hunger_{ 0 };
		int thirst_{ 0 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };
		unsigned int currentLevel{ 0 };
	};
}// namespace vc
