/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Entity.h"

namespace vc {
	enum class CharacterSex {
		MALE,
		FEMALE,
		INTERSEX,
	};

	enum class CharacterMood {
		MOOD_SAD,
		MOOD_NEUTRAL,
		MOOD_HAPPY,
	};

	enum class CharacterDirective {
		IDLE, // Do nothing
		EAT,  // Search for a food source
		DRINK,// Search for water or source of drink
		DEAD, // Do nothing
		CHAT, // Chat with other characters
		MOURN,// Cry over a character that died
		SLEEP,// Find somewhere to sleep

		MAX_DIRECTIVES
	};

	class BaseCharacter : public Entity {
	public:
		IMPLEMENT_SUPER( Entity )

	protected:
	private:
		struct Directive {
			unsigned int weight{ 0 };
			bool isCompleted{ false };
			PLVector2 targetPosition;
			std::string description;
		};

		std::vector< Directive > directives;

		unsigned int age{ 0 };
		unsigned int maxAge{ 100 };

		int health{ 100 };
		int maxHealth{ 100 };

		int stamina{ 100 };
		int maxStamina{ 100 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };

		int influence{ 0 };
	};

	REGISTER_ENTITY( character_base, BaseCharacter )
}// namespace vc
