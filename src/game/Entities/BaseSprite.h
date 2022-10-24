// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"
#include "engine/Sprite.h"

namespace ct
{
	class BaseSprite : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseSprite, Entity )

	public:
		void Draw( const Camera &camera ) override;

	protected:
		const Sprite *sprite_{ nullptr };
	};
}// namespace ct
