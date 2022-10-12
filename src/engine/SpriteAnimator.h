// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Sprite.h"
#include "SpriteSheet.h"

namespace ct
{
	struct SpriteAnimation
	{
		struct Frame
		{
			const Sprite *sprite{ nullptr };
			bool mirror{ false };
			hei::Vector2 origin;
		};

		std::vector< Frame > frames;
		unsigned int playbackSpeed{ 0 };
		bool loop{ false };
	};

	class SpriteAnimator
	{
	public:
		SpriteAnimator() = default;

		static bool CacheAnimationSet( const char *path );
		static inline const SpriteAnimation *GetAnimation( const char *path, const std::string &animationName )
		{
			auto i = animationSets_.find( path );
			if ( i == animationSets_.end() )
			{
				Warning( "Failed to find animation set: %s\n", path );
				return nullptr;
			}

			auto j = i->second.find( animationName );
			if ( j == i->second.end() )
			{
				Warning( "Failed to find animation: %s\n", path );
				return nullptr;
			}

			return &j->second;
		}

	private:
		static std::map< std::string, SpriteAnimation > ParseFile( const char *buffer );

	public:
		void SetAnimation( const SpriteAnimation *animation );

		void Tick();
		void Draw( int x, int y );

	public:
		const SpriteAnimation::Frame *GetCurrentFrame() const
		{
			if ( animation_ == nullptr )
				return nullptr;

			if ( frame_ >= animation_->frames.size() )
				return nullptr;

			return &animation_->frames[ frame_ ];
		}

	private:
		// < filename, < animation name, animation > >
		static std::map< std::string, std::map< std::string, SpriteAnimation > > animationSets_;

		const SpriteAnimation *animation_{ nullptr };
		unsigned int nextFrameTime_{ 0 };
		unsigned int frame_{ 0 };
	};
}// namespace ct
