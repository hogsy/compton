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
/*--------------------------------------------------------------------------------------
 * Tree.cpp
 *  Grows around the world, can be planted by characters.
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Tree.h"
#include "Random.h"

REGISTER_ENTITY( Tree, vc::Tree )

vc::Tree::Tree() {}
vc::Tree::~Tree() {}

void vc::Tree::Spawn()
{
	SuperClass::Spawn();

	myHealth	= random::GenerateRandomInteger( 1, 100 );
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
