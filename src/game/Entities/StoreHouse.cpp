/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "BaseBuilding.h"

namespace vc
{
	class StoreHouse : public BaseBuilding
	{
		DECLARE_ENTITY_CLASS( StoreHouse, BaseBuilding )
	};
}// namespace vc

REGISTER_ENTITY( StoreHouse, vc::StoreHouse )

vc::StoreHouse::StoreHouse() {}
vc::StoreHouse::~StoreHouse() {}
