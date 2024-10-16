// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Boid.cpp
 *  Dumb replicant of Boid's in SFC, only possibly a bit more fleshed out.
 *------------------------------------------------------------------------------------*/

#include "../app.h"
#include "../serializer.h"

#include "Boid.h"

REGISTER_ENTITY( Boid, Boid )

Boid::Boid()  = default;
Boid::~Boid() = default;

void Boid::Spawn()
{
	SuperClass::Spawn();
}

void Boid::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	//GetApp()->GetImageManager()->DrawSprite( ImageManager::SPR_GROUP_OBJECTS_0, 6,
	//                                        ( int ) origin.x, ( int ) origin.y, true );
}

void Boid::Tick()
{
}

void Boid::Deserialize( Serializer *read )
{
	SuperClass::Deserialize( read );

	type_ = ( Type ) read->ReadInteger();
}

void Boid::Serialize( Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( ( int ) type_ );
}
