// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Tree.cpp
 *  Grows around the world, can be planted by characters.
 *------------------------------------------------------------------------------------*/

#include "../Game.h"

#include "Tree.h"

using namespace ct;
using namespace ct::game;

REGISTER_ENTITY( Tree, ds::Tree )

ds::Tree::Tree() = default;
ds::Tree::~Tree() = default;

void ds::Tree::Spawn()
{
	SuperClass::Spawn();

	health_ = random::GenerateRandomInteger( 1, 100 );
	maxHealth_ = random::GenerateRandomInteger( health_, 200 );

	sprite_ = ct::spriteManager->GetSprite( "sprites/environment/tree00.gif", SpriteManager::SPRITE_GROUP_ENTITY );
	offset_ = math::Vector2( -2, 2 );
}

void ds::Tree::Precache()
{
	SuperClass::Precache();

	ct::spriteManager->GetSprite( "sprites/environment/tree00.gif", SpriteManager::SPRITE_GROUP_ENTITY );
}
