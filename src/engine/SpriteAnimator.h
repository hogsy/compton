// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Sprite.h"
#include "SpriteSheet.h"

namespace vc
{
	class SpriteAnimation
	{
	public:
		SpriteAnimation() = default;
		SpriteAnimation( const std::vector< Sprite * > &sprites, unsigned int playbackSpeed = 1, bool mirror = false );
		~SpriteAnimation();

	private:
		std::vector< const Sprite * > sprites_;

		unsigned int playbackSpeed_{ 1 };

		bool mirror{ false };
		bool loop{ false };
	};

	class SpriteAnimator
	{
	};
}// namespace vc
