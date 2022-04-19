// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Entity.h"
#include "SpriteAnimator.h"

namespace ct
{
	class BaseAnimated : public Entity
	{
	public:
		IMPLEMENT_SUPER( Entity );

		static constexpr unsigned int MAX_SPRITE_DIRECTIONS = 8;// n, ne, e, se, s, sw, w, nw
		static constexpr const char *DIRECTIONS[ MAX_SPRITE_DIRECTIONS ] = {
				"n",
				"ne",
				"e",
				"se",
				"s",
				"sw",
				"w",
				"nw",
		};

		virtual void Tick() override;
		virtual void Draw( const Camera &camera ) override;

		bool CacheAnimationSet( const char *path );
		virtual void SetAnimation( const SpriteAnimation *animation );

	protected:
		SpriteAnimator animator_;
	};
}// namespace ct
