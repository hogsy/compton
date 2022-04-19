// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "SpriteManager.h"
#include "Entities/BaseCreature.h"

namespace ct::game::ds
{
	class HumanCreature : public BaseCreature
	{
		DECLARE_ENTITY_CLASS( HumanCreature, BaseCreature )

	public:
		void Precache() override;
		void Spawn() override;
		void Tick() override;

	private:
		unsigned int currentAnimation_{ 0 };
	};
}// namespace ct::game::ds

using namespace ct;
using namespace ct::game;

REGISTER_ENTITY( HumanCreature, ds::HumanCreature )

static const SpriteAnimation *idleAnimations[ ct::BaseAnimated::MAX_SPRITE_DIRECTIONS ] = { nullptr };
static const SpriteAnimation *runAnimations[ ct::BaseAnimated::MAX_SPRITE_DIRECTIONS ] = { nullptr };

ds::HumanCreature::HumanCreature() = default;
ds::HumanCreature::~HumanCreature() = default;

void ds::HumanCreature::Precache()
{
	SuperClass::Precache();

	ct::spriteManager->GetSpriteSheet( "sprites/creatures/human/human_sprites.sdf" );

	if ( CacheAnimationSet( "sprites/creatures/human/human.ani" ) )
	{
		if ( idleAnimations[ 0 ] == nullptr )
		{
			for ( unsigned int i = 0; i < MAX_SPRITE_DIRECTIONS; ++i )
			{
				std::string name = "human_idle_" + std::string( DIRECTIONS[ i ] );
				idleAnimations[ i ] = SpriteAnimator::GetAnimation( "sprites/creatures/human/human.ani", name.c_str() );
			}
		}
		if ( runAnimations[ 0 ] == nullptr )
		{
			for ( unsigned int i = 0; i < MAX_SPRITE_DIRECTIONS; ++i )
			{
				std::string name = "human_run_" + std::string( DIRECTIONS[ i ] );
				runAnimations[ i ] = SpriteAnimator::GetAnimation( "sprites/creatures/human/human.ani", name.c_str() );
			}
		}
	}
}

void ds::HumanCreature::Spawn()
{
	SuperClass::Spawn();

	SetAnimation( nullptr );
}

static unsigned int maxTicksTest = 50;

void ds::HumanCreature::Tick()
{
	SuperClass::Tick();

	if ( maxTicksTest <= GetApp()->GetNumOfTicks() )
	{
		SetAnimation( idleAnimations[ currentAnimation_++ ] );
		if ( currentAnimation_ >= MAX_SPRITE_DIRECTIONS )
			currentAnimation_ = 0;

		maxTicksTest = GetApp()->GetNumOfTicks() + 50;
	}
}
