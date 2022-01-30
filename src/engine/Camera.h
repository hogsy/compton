// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Serializer.h"

#define CAMERA_MAXSPEED     10.0f
#define CAMERA_ACCELERATION 2.0f
#define CAMERA_FRICTION     10.0f

namespace vc
{
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

		inline void Deserialize( Serializer *read )
		{
			oldPosition = read->ReadCoordinate();
			position = read->ReadCoordinate();
			oldVelocity = read->ReadCoordinate();
			velocity = read->ReadCoordinate();
			movementMode = ( MoveMode ) read->ReadInteger();
		}
		inline void Serialize( Serializer *write )
		{
			write->WriteCoordinate( oldPosition );
			write->WriteCoordinate( position );
			write->WriteCoordinate( oldVelocity );
			write->WriteCoordinate( velocity );
			write->WriteInteger( ( int ) movementMode );
		}
	};
}// namespace vc