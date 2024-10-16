// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "game_mode.h"

#include "engine_entity.h"

namespace vc::entity
{
	class DebugWaypoint : public Entity
	{
		DECLARE_ENTITY_CLASS( DebugWaypoint, Entity )

		void Draw( const Camera &camera ) override;
	};
}// namespace vc::entity

REGISTER_ENTITY( DebugWaypoint, vc::entity::DebugWaypoint )

using namespace vc::entity;

DebugWaypoint::DebugWaypoint() {}
DebugWaypoint::~DebugWaypoint() {}

void DebugWaypoint::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	// Draw a lil flag so we know it's there...
	//al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 255, 0, 255 ) );
}
