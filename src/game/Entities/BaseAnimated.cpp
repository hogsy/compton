// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "BaseAnimated.h"

REGISTER_ENTITY( BaseAnimated, ct::BaseAnimated )

void ct::BaseAnimated::Tick()
{
	SuperClass::Tick();

	animator_.Tick();
}

void ct::BaseAnimated::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	hei::Vector2 p = origin_ + offset_;
	animator_.Draw( p - camera.position );
}

void ct::BaseAnimated::CacheAnimation( const char *path )
{
	animator_.LoadFile( path );
}

void ct::BaseAnimated::SetAnimation( const char *name )
{
	animator_.SetAnimation( name );
}
