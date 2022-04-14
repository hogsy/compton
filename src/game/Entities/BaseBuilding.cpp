// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "BaseBuilding.h"

ct::BaseBuilding::BaseBuilding() {}
ct::BaseBuilding::~BaseBuilding() {}

void ct::BaseBuilding::Deserialize( Serializer *read )
{
	SuperClass::Deserialize( read );

	maxConstructionAmount_ = read->ReadInteger();
	constructionAmount_ = read->ReadInteger();
}

void ct::BaseBuilding::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( maxConstructionAmount_ );
	write->WriteInteger( constructionAmount_ );
}

void ct::BaseBuilding::Draw( const ct::Camera &camera )
{
	Entity::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}
