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

	hei::Vector2 p = origin_ + offset_;
	animator_.Draw( p - camera.position );
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
