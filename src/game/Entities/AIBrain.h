/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2021, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc
{
	class AIBrain
	{
	public:
		AIBrain();
		~AIBrain();

		enum class Mood
		{
			SAD,
			NEUTRAL,
			HAPPY,

			MAX_MOODS
		};

		inline Mood GetCurrentMood() const { return myMood; }

		void Tick();

	private:
		Mood myMood{ Mood::NEUTRAL };

		struct FeedbackState
		{
			struct Need
			{
				const char *description;
				double		value;
			};
			Need food{ "food", 0.0 };
			Need water{ "water", 0.0 };
			Need health{ "health", 0.0 };
			Need movement{ "movement", 0.0 };
			Need safety{ "safety", 0.0 };
			Need rest{ "rest", 0.0 };
			Need shelter{ "shelter", 0.0 };
		};
		FeedbackState myState;

		class MemoryManager
		{
		public:
		private:
			class Memory
			{
			public:
			private:
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
			std::map< std::string, Memory > myMemories;
		};
		MemoryManager myMemoryManager;

		struct Directive
		{
			double		weight{ 0 };
			bool		isCompleted{ false };
			PLVector2	targetPosition;
			std::string description;
		};
		std::vector< Directive > myDirectives;
	};
}// namespace vc
