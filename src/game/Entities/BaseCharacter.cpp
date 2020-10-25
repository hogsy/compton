/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "BaseCreature.h"
#include "Random.h"
#include "Serializer.h"
#include "Utility.h"

namespace vc {
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

	class BaseCharacter : public BaseCreature {
	public:
		IMPLEMENT_SUPER( BaseCreature )

		void Spawn() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;
		void Tick() override;

	protected:
	private:
		char name[ MAX_CHARACTER_NAME ];

		float myInfluence{ 0.0f };
	};
}// namespace vc

REGISTER_ENTITY( BaseCharacter, vc::BaseCharacter )

void vc::BaseCharacter::Spawn() {
	SuperClass::Spawn();

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
	snprintf( name, sizeof( name ), "%s %s %s", firstName, lastName, UTIL_GetRomanNumeralForNum( generation ) );
}

void vc::BaseCharacter::Deserialize( vc::Serializer *read ) {
	SuperClass::Deserialize( read );

	read->ReadString( name, sizeof( name ) );
}

void vc::BaseCharacter::Serialize( vc::Serializer *write ) {
	SuperClass::Serialize( write );

	write->WriteString( name );
}

void vc::BaseCharacter::Draw( const Camera &camera ) {
	SuperClass::Draw( camera );

	al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 0, 255, 255 ) );
	al_draw_pixel( origin.x, origin.y, al_map_rgb( 255, 0, 0 ) );
}

void vc::BaseCharacter::Tick() {
	SuperClass::Tick();
}
