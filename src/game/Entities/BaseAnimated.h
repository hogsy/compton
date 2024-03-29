// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"
#include "engine/SpriteAnimator.h"

#include "../Game.h"

namespace ct
{
	class BaseAnimated : public Entity
	{
	public:
		IMPLEMENT_SUPER( Entity );

		static constexpr const char *DIRECTIONS[ game::MAX_SPRITE_DIR ] = {
		        "n",
		        //"ne",
		        "e",
		        //"se",
		        "s",
		        //"sw",
		        "w",
		        //"nw",
		};

		virtual void Tick() override;
		virtual void Draw( const Camera &camera ) override;

		bool         CacheAnimationSet( const char *path );
		virtual void SetAnimation( const SpriteAnimation *animation );

	protected:
		SpriteAnimator animator_;
	};
}// namespace ct
