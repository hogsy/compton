// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"
#include "../serializer.h"

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

REGISTER_ENTITY( BaseCharacter, BaseCharacter )

void BaseCharacter::Spawn()
{
	SuperClass::Spawn();
}

void BaseCharacter::Deserialize( Serializer *read )
{
	SuperClass::Deserialize( read );

	read->ReadString( name, sizeof( name ) );
}

void BaseCharacter::Serialize( Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteString( name );
}

void BaseCharacter::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}

void BaseCharacter::Tick()
{
	SuperClass::Tick();
}
