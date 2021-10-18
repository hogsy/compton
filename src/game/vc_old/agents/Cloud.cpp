/*
SimGame Engine
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

#include "../shared.h"
#include "SpriteAgent.h"

namespace vc
{
	class Cloud : public SpriteAgent
	{
		IMPLEMENT_SUPER( SpriteAgent )

		void Draw() override
		{
			PLVector2 oldPosition = position_;
			position_.y			  = ( std::sin( vc::GetApp()->GetNumOfTicks() / ( 120.0f / jiggle ) ) * 5.0f + 5.0f ) + position_.y;

			//position_.x -= game.camera_x;
			//position_.y -= game.camera_y;

			SpriteAgent::Draw();

			position_ = oldPosition;
		}

		void Move( float speed )
		{
			position_.x += speed / damping;
			angle = std::cos( sAngle++ / 1000.0f ) * 0.05f;
		}

	private:
		float  damping{ 0 }, jiggle{ 0 };
		double sAngle;
	};
}// namespace vc
