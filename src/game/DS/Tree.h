// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../Entities/BaseSprite.h"

namespace ct::game::ds
{
	class Tree : public BaseSprite
	{
		DECLARE_ENTITY_CLASS( Tree, BaseSprite )

	public:
		void Spawn() override;
		void Precache() override;
	};
}// namespace ct::game::ds
