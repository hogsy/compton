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

#include "engine/Compton.h"
#include "engine/GameMode.h"
#include "engine/Entity.h"

namespace ct::entity
{
	class DebugWaypoint : public Entity
	{
		DECLARE_ENTITY_CLASS( DebugWaypoint, Entity )

		void Draw( const Camera &camera ) override;
	};
}// namespace ct::entity

REGISTER_ENTITY( DebugWaypoint, ct::entity::DebugWaypoint )

using namespace ct::entity;

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
