// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

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
			position_.y           = ( std::sin( vc::GetApp()->GetNumOfTicks() / ( 120.0f / jiggle ) ) * 5.0f + 5.0f ) + position_.y;

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
