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
#include "BaseBuilding.h"

vc::BaseBuilding::BaseBuilding() {}
vc::BaseBuilding::~BaseBuilding() {}

void vc::BaseBuilding::Deserialize( Serializer *read )
{
	SuperClass::Deserialize( read );
}

void vc::BaseBuilding::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );
}

void vc::BaseBuilding::Draw( const vc::Camera &camera )
{
	Entity::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	al_draw_filled_rectangle( origin.x + 32, origin.y + 32, origin.x - 32, origin.y - 32, al_map_rgb( 64, 64, 64 ) );
	al_draw_text( GetApp()->GetDefaultFont(), al_map_rgb( 255, 255, 255 ), origin.x, origin.y, 0, "Building" );
}
