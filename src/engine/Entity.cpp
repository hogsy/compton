// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "Serializer.h"

ct::Entity::Entity() = default;
ct::Entity::~Entity() = default;

/**
 * Called if the entity is spawning anew.
 */
void ct::Entity::Spawn()
{
}

void ct::Entity::Tick()
{
}

void ct::Entity::Draw( const Camera &camera )
{
}

/**
 * Loading from pre-existing dataset.
 */
void ct::Entity::Deserialize( Serializer *read )
{
	origin   = read->ReadCoordinate();
	velocity = read->ReadCoordinate();
	bounds   = read->ReadCoordinate();
}

/**
 * Saving.
 */
void ct::Entity::Serialize( Serializer *write )
{
	write->WriteCoordinate( origin );
	write->WriteCoordinate( velocity );
	write->WriteCoordinate( bounds );
}

/**
 * Determine if we should display this particular entity or not.
 */
bool ct::Entity::ShouldDraw( const ct::Camera &camera ) const
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
