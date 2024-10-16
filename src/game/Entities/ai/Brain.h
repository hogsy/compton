// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine_entity.h"

namespace vc::ai
{
	enum class MotorAction
	{
		USE,
		DRINK,
		EAT,
		TALK,
		ATTACK,
		APPROACH,
		RETREAT,
	};

	struct DecisionTree
	{
	};

	class Concept
	{
	};

	struct FeedbackState
	{
		struct Need
		{
			const char *description;
			double      value;
		};
		Need food{ "food", 0.0 };
		Need water{ "water", 0.0 };
		Need health{ "health", 0.0 };
		Need movement{ "movement", 0.0 };
		Need pleasure{ "pleasure", 0.0 };
		Need safety{ "safety", 0.0 };
		Need rest{ "rest", 0.0 };
		Need shelter{ "shelter", 0.0 };
	};

	struct Memory
	{
		enum class Disposition : uint8_t
		{
			NEUTRAL,
			POSITIVE,
			NEGATIVE,
		};
		Disposition genericDisposition{ Disposition::NEUTRAL };

		unsigned int  curResponses{ 0 };
		FeedbackState mappedResponses[ 8 ];
		FeedbackState averageResponse;
	};

	class Brain
	{
	public:
		Brain();
		~Brain();

		bool active{ true };// if false, brain will not 'tick'

		// Mood indicator, provided as a generic reflectance
		// of internal state - not necessarily accurately either

		enum class Mood
		{
			SAD,
			NEUTRAL,
			HAPPY,

			MAX_MOODS
		};

		inline Mood GetCurrentMood() const { return mood_; }

		void Tick();

		//======================================================
		// MEMORY

	private:
		Mood mood_{ Mood::NEUTRAL };

		FeedbackState feedbackState_;

		class MemoryManager
		{
		public:
			inline void AddMemory( const std::string &descriptor, const Memory &memory )
			{
			}

			inline void Wipe()
			{
				memories_.clear();
			}

		private:
			std::map< std::string, Memory > memories_;
			// the above is used for carrying identifications for
			// each agent in the world. each agent carries a unique
			// descriptor for its "classification", e.g. honey, water, ice, gremlin
			// which we use to fetch our disposition
		};
		MemoryManager memoryManager_;

	public:
		MemoryManager *GetMemoryManager() { return &memoryManager_; }

		//======================================================

	public:
		Entity *owner_{ nullptr };

		//======================================================
		// DIRECTIVES

		struct Directive
		{
			MotorAction              type;
			double                   weight{ 0 };
			bool                     isCompleted{ false };
			PLVector2                targetPosition;
			std::string              description;
			std::vector< Directive > subDirectives;
		};

	private:
		std::vector< Directive > directives_;

	public:
		[[nodiscard]] const Directive *GetTopDirective() const
		{
			return &directives_[ 0 ];
		}
	};
}// namespace vc::ai
