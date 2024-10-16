// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Tree.cpp
 *  Grows around the world, can be planted by characters.
 *------------------------------------------------------------------------------------*/

#include "../app.h"
#include "../random.h"

#include "Tree.h"

REGISTER_ENTITY( Tree, Tree )

Tree::Tree()  = default;
Tree::~Tree() = default;

void Tree::Spawn()
{
	SuperClass::Spawn();

	health    = GenerateRandomInteger( 1, 100 );
	maxHealth = GenerateRandomInteger( health, 200 );
}

void Tree::Draw( const Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}
}
