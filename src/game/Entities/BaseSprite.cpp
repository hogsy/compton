// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Game.h"

#include "BaseSprite.h"

ct::BaseSprite::BaseSprite()  = default;
ct::BaseSprite::~BaseSprite() = default;

void ct::BaseSprite::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( sprite_ == nullptr )
		return;

	if ( !ShouldDraw( camera ) )
		return;

	math::Vector2 iso = origin_.ToIso();
	iso.x -= ( int ) camera.position.x;
	iso.y -= ( int ) camera.position.y;

	sprite_->Draw( iso.x, iso.y );
}
