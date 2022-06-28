// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"
#include "engine/PlayerManager.h"

#include "BaseCreature.h"

namespace ct
{
	class BaseCharacter : public BaseCreature
	{
	public:
		IMPLEMENT_SUPER( BaseCreature )

		void Spawn() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		void Draw( const Camera &camera ) override;
		void Tick() override;

		virtual bool TakeControl( int playerNum );
		virtual void ReleaseControl();

	protected:
	private:
		static constexpr unsigned int MAX_CHARACTER_NAME = 64;
		char name[ MAX_CHARACTER_NAME ];

		static constexpr unsigned int NUM_SPRITES = 5;

		int controllingPlayer_{ -1 };
		bool isPossessed_{ false };

		hei::Vector2 debugGoal;
		hei::Vector2 debugRoutes[ 16 ];
		unsigned int debugNumRoutes{ 0 };
		unsigned int debugCurRoute{ 0 };
		unsigned int debugGoalDelay{ 2000 };

		float influence_{ 0.0f };
	};
}// namespace vc
