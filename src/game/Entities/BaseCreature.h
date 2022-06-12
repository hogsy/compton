// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Entity.h"
#include "BaseAnimated.h"
#include "Inventory.h"

#include "Entities/ai/Brain.h"
#include "Entities/ai/Sensor.h"

namespace ct
{
	class BaseCreature : public BaseAnimated
	{
		DECLARE_ENTITY_CLASS( BaseCreature, BaseAnimated )

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

	private:
		void Think();

	public:
		virtual void StepTowards( const hei::Vector2 &target, float speed = 1.0f );
		virtual void StepAway( const hei::Vector2 &target, int speed = 1.0f );

	protected:
		Sex sex_{ Sex::INTERSEX };

		ai::Brain brain_;
		ai::Sensor sensors_[ ai::Sensor::MAX_SENSOR_TYPES ];

		hei::Vector2 goal_;

		Inventory inventory;

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
