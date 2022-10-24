// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Serializer.h"

#define CAMERA_MAXSPEED     10.0f
#define CAMERA_ACCELERATION 2.0f
#define CAMERA_FRICTION     10.0f

namespace ct
{
	class Entity;
	struct Camera : public ISerializable
	{
		hei::Vector2 oldPosition{ 0.0f, 0.0f }, position{ 0.0f, 0.0f };
		hei::Vector2 oldVelocity{ 0.0f, 0.0f }, velocity{ 0.0f, 0.0f };

		enum class MoveMode
		{
			FOLLOWING,
			FREE,
			OTHER,
		};
		MoveMode movementMode{ MoveMode::FREE };

		Entity *followTarget{ nullptr };

		inline void Deserialize( Serializer *read ) override
		{
			oldPosition  = read->ReadCoordinate();
			position     = read->ReadCoordinate();
			oldVelocity  = read->ReadCoordinate();
			velocity     = read->ReadCoordinate();
			movementMode = ( MoveMode ) read->ReadI32();
		}
		inline void Serialize( Serializer *write ) override
		{
			write->WriteCoordinate( oldPosition );
			write->WriteCoordinate( position );
			write->WriteCoordinate( oldVelocity );
			write->WriteCoordinate( velocity );
			write->WriteI32( ( int ) movementMode );
		}
	};
}// namespace ct
