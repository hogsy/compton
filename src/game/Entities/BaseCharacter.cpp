/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Entity.h"
#include "Random.h"
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

#define MAX_CHARACTER_NAME 64

	class BaseCharacter : public Entity {
	public:
		IMPLEMENT_SUPER( Entity )

		void Spawn() override;

		bool CanBreed( BaseCharacter *other );

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw() override;

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

		char name[ MAX_CHARACTER_NAME ];

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
		unsigned int currentLevel{ 0 };

		int influence{ 0 };
	};
}// namespace vc

REGISTER_ENTITY( character_base, vc::BaseCharacter )

void vc::BaseCharacter::Spawn() {
	static const char *names[]={
			"Adelmarus",
	        "Arnaldus",
	        "Carola",
	        "Harmonia",
	        "Harrius",
	        "Hilarius",
	        "Michaelis",
	        "Victoria",
	        "Zacharias",
	};
	const char *firstName = names[ random::GenerateRandomInteger( 0, plArrayElements( names ) ) ];
	const char *lastName = names[ random::GenerateRandomInteger( 0, plArrayElements( names ) ) ];
	snprintf( name, sizeof( name ), "%s %s", firstName, lastName );

	// Age
	age = random::GenerateRandomInteger( 1, 50 );
	maxAge = random::GenerateRandomInteger( age, age + 50 );

	// Health
	health = random::GenerateRandomInteger( 50, 100 );
	maxHealth = random::GenerateRandomInteger( health, health + 100 );

	// Stamina
	stamina = random::GenerateRandomInteger( 30, 100 );
	maxStamina = random::GenerateRandomInteger( stamina, stamina + 100 );

	// Generate a random sex for the character
	sex = static_cast< CharacterSex >( random::GenerateRandomInteger( 0, static_cast< int >( CharacterSex::MAX_SEXES ) ) );
	// Initial mood should also initially be randomised
	mood = static_cast< CharacterMood >( random::GenerateRandomInteger( 0, static_cast< int >( CharacterMood::MAX_MOODS ) ) );
}

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

	read->ReadString( name, sizeof( name ) );

	health = read->ReadInteger();
	maxHealth = read->ReadInteger();

	stamina = read->ReadInteger();
	maxStamina = read->ReadInteger();

	experience = read->ReadInteger();
	maxExperience = read->ReadInteger();
}

void vc::BaseCharacter::Serialize( vc::Serializer *write ) {
	SuperClass::Serialize( write );

	write->WriteString( name );

	write->WriteInteger( health );
	write->WriteInteger( maxHealth );

	write->WriteInteger( stamina );
	write->WriteInteger( maxStamina );

	write->WriteInteger( experience );
	write->WriteInteger( maxExperience );
}

void vc::BaseCharacter::Draw() {
	if ( !isVisible ) {
		return;
	}

	al_draw_filled_circle( 0, 0, 0.5f, al_map_rgb( 0, 255, 0 ) );
}
