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

namespace ct
{
	namespace ai
	{
		enum class Verb
		{
			VERB_USE,
			VERB_DRINK,
			VERB_EAT,
			VERB_TALK,
			VERB_APPROACH,
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

			enum class Mood
			{
				SAD,
				NEUTRAL,
				HAPPY,

				MAX_MOODS
			};

			inline Mood GetCurrentMood() const { return myMood; }

			void Tick();

			//======================================================
			// MEMORY

			inline void AddMemory()
			{
			}

			inline void WipeMemory()
			{
				myMemoryManager.Wipe();
			}

		private:
			Mood myMood{ Mood::NEUTRAL };

			FeedbackState myState;

			class MemoryManager
			{
			public:
				inline void AddMemory( const std::string &descriptor, const Memory &memory )
				{
				}

				inline void Wipe()
				{
					myMemories.clear();
				}

			private:
				std::map< std::string, Memory > myMemories;
				// the above is used for carrying identifications for
				// each agent in the world. each agent carries a unique
				// descriptor for it's "classification", e.g. honey, water, ice, gremlin
				// which we use to fetch our disposition
			};
			MemoryManager myMemoryManager;

			struct Directive
			{
				double       weight{ 0 };
				bool         isCompleted{ false };
				hei::Vector2 targetPosition;
				std::string  description;
			};
			std::vector< Directive > myDirectives;
		};
	}// namespace ai
}// namespace vc
