// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine_entity.h"

namespace vc
{
	class Tree : public Entity
	{
		DECLARE_ENTITY_CLASS( Tree, Entity )

	public:
		void Spawn() override;

		void Draw( const Camera &camera ) override;
	};
}// namespace vc
