/*
SimGame Engine
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

#include "SimGame.h"
#include "Entity.h"
#include "Serializer.h"

vc::Entity::Entity()
{
}

vc::Entity::~Entity()
{
}

/**
 * Called if the entity is spawning anew.
 */
void vc::Entity::Spawn()
{
	origin = velocity = bounds = pl_vecOrigin2;
}

void vc::Entity::Tick()
{
}

void vc::Entity::Draw( const Camera &camera )
{
}

/**
 * Loading from pre-existing dataset.
 */
void vc::Entity::Deserialize( Serializer *read )
{
	origin	 = read->ReadCoordinate();
	velocity = read->ReadCoordinate();
	bounds	 = read->ReadCoordinate();
}

/**
 * Saving.
 */
void vc::Entity::Serialize( Serializer *write )
{
	write->WriteCoordinate( origin );
	write->WriteCoordinate( velocity );
	write->WriteCoordinate( bounds );
}

/**
 * Determine if we should display this particular entity or not.
 */
bool vc::Entity::ShouldDraw( const vc::Camera &camera ) const
{
	if ( origin.x - bounds.x > camera.position.x + DISPLAY_WIDTH )
	{
		return false;
	}
	else if ( origin.y - bounds.y > camera.position.y + DISPLAY_HEIGHT )
	{
		return false;
	}
	else if ( origin.x + bounds.x < camera.position.x - DISPLAY_WIDTH )
	{
		return false;
	}
	else if ( origin.y + bounds.y < camera.position.y - DISPLAY_HEIGHT )
	{
		return false;
	}

	return true;
}
