// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#define CAMERA_MAXSPEED     10.0f
#define CAMERA_ACCELERATION 2.0f
#define CAMERA_FRICTION     10.0f

namespace vc
{
	struct Camera
	{
		hei::Vector2 oldPosition{ 0.0f, 0.0f }, position{ 0.0f, 0.0f };
		hei::Vector2 oldVelocity{ 0.0f, 0.0f }, velocity{ 0.0f, 0.0f };
		float        zoom{ 1.0f }, angle{ 0.0f };

		enum class MoveMode
		{
			FOLLOWING,
			MANUAL,
			FREE,
		};
		MoveMode movementMode{ MoveMode::FREE };
	};
}// namespace vc