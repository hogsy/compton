/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "BaseBuilding.h"

void vc::BaseBuilding::Deserialize( Serializer *read ) {
	SuperClass::Deserialize( read );
}

void vc::BaseBuilding::Serialize( vc::Serializer *write ) {
	SuperClass::Serialize( write );
}
