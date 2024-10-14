// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Boid.cpp
 *  Dumb replicant of Boid's in SFC, only possibly a bit more fleshed out.
 *------------------------------------------------------------------------------------*/

#include "../../engine/Compton.h"
#include "../../engine/Serializer.h"

#include "Boid.h"

REGISTER_ENTITY( Boid, vc::Boid )

vc::Boid::Boid() = default;
vc::Boid::~Boid() = default;

void vc::Boid::Spawn()
{
	SuperClass::Spawn();
}

void vc::Boid::Draw( const vc::Camera &camera )
{
	SuperClass::Draw( camera );

	GetApp()->GetImageManager()->DrawSprite( ImageManager::SPR_GROUP_OBJECTS_0, 6,
	                                         ( int ) origin.x, ( int ) origin.y, true );
}

void vc::Boid::Tick()
{
}

void vc::Boid::Deserialize( vc::Serializer *read )
{
	SuperClass::Deserialize( read );

	type_ = ( Type ) read->ReadInteger();
}

void vc::Boid::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( ( int ) type_ );
}
