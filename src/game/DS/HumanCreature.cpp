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
	};
}// namespace vc::game::ds

using namespace ct;
using namespace ct::game;

REGISTER_ENTITY( HumanCreature, ds::HumanCreature )

ds::HumanCreature::HumanCreature() = default;
ds::HumanCreature::~HumanCreature() = default;

void ds::HumanCreature::Precache()
{
	SuperClass::Precache();

	ct::spriteManager->GetSpriteSheet( "sprites/creatures/human/human_sprites.sdf" );

	CacheAnimation( "sprites/creatures/human/human.ani" );
}

void ds::HumanCreature::Spawn()
{
	SuperClass::Spawn();

	CacheAnimation( "sprites/creatures/human/human.ani" );
	SetAnimation( "human_idle_s" );
}

static unsigned int maxTicksTest = 50;
static const char *animations[] = {
		"human_idle_n",
		"human_idle_ne",
		"human_idle_e",
		"human_idle_se",
		"human_idle_s",
		"human_idle_sw",
		"human_idle_w",
		"human_idle_nw",
		"human_run_n",
		"human_run_ne",
		"human_run_e",
		"human_run_se",
		"human_run_s",
		"human_run_sw",
		"human_run_w",
		"human_run_nw",
};
static unsigned int currentAnimation = 0;
static constexpr unsigned int MAX_ANIMATIONS = PL_ARRAY_ELEMENTS( animations );

void ds::HumanCreature::Tick()
{
	SuperClass::Tick();

	if ( maxTicksTest <= GetApp()->GetNumOfTicks() )
	{
		SetAnimation( animations[ currentAnimation++ ] );
		if ( currentAnimation >= MAX_ANIMATIONS )
			currentAnimation = 0;

		maxTicksTest = GetApp()->GetNumOfTicks() + 50;
	}
}
