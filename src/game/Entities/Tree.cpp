/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *
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
