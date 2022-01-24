// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Entity.h"
#include "SpriteAnimator.h"

namespace vc
{
	class BaseAnimated : public Entity
	{
	public:
		IMPLEMENT_SUPER( Entity );

	protected:
		SpriteAnimator animator_;
	};
}// namespace vc
