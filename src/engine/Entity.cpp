/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

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
