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

#pragma once

#include "Entity.h"
#include "Serializer.h"

namespace vc
{
	class BaseBuilding : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseBuilding, Entity )

	public:
		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;

	protected:
	private:
	};
}// namespace vc
