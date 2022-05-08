// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "BaseBuilding.h"

ct::BaseBuilding::BaseBuilding() {}
ct::BaseBuilding::~BaseBuilding() {}

void ct::BaseBuilding::Deserialize( Serializer *read )
{
	SuperClass::Deserialize( read );

	maxConstructionAmount_ = read->ReadI32();
	constructionAmount_ = read->ReadI32();
}

void ct::BaseBuilding::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteI32( maxConstructionAmount_ );
	write->WriteI32( constructionAmount_ );
}

void ct::BaseBuilding::Draw( const ct::Camera &camera )
{
	Entity::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}
