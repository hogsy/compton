// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Tree.cpp
 *  Grows around the world, can be planted by characters.
 *------------------------------------------------------------------------------------*/

#include "engine_private.h"
#include "Tree.h"
#include "Random.h"

REGISTER_ENTITY( Tree, vc::Tree )

vc::Tree::Tree() {}
vc::Tree::~Tree() {}

void vc::Tree::Spawn()
{
	SuperClass::Spawn();

	myHealth    = random::GenerateRandomInteger( 1, 100 );
	myMaxHealth = random::GenerateRandomInteger( myHealth, 200 );
}

void vc::Tree::Draw( const vc::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}
