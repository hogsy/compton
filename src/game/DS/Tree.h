// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"

namespace ct::game::ds
{
	class Tree : public Entity
	{
		DECLARE_ENTITY_CLASS( Tree, Entity )

	public:
		void Spawn() override;
		void Precache() override;

		void Draw( const Camera &camera ) override;

	private:
		const Sprite *sprite_{ nullptr };
	};
}// namespace vc
