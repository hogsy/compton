// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
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
