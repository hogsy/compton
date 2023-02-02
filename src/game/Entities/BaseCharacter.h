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
		char                          name[ MAX_CHARACTER_NAME ];

		int  controllingPlayer_{ -1 };
		bool isPossessed_{ false };

		float influence_{ 0.0f };

	public:
		bool IsMoving() const override;
	};
}// namespace ct
