// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine_entity.h"
#include "../../engine/Random.h"

#include "../Entities/ai/Brain.h"
#include "../Entities/ai/Sensor.h"

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

		/**
		 * Let's us know whether this particular
		 * creature can actually get pregnant in
		 * it's current state.
		 */
		inline bool CanBecomePregnant()
		{
			if ( sex == Sex::MALE || isPregnant )
			{
				return false;
			}

			// Only adults can make babies
			if ( phase != LifePhase::LIFE_PHASE_ADULTHOOD )
			{
				return false;
			}

			return ( random::GenerateRandomInteger( 0, age / MAX_LIFE_PHASES ) == 0 );
		}

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;
		void Tick() override;

	private:
		void Think();

	public:
		// Actions
		virtual ai::FeedbackState Use();
		virtual ai::FeedbackState Drink();
		virtual ai::FeedbackState Eat();
		virtual ai::FeedbackState Talk();
		virtual ai::FeedbackState Attack();

		virtual void StepTowards( const PLVector2 &target, int speed = 5 );
		virtual void StepAway( const PLVector2 &target, int speed = 1.0f );

	public:
		enum LifePhase
		{
			LIFE_PHASE_BABYHOOD, // immobile and weak
			LIFE_PHASE_CHILDHOOD,// mobile but weak
			LIFE_PHASE_ADULTHOOD,// mobile
			LIFE_PHASE_ELDERLY,  // slower and weaker
			LIFE_PHASE_EXPIRED,  // dead

			MAX_LIFE_PHASES
		};

	protected:
		LifePhase phase{ LifePhase::LIFE_PHASE_BABYHOOD };
		Sex       sex{ Sex::INTERSEX };

		ai::Brain  brain;
		ai::Sensor sensors[ ai::Sensor::MAX_SENSOR_TYPES ];

		Entity *targetEntity{};

		unsigned int age{ 0 };
		unsigned int maxAge{ 100 };
		unsigned int generation{ 0 };

		bool         isPregnant{ false };
		unsigned int timePregnant{ 0 };

		int stamina{ 100 };
		int maxStamina{ 100 };

		int myHunger{ 0 };
		int myThirst{ 0 };

		unsigned int stepTime{};

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };
		unsigned int currentLevel{ 0 };

	public:
		[[nodiscard]] virtual bool IsMoving() const;
	};
}// namespace vc
