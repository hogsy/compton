// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "engine/Compton.h"

#include "BaseAnimated.h"

using namespace ct;

REGISTER_ENTITY( BaseAnimated, BaseAnimated )

void BaseAnimated::Tick()
{
	SuperClass::Tick();

	animator_.Tick();
}

void BaseAnimated::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
		return;

	math::Vector2 iso = origin_.ToIso();
	animator_.Draw( ( iso.x + ( int ) offset_.x ) - ( int ) camera.position.x,
	                ( iso.y + ( int ) offset_.y ) - ( int ) camera.position.y );
}

bool BaseAnimated::CacheAnimationSet( const char *path )
{
	if ( !SpriteAnimator::CacheAnimationSet( path ) )
	{
		Warning( "Entity (%s) failed to cache animation set: %s\n", GetClassIdentifier(), path );
		return false;
	}

	return true;
}

void BaseAnimated::SetAnimation( const SpriteAnimation *animation )
{
	animator_.SetAnimation( animation );
}
