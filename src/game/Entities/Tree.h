/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"

namespace vc {
	class Tree : public Entity {
		DECLARE_ENTITY_CLASS( Tree, Entity )

	public:
		void Spawn() override;

		void Draw( const Camera &camera ) override;
	};
}// namespace vc
