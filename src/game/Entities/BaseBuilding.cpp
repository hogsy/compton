/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

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
