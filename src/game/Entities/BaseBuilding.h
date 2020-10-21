/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"
#include "Serializer.h"

namespace vc {
	class BaseBuilding : public Entity {
		DECLARE_ENTITY_CLASS( BaseBuilding, Entity )

	public:
		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

	protected:
	private:
	};
}
