/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"
#include "BaseCreature.h"

namespace vc {
#define MAX_CHARACTER_NAME 64

	class BaseCharacter : public BaseCreature {
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

		PLVector2 debugGoal;
		PLVector2 debugRoutes[ 16 ];
		unsigned int debugNumRoutes{ 0 };
		unsigned int debugCurRoute{ 0 };
		unsigned int debugGoalDelay{ 2000 };

		float myInfluence{ 0.0f };
	};
}// namespace vc

