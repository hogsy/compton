// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Sprite.h"
#include "SpriteSheet.h"

namespace ct
{
	class SpriteAnimator
	{
	public:
		SpriteAnimator() = default;

		bool LoadFile( const char *path );

	private:
		bool ParseFile( const char *buffer );

	public:
		void SetAnimation( const char *name );

		void Tick();
		void Draw( const hei::Vector2 &position );

	private:
		struct SpriteAnimation
		{
			struct Frame
			{
				const Sprite *sprite{ nullptr };
				bool mirror{ false };
				hei::Vector2 origin;
			};

			std::vector< Frame > frames;
			unsigned int currentFrame{ 0 };
			unsigned int playbackSpeed{ 0 }, nextFrameTime{ 0 };
			bool loop{ false };
		};

		// < filename, < animation name, animation > >
		static std::map< std::string, std::map< std::string, SpriteAnimation > > cachedAnimations_;

		std::map< std::string, SpriteAnimation > animations_;
		SpriteAnimation *currentAnimation_{ nullptr };
	};
}// namespace vc
