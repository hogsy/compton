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
