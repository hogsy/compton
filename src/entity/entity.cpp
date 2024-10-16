// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"
#include "../serializer.h"

#include "entity.h"

Entity::Entity()  = default;
Entity::~Entity() = default;

/**
 * Called if the entity is spawning anew.
 */
void Entity::Spawn()
{
	origin = velocity = bounds = pl_vecOrigin2;
}

void Entity::Tick()
{
}

void Entity::Draw( const Camera &camera )
{
}

/**
 * Loading from pre-existing dataset.
 */
void Entity::Deserialize( Serializer *read )
{
	origin   = read->ReadCoordinate();
	velocity = read->ReadCoordinate();
	bounds   = read->ReadCoordinate();
}

/**
 * Saving.
 */
void Entity::Serialize( Serializer *write )
{
	write->WriteCoordinate( origin );
	write->WriteCoordinate( velocity );
	write->WriteCoordinate( bounds );
}

/**
 * Determine if we should display this particular entity or not.
 */
bool Entity::ShouldDraw( const Camera &camera ) const
{
	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->get_draw_height();

	if ( origin.x - bounds.x > camera.position.x + sw )
	{
		return false;
	}
	else if ( origin.y - bounds.y > camera.position.y + sh )
	{
		return false;
	}
	else if ( origin.x + bounds.x < camera.position.x - sw )
	{
		return false;
	}
	else if ( origin.y + bounds.y < camera.position.y - sh )
	{
		return false;
	}

	return true;
}
