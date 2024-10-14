// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "Serializer.h"

vc::Entity::Entity() = default;
vc::Entity::~Entity() = default;

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
	origin   = read->ReadCoordinate();
	velocity = read->ReadCoordinate();
	bounds   = read->ReadCoordinate();
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
	int sw = vc::GetApp()->GetDrawWidth();
	int sh = vc::GetApp()->GetDrawHeight();

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
