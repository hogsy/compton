// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "BaseAnimated.h"

REGISTER_ENTITY( BaseAnimated, vc::BaseAnimated )

void vc::BaseAnimated::Tick()
{
	SuperClass::Tick();

	animator_.Tick();
}

void vc::BaseAnimated::Draw( const vc::Camera &camera )
{
	SuperClass::Draw( camera );

	animator_.Draw( origin - camera.position );
}

void vc::BaseAnimated::CacheAnimation( const char *path )
{
	animator_.LoadFile( path );
}

void vc::BaseAnimated::SetAnimation( const char *name )
{
	animator_.SetAnimation( name );
}
