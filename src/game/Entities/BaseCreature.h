// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"
#include "engine/Random.h"

#include "BaseAnimated.h"

#include "../Inventory.h"

#include "../ALife/Brain.h"
#include "../ALife/Sensor.h"

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

		/**
		 * Let's us know whether this particular
		 * creature can actually get pregnant in
		 * it's current state.
		 */
		inline bool CanBecomePregnant()
		{
			if ( sex_ == Sex::MALE || isPregnant_ )
				return false;

			// Only adults can make babies
			if ( phase_ != LifePhase::LIFE_PHASE_ADULTHOOD )
				return false;

			return ( random::GenerateRandomInteger( 0, age_ / MAX_LIFE_PHASES ) == 0 );
		}

		virtual void Deserialize( Serializer *read ) override;
		virtual void Serialize( Serializer *write ) override;

		virtual void Draw( const Camera &camera ) override;
		virtual void Tick() override;

	private:
		void Think();

	public:
		// Actions
		virtual ai::FeedbackState Use();
		virtual ai::FeedbackState Drink();
		virtual ai::FeedbackState Eat();
		virtual ai::FeedbackState Talk();
		virtual ai::FeedbackState Attack();

		virtual void StepTowards( const math::Vector2 &target, int speed = 5 );
		virtual void StepAway( const math::Vector2 &target, int speed = 1.0f );

	protected:
		Sex sex_{ Sex::INTERSEX };

		ai::Brain  brain_;
		ai::Sensor sensors_[ ai::Sensor::MAX_SENSOR_TYPES ];

		hei::Vector2 goal_;

		Inventory inventory;

		Entity *targetEntity_{ nullptr };

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
		LifePhase    phase_{ LifePhase::LIFE_PHASE_BABYHOOD };
		unsigned int age_{ 0 };
		unsigned int maxAge_{ 100 };
		unsigned int generation_{ 0 };

		bool         isPregnant_{ false };
		unsigned int timePregnant_{ 0 };

		int stamina_{ 100 };
		int maxStamina_{ 100 };

		int hunger_{ 0 };
		int thirst_{ 0 };

		unsigned int stepTime_{ 0 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };
		unsigned int currentLevel{ 0 };

	public:
		[[nodiscard]] virtual bool IsMoving() const;
	};
}// namespace ct
