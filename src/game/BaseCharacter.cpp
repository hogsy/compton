/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Entity.h"
#include "Serializer.h"

namespace vc {
	enum class CharacterSex {
		MALE,
		FEMALE,
		INTERSEX,

		MAX_SEXES
	};

	enum class CharacterMood {
		MOOD_SAD,
		MOOD_NEUTRAL,
		MOOD_HAPPY,

		MAX_MOODS
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

		bool CanBreed( BaseCharacter *other );

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

	protected:
	private:
		struct Directive {
			unsigned int weight{ 0 };
			bool isCompleted{ false };
			PLVector2 targetPosition;
			std::string description;
		};

		std::vector< Directive > directives;

		CharacterMood mood{ CharacterMood::MOOD_NEUTRAL };
		CharacterSex sex{ CharacterSex::INTERSEX };

		unsigned int age{ 0 };
		unsigned int maxAge{ 100 };

		bool isPregnant{ false };
		unsigned int timePregnant{ 0 };

		int health{ 100 };
		int maxHealth{ 100 };

		int stamina{ 100 };
		int maxStamina{ 100 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };

		int influence{ 0 };
	};
}// namespace vc

REGISTER_ENTITY( character_base, vc::BaseCharacter )

/**
 * Determines whether or not this character can breed with the other.
 */
bool vc::BaseCharacter::CanBreed( BaseCharacter *other ) {
	// Can't get pregnant if we're already pregnant!
	if ( isPregnant || other->isPregnant ) {
		return false;
	}

	switch( sex ) {
		default:
			break;
		case CharacterSex::INTERSEX:
			return true;
		case CharacterSex::MALE:
			if ( other->sex == CharacterSex::FEMALE ) {
				return true;
			}
			break;
		case CharacterSex::FEMALE:
			if ( other->sex == CharacterSex::MALE ) {
				return true;
			}
			break;
	}

	return false;
}

void vc::BaseCharacter::Deserialize( vc::Serializer *read ) {
	SuperClass::Deserialize( read );

	health = read->ReadInteger();
	maxHealth = read->ReadInteger();

	stamina = read->ReadInteger();
	maxStamina = read->ReadInteger();

	experience = read->ReadInteger();
	maxExperience = read->ReadInteger();
}

void vc::BaseCharacter::Serialize( vc::Serializer *write ) {
	SuperClass::Serialize( write );

	write->WriteInteger( health );
	write->WriteInteger( maxHealth );

	write->WriteInteger( stamina );
	write->WriteInteger( maxStamina );

	write->WriteInteger( experience );
	write->WriteInteger( maxExperience );
}
