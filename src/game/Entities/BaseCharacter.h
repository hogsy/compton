// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../../engine/Entity.h"

#include "BaseCreature.h"

namespace vc
{
#define MAX_CHARACTER_NAME 64

	class BaseCharacter : public BaseCreature
	{
	public:
		IMPLEMENT_SUPER( BaseCreature )

		void Spawn() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;
		void Tick() override;

	protected:
	private:
		char name[ MAX_CHARACTER_NAME ];

		hei::Vector2 debugGoal;
		hei::Vector2 debugRoutes[ 16 ];
		unsigned int debugNumRoutes{ 0 };
		unsigned int debugCurRoute{ 0 };
		unsigned int debugGoalDelay{ 2000 };

		float myInfluence{ 0.0f };
	};
}// namespace vc
