/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/*--------------------------------------------------------------------------------------
 * Boid.cpp
 *  Dumb replicant of Boid's in SFC, only possibly a bit more fleshed out.
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "Serializer.h"
#include "Boid.h"

REGISTER_ENTITY( Boid, vc::Boid )

vc::Boid::Boid()  = default;
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
