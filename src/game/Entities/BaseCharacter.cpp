/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "BaseCharacter.h"
#include "Random.h"
#include "Serializer.h"
#include "Utility.h"

namespace vc {
	/*
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
	 */
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

	if ( !ShouldDraw( camera ) ) {
		return;
	}

	al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 0, 255, 255 ) );
	al_draw_pixel( origin.x, origin.y, al_map_rgb( 255, 0, 0 ) );

	al_draw_filled_circle( debugGoal.x, debugGoal.y, 5.0f, al_map_rgb( 255, 0, 0 ) );
	al_draw_line( origin.x, origin.y, debugGoal.x, debugGoal.y, al_map_rgb( 255, 0, 0 ), 1.0f );

	al_draw_text( GetApp()->GetDefaultFont(), al_map_rgb( 255, 255, 255 ), origin.x, origin.y, 0, name );
}

void vc::BaseCharacter::Tick() {
	SuperClass::Tick();

	// Setup a goal for us to move to.
	if ( debugGoal == 0 || origin == debugGoal || debugGoalDelay < GetApp()->GetNumOfTicks() ) {
		debugGoal.x = random::GenerateRandomInteger( origin.x - 512, origin.x + 512 );
		debugGoal.y = random::GenerateRandomInteger( origin.y - 512, origin.y + 512 );
		debugGoalDelay = GetApp()->GetNumOfTicks() + 200;
	}

	PLVector2 direction = debugGoal - origin;
	//velocity.x = ( plInOutPow( origin.x, direction.Length() ) ) + direction.x;
	//velocity.y = ( plInOutPow( origin.y, direction.Length() ) ) + direction.y;
	velocity += direction * direction.Length();
	if ( velocity.x > 1.0f ) velocity.x = 1.0f;
	if ( velocity.x < -1.0f ) velocity.x = -1.0f;
	if ( velocity.y > 1.0f ) velocity.y = 1.0f;
	if ( velocity.y < -1.0f ) velocity.y = -1.0f;

	origin += velocity;
}
