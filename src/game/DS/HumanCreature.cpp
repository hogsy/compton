// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "SpriteManager.h"
#include "Entities/BaseCreature.h"

namespace vc::game::ds
{
	class HumanCreature : public BaseCreature
	{
		DECLARE_ENTITY_CLASS( HumanCreature, BaseCreature )

	public:
		void Precache() override;
	};
}// namespace vc::game::ds

using namespace vc;
using namespace vc::game;

REGISTER_ENTITY( HumanCreature, ds::HumanCreature )

ds::HumanCreature::HumanCreature() = default;
ds::HumanCreature::~HumanCreature() = default;

void ds::HumanCreature::Precache()
{
	SuperClass::Precache();

	vc::spriteManager->GetSpriteSheet( "sprites/creatures/human/human_sprites.sdf" );
}
