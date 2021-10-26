/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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