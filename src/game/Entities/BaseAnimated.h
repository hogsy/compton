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

		virtual void Tick() override;
		virtual void Draw( const Camera &camera ) override;

		void CacheAnimation( const char *path );
		virtual void SetAnimation( const char *name );

	protected:
		SpriteAnimator animator_;
	};
}// namespace vc
