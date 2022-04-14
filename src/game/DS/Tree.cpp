// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Tree.cpp
 *  Grows around the world, can be planted by characters.
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "Tree.h"
#include "Random.h"

using namespace ct;
using namespace ct::game;

REGISTER_ENTITY( Tree, ds::Tree )

ds::Tree::Tree() {}
ds::Tree::~Tree() {}

void ds::Tree::Spawn()
{
	SuperClass::Spawn();

	myHealth    = random::GenerateRandomInteger( 1, 100 );
	myMaxHealth = random::GenerateRandomInteger( myHealth, 200 );

	sprite_ = ct::spriteManager->GetSprite( "sprites/environment/tree00.gif", SpriteManager::SPRITE_GROUP_ENTITY );
}

void ds::Tree::Precache()
{
	SuperClass::Precache();

	ct::spriteManager->GetSprite( "sprites/environment/tree00.gif", SpriteManager::SPRITE_GROUP_ENTITY );
	ct::spriteManager->GetSpriteSheet( "sprites/environment/tree_sprites.sdf" );
}

void ds::Tree::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( sprite_ == nullptr || !ShouldDraw( camera ) )
	{
		return;
	}

	hei::Vector2 pos = origin - camera.position;
	sprite_->Draw( pos.x, pos.y );
}
