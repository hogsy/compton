// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Game.h"

#include "BaseSprite.h"

ct::BaseSprite::BaseSprite() = default;
ct::BaseSprite::~BaseSprite() = default;

void ct::BaseSprite::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( sprite_ == nullptr )
		return;

	if ( !ShouldDraw( camera ) )
		return;

	int tx, ty;
	render::TransformToIso( origin_.x + offset_.x, origin_.y + offset_.y, &tx, &ty );

	tx -= camera.position.x;
	ty -= camera.position.y;

	sprite_->Draw( tx, ty );
}
