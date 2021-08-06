/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#define CAMERA_MAXSPEED		10.0f
#define CAMERA_ACCELERATION 2.0f
#define CAMERA_FRICTION		10.0f

namespace vc
{
	struct Camera
	{
		PLVector2 oldPosition{ 0.0f, 0.0f }, position{ 0.0f, 0.0f };
		PLVector2 oldVelocity{ 0.0f, 0.0f }, velocity{ 0.0f, 0.0f };
		float	  zoom{ 1.0f }, angle{ 0.0f };

		enum class MoveMode
		{
			FOLLOWING,
			MANUAL,
			FREE,
		};
		MoveMode movementMode{ MoveMode::FREE };
	};
}// namespace vc