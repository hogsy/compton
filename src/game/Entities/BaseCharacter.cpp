// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "engine_private.h"
#include "../../engine/Serializer.h"

#include "BaseCharacter.h"

namespace vc
{
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

void vc::BaseCharacter::Spawn()
{
	SuperClass::Spawn();
}

void vc::BaseCharacter::Deserialize( vc::Serializer *read )
{
	SuperClass::Deserialize( read );

	read->ReadString( name, sizeof( name ) );
}

void vc::BaseCharacter::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteString( name );
}

void vc::BaseCharacter::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}

void vc::BaseCharacter::Tick()
{
	SuperClass::Tick();
}
